#include "Parser.h"
#include "MemoryStream.h"
#include "CSharpString.h"
#include "str_plugin.h"
#include "Encoding.h"
#include "json/json.h"
#include <iostream>
#include <memory>
#include <string>
using namespace std;

void ScnParser::ReadFile(const string& _source) noexcept
{
	return ReadFile(StringToWString(_source));
}
void ScnParser::ReadFile(const LPCSTR _source) noexcept
{
	return ReadFile(string(_source));
}
void ScnParser::ReadFile(const LPCWSTR _source) noexcept
{
	return ReadFile(wstring(_source));
}
void ScnParser::ReadFile(const wstring& _source) noexcept
{
	fileStream.open(_source.c_str(), ios::in | ios::binary);
	fileStream.seekg(0, ios::end);
	size_t _size = fileStream.tellg();
	fileStream.seekg(0, ios::beg);

	BYTE* _buffer = new BYTE[_size];

	fileStream.read((char*)_buffer, _size * sizeof(BYTE));

	if (stream == nullptr)
	{
		stream = new MemoryStream(_buffer, _size);
	}
	else
	{
		stream->Dispose();
		stream = new MemoryStream(_buffer, _size);
	}
	fileStream.close();
}

size_t ScnParser::FindTextMagic() noexcept
{
	vector<BYTE> _Mark = _magic;
	size_t _Pos = 0;
	size_t _size = stream->GetSize();
	size_t _searchSize = _size - _Mark.size();
	for (size_t i = 0; i < _searchSize; i++)
	{
		if (*(stream->GetBase() + i) == _Mark[0])
		{
			if (memcmp(stream->GetBase() + i, _Mark.data(), _Mark.size()) == 0)
			{
				return i + _Mark.size();
			}
		}
	}
	return wstring::npos;
}
size_t ScnParser::FindTextMagic(size_t _offset) noexcept
{
	vector<BYTE> _Mark = _magic;
	size_t _Pos = 0 + _offset;
	size_t _size = stream->GetSize();
	size_t _searchSize = _size - _Mark.size() - _offset;
	for (size_t i = _offset; i < _offset + _searchSize; i++)
	{
		if (*(stream->GetBase() + i) == _Mark[0])
		{
			if (memcmp(stream->GetBase() + i, _Mark.data(), _Mark.size()) == 0)
			{
				return i + _Mark.size();
			}
		}
	}
	return wstring::npos;
}

size_t ScnParser::FindTextEnd() noexcept
{
	auto _beginPos = FindTextMagic();
	size_t _Pos = _beginPos;
	size_t ClosureCount = 0;
	do
	{
		if (*((char*)(stream->GetBase() + _Pos)) == '[' || *((char*)(stream->GetBase() + _Pos)) == '{')
			ClosureCount += 1;
		if (*((char*)(stream->GetBase() + _Pos)) == ']' || *((char*)(stream->GetBase() + _Pos)) == '}')
			ClosureCount -= 1;
		_Pos++;
	} while (ClosureCount != 0);
	return _Pos - 1;
}
size_t ScnParser::FindTextEnd(size_t _offset) noexcept
{
	auto _beginPos = FindTextMagic(_offset);
	size_t _Pos = _beginPos;
	size_t ClosureCount = 0;
	do
	{
		if (*((char*)(stream->GetBase() + _Pos)) == '[' || *((char*)(stream->GetBase() + _Pos)) == '{')
			ClosureCount += 1;
		if (*((char*)(stream->GetBase() + _Pos)) == ']' || *((char*)(stream->GetBase() + _Pos)) == '}')
			ClosureCount -= 1;
		_Pos++;
	} while (ClosureCount != 0);
	return _Pos - 1;
}
void ScnParser::FindAllText() noexcept
{
	size_t _beginPos = FindTextMagic();
	size_t _endPos = FindTextEnd();
	while (_beginPos != wstring::npos)
	{
		_endPos = FindTextEnd(_beginPos - _magic.size());
		size_t _size = _endPos - _beginPos + 1;

		char* _temp = new char[_size];
		memcpy(_temp, stream->GetBase() + _beginPos, _size * sizeof(char) / sizeof(BYTE));
		string _content = _temp;

		delete[] _temp;
		_temp = nullptr;

		Json::Reader reader;
		Json::Value root;
		wstring _speaker;
		vector<wstring> _words;

		if (reader.parse(_content.c_str(), _content.c_str() + _content.size(), root))
		{
			for (auto& block : root)
			{
				ScnString _scnString;
				
				_words.clear();

				_scnString.Speaker = StringToWString(block[0].asString().c_str());

				bool _finished = false;

				for (auto& singleLine : block[1][0])
				{
					try
					{
						if (!singleLine.isNull() && _finished != true)
						{
							if (singleLine.isInt())
								_finished = true;
							else
							{
								ScnSingleString _scnSingleString;
								_scnSingleString._Content = StringToWString(singleLine.asString().c_str());
								_scnSingleString._beginPos = singleLine.getOffsetStart() + _beginPos + 1; //Go over the " symbol
								_scnSingleString._Ptr = (uint)(stream->GetBase() + _scnSingleString._beginPos);
								stream->Seek(_scnSingleString._beginPos, MEM_BEG);
								char buffer = 0;
								do
								{
									stream->Read(&buffer, sizeof(char));
								} 
								while (buffer != '\"');
								_scnSingleString._endPos = stream->GetPosition() - 1;
								//_scnSingleString._endPos = _scnSingleString._beginPos + _scnSingleString._Content.size() * sizeof(char) / sizeof(BYTE);
								_scnSingleString._realSize = _scnSingleString._Content.size();
								_scnSingleString._takeupSize = _scnSingleString._endPos - _scnSingleString._beginPos;
								_scnString.Content.push_back(_scnSingleString);
							}
								
						}
					}
					catch (...)
					{
						continue;
					}
				}
				//_result.push_back(make_pair(_speaker, _words));
				ScnStringPool.push_back(_scnString);
			}


		}
		_beginPos = FindTextMagic(_endPos);
	}
	return;
}
_declspec(dllexport)
ScnParser::ScnParser()
{
	stream = nullptr;
}
_declspec(dllexport)
ScnParser::ScnParser(const string& _fileAddress)
{
	stream = nullptr;
	ReadFile(_fileAddress);
	FindAllText();
}
_declspec(dllexport)
ScnParser::ScnParser(const wstring& _fileAddress)
{
	stream = nullptr;
	ReadFile(_fileAddress);
	FindAllText();
}
_declspec(dllexport)
ScnParser::ScnParser(const LPCSTR _fileAddress)
{
	stream = nullptr;
	ReadFile(_fileAddress);
	FindAllText();
}
_declspec(dllexport)
ScnParser::ScnParser(const LPCWSTR _fileAddress)
{
	stream = nullptr;
	ReadFile(_fileAddress);
	FindAllText();
}
_declspec(dllexport)
ScnParser::ScnParser(BYTE* _buffer, size_t _size)
{
	stream = new MemoryStream(_buffer, _size);
	FindAllText();
}
_declspec(dllexport)
ScnParser::~ScnParser()
{
	if (stream != nullptr)
	{
		stream->Dispose();
	}

	vector<ScnString>().swap(ScnStringPool);

	if (fileStream.is_open())
	{
		fileStream.close();
	}
}
