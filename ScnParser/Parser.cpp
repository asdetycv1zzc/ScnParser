#include "Parser.h"
#include "MemoryStream.h"
#include "CSharpString.h"
#include "str_plugin.h"
#include "Encoding.h"
#include "BitConverter.h"
#include "List.h"
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

size_t ScnJsonParser::FindTextMagic() noexcept
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
size_t ScnJsonParser::FindTextMagic(size_t _offset) noexcept
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

size_t ScnJsonParser::FindTextEnd() noexcept
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
size_t ScnJsonParser::FindTextEnd(size_t _offset) noexcept
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
void ScnJsonParser::FindAllText() noexcept
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

		if (reader.parse(_content.c_str(), _content.c_str() + _content.size(), root))
		{
			for (auto& block : root)
			{
				ScnString _scnString;

				_scnString.Speaker = StringToWString(block[0].asString().c_str());


				for (auto& singleLine : block[1][0])
				{
					if (singleLine.isInt() || singleLine.isNull())
						continue;
					try
					{
						ScnSingleString _scnSingleString;
						_scnSingleString._Content = StringToWString(singleLine.asString().c_str());
						_scnSingleString._beginPos = singleLine.getOffsetStart() + _beginPos + 1; //Go over the " symbol
						//_scnSingleString._Ptr = (uint)(stream->GetBase() + _scnSingleString._beginPos);
						stream->Seek(_scnSingleString._beginPos, MEM_BEG);
						char buffer = 0;
						do
						{
							stream->Read(&buffer, sizeof(char));
						} while (buffer != '\"');
						_scnSingleString._endPos = stream->GetPosition() - 1;
						//_scnSingleString._endPos = _scnSingleString._beginPos + _scnSingleString._Content.size() * sizeof(char) / sizeof(BYTE);
						_scnSingleString._real_size = _scnSingleString._Content.size();
						_scnSingleString._memory_size = _scnSingleString._real_size * sizeof(wchar_t);
						_scnSingleString._in_file_size = _scnSingleString._endPos - _scnSingleString._beginPos;
						_scnString.Content.push_back(_scnSingleString);

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


extern "C" _declspec(dllexport)
bool WINAPI ScnJsonParser::Parse(_Out_ BYTE * _Dest, _Out_ ulong & _size)
{
	if (ScnStringPool.empty())
	{
		FindAllText();
	}
	vector<BYTE> _result(sizeof(size_t));
	memcpy(_result.data(), BitConverter::GetBytes(ScnStringPool.size()).data(), sizeof(size_t));

	for (size_t i = 0; i < ScnStringPool.size(); i++)
	{
		List<BYTE> _speaker = Encoding::GetBytes(ScnStringPool[i].Speaker);
		vector<BYTE> _speaker_size = BitConverter::GetBytes(_speaker.size() * sizeof(BYTE));
		vector<BYTE> _content(sizeof(size_t));
		memcpy(_content.data(), BitConverter::GetBytes(ScnStringPool[i].Content.size()).data(), sizeof(size_t));
		for (size_t j = 0; j < ScnStringPool[i].Content.size(); j++)
		{
			MemoryStream* _scnString_sizes = new MemoryStream(new BYTE[5 * sizeof(size_t) / sizeof(BYTE)], 5 * sizeof(size_t) / sizeof(BYTE));
			_scnString_sizes->Write(BitConverter::GetBytes(ScnStringPool[i].Content[j]._beginPos));
			_scnString_sizes->Write(BitConverter::GetBytes(ScnStringPool[i].Content[j]._endPos));
			_scnString_sizes->Write(BitConverter::GetBytes(ScnStringPool[i].Content[j]._real_size));
			_scnString_sizes->Write(BitConverter::GetBytes(ScnStringPool[i].Content[j]._memory_size));
			_scnString_sizes->Write(BitConverter::GetBytes(ScnStringPool[i].Content[j]._in_file_size));
			List<BYTE> _contentString = Encoding::GetBytes(ScnStringPool[i].Content[j]._Content);
			vector<BYTE> _singleScnString(_scnString_sizes->GetSize() + _contentString.size());
			memcpy(_singleScnString.data(), _scnString_sizes->GetBase(), _scnString_sizes->GetSize());
			memcpy(_singleScnString.data() + _scnString_sizes->GetSize(), _contentString.data(), _contentString.size());
			_content.insert(_content.end(), _singleScnString.begin(), _singleScnString.end());
			_scnString_sizes->Dispose();
		}
		_result.insert(_result.end(), _speaker_size.begin(), _speaker_size.end());
		_result.insert(_result.end(), _speaker.begin(), _speaker.end());
		_result.insert(_result.end(), _content.begin(), _content.end());
	}
	if (_Dest != nullptr)
	{
		memcpy(_Dest, _result.data(), _result.size() * sizeof(BYTE));
		vector<BYTE>().swap(_result);
	}
	_size = _result.size();
	return true;
}
extern "C" _declspec(dllexport)
bool WINAPI ScnJsonParser::Init(_In_ LPCWSTR _FileAddress)
{
	stream = nullptr;
	try
	{
		ReadFile(_FileAddress);
		return true;
	}
	catch (...)
	{
		return false;
	}
	//FindAllText();

}
extern "C" _declspec(dllexport)
WINAPI ScnJsonParser::ScnJsonParser()
{
	stream = nullptr;
}
extern "C" _declspec(dllexport)
WINAPI ScnJsonParser::ScnJsonParser(const string & _fileAddress)
{
	stream = nullptr;
	ReadFile(_fileAddress);
	//FindAllText();
}
extern "C" _declspec(dllexport)
WINAPI ScnJsonParser::ScnJsonParser(const wstring & _fileAddress)
{
	stream = nullptr;
	ReadFile(_fileAddress);
	//FindAllText();
}
extern "C" _declspec(dllexport)
WINAPI ScnJsonParser::ScnJsonParser(const LPCSTR _fileAddress)
{
	stream = nullptr;
	ReadFile(_fileAddress);
	//FindAllText();
}
extern "C" _declspec(dllexport)
WINAPI ScnJsonParser::ScnJsonParser(const LPCWSTR _fileAddress)
{
	stream = nullptr;
	ReadFile(_fileAddress);
	//FindAllText();
}
extern "C" _declspec(dllexport)
WINAPI ScnJsonParser::ScnJsonParser(BYTE * _buffer, size_t _size)
{
	stream = new MemoryStream(_buffer, _size);
	//FindAllText();
}
extern "C" _declspec(dllexport)
WINAPI ScnJsonParser::~ScnJsonParser()
{
	Dispose();
}
extern "C" _declspec(dllexport)
void WINAPI ScnJsonParser::Dispose() noexcept
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

extern "C" _declspec(dllexport)
ScnJsonParser * WINAPI EstablishJsonParserPointer()
{
	ScnJsonParser* _parser = new ScnJsonParser();
	return _parser;
}
extern "C" _declspec(dllexport)
void WINAPI DispatchJsonParserPointer(ScnJsonParser * _Ptr)
{
	_Ptr->Dispose();
	_Ptr = nullptr;
}
extern "C" _declspec(dllexport) 
bool WINAPI JsonParserInit(ScnJsonParser * _Ptr, _In_ LPCWSTR _FileAddress)
{
	return _Ptr->Init(_FileAddress);
}
extern "C" _declspec(dllexport) 
bool WINAPI JsonParserParse(ScnJsonParser * _Ptr, _Out_ BYTE * _Dest, _Out_ ulong & _size)
{
	return _Ptr->Parse(_Dest, _size);
}
