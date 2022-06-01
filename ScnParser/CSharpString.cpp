#include<algorithm>
#include"CSharpString.h"
using namespace std;

template<typename _Ty>
string ToString(const _Ty _source, const string& _Method)
{
	if (_Method == "X2")
	{
		char* _buffer = new char[sizeof(_Ty) / sizeof(BYTE) * 2 + 1];
		sprintf(_buffer, "%-X2", _source);
		*(_buffer + sizeof(_Ty) / sizeof(BYTE) * 2) = '\0';
		string _result(_buffer);
		delete[] _buffer;
		return _result;
	}
	else
		return "";
}

string ToUpper(const string& _source)
{
	auto _size = _source.size();
	auto _result = _source;
	for (size_t i = 0; i < _size; i++)
	{
		if (_result[i] >= 'a' && _result[i] <= 'z')
		{
			_result[i] -= 'a' - 'A';
		}
	}
	return _result;
}
size_t Find(const string& _source, const char* _Src,size_t _offset) //Always return the absolute value of the position where _Src is found
{
	auto _size = _source.size();
	auto _SrcSize = strlen(_Src);
	size_t _result = string::npos;
	if (_size * _SrcSize == 0 || _offset > _size) return _result;
	for (size_t i = _offset; i < _size; i++)
	{
		if (_source[i] == *_Src)
		{
			if (memcmp(_source.data() + i, _Src, _SrcSize * sizeof(char) / sizeof(BYTE)) == 0)
			{
				return i;
			}
		}
	}
	return _result;
}

void ReplaceAll(string& _source, const char* _Src, const char* _Dst)
{
	auto _size = _source.size();
	auto _SrcSize = strlen(_Src);
	auto _DstSize = strlen(_Dst);
	if (_size * _SrcSize * _DstSize == 0 || _SrcSize > _size) return;
	auto _pos = Find(_source, _Src, 0);
	if (_pos == string::npos) return;

	if (_SrcSize < _DstSize)
	{
		//Generate Buffer '0'
		auto _BufSize = _DstSize - _SrcSize;
		char* _zeroBuffer = new char[_BufSize];
		memset(_zeroBuffer, '0', _BufSize);

		while (_pos != string::npos)
		{
			_source.insert(_pos, _zeroBuffer);
			memcpy(_source.data() + _pos, _Dst, _DstSize);
			_pos = Find(_source, _Src, _pos + 1);
		}
		delete[] _zeroBuffer;
	}
	if (_SrcSize > _DstSize)
	{
		while (_pos != string::npos)
		{
			_source.replace(_pos, _SrcSize, _Dst);
			_pos = Find(_source, _Src, _pos + 1);
		}
	}
	
}