#include<iostream>
#include<string>
#include"List.h"
#include"Encoding.h"
#include"Encoding_Alt.h"
using namespace std;

string Encoding::_Unicode::GetString(const vector<BYTE>& _stream)
{
	return WStringToString(GetWstring(_stream));
}
wstring Encoding::_Unicode::GetWstring(const vector<BYTE>& _stream)
{
	auto _size = _stream.size();
	wstring _temp;
	for (size_t i = 0; i < _size / (sizeof(wchar_t) / sizeof(BYTE)); i++)
	{
		wchar_t _buffer = L'\0';
		for (int j = 0; j < sizeof(wchar_t) / sizeof(BYTE); j++)
		{
			memcpy((BYTE*)&_buffer + j, &_stream[i * sizeof(wchar_t) / sizeof(BYTE) + j], sizeof(BYTE));
		}
		_temp.push_back(_buffer);
	}
	_temp.push_back(L'\0');
	return _temp;
}
string Encoding::_UTF8::GetString(const vector<BYTE>& _stream)
{
	auto _temp = Encoding::_Unicode::GetWstring(_stream);
	return UnicodeToUtf8(_temp);
}
wstring Encoding::_UTF8::GetWstring(const vector<BYTE>& _stream)
{
	auto _temp = Encoding::_Unicode::GetWstring(_stream);
	return StringToWString(UnicodeToUtf8(_temp));
}
List<BYTE> Encoding::GetBytes(const std::string& _source)
{
	/*
	BYTE* _real = new BYTE[_source.size() * sizeof(char) / sizeof(BYTE)];
	memcpy(_real, _source.c_str(), _source.size() * sizeof(char) / sizeof(BYTE));
	List<BYTE> _result(_source.size() * sizeof(char) / sizeof(BYTE));
	for (size_t i = 0; i < _source.size() * sizeof(char) / sizeof(BYTE); i++)
	{
		_result[i] = *(_real + i);
	}
	delete[] _real;*/
	return List<BYTE>((BYTE*)_source.c_str(), _source.size() * sizeof(char) / sizeof(BYTE));
}
List<BYTE> Encoding::GetBytes(const wstring& _source)
{
	/*
	BYTE* _real = new BYTE[_source.size() * sizeof(wchar_t) / sizeof(BYTE)];
	memcpy(_real, _source.c_str(), _source.size() * sizeof(wchar_t) / sizeof(BYTE));
	List<BYTE> _result(_source.size() * sizeof(wchar_t) / sizeof(BYTE));
	for (size_t i = 0; i < _source.size() * sizeof(wchar_t) / sizeof(BYTE); i++)
	{
		_result[i] = *(_real + i);
	}
	delete[] _real;*/
	return List<BYTE>((BYTE*)_source.c_str(), _source.size() * sizeof(wchar_t) / sizeof(BYTE));
}
template<typename T>
List<BYTE> Encoding::GetBytes(const vector<T>& _source)
{
	/*
	BYTE* _real = new BYTE[_source.size() * sizeof(T) / sizeof(BYTE)];
	memcpy(_real, _source.data(), _source.size() * sizeof(T) / sizeof(BYTE));
	List<BYTE> _result(_source.size() * sizeof(T) / sizeof(BYTE));
	for (size_t i = 0; i < _source.size() * sizeof(T) / sizeof(BYTE); i++)
	{
		_result[i] = *(_real + i);
	}
	delete[] _real;*/
	return List<BYTE>((BYTE*)_source.data(), _source.size() * sizeof(T) / sizeof(BYTE));
}
List<BYTE> Encoding::_Unicode::GetBytes(const std::string& _source)
{
	/*
	BYTE* _real = new BYTE[_source.size() * sizeof(char) / sizeof(BYTE)];
	memcpy(_real, _source.c_str(), _source.size() * sizeof(char) / sizeof(BYTE));
	List<BYTE> _result(_source.size() * sizeof(char) / sizeof(BYTE));
	for (size_t i = 0; i < _source.size() * sizeof(char) / sizeof(BYTE); i++)
	{
		_result[i] = *(_real + i);
	}
	delete[] _real;*/
	return List<BYTE>((BYTE*)_source.c_str(), _source.size() * sizeof(char) / sizeof(BYTE));
}
List<BYTE> Encoding::_UTF8::GetBytes(const std::string& _source)
{
	//BYTE* _real = new BYTE[_source.size() * sizeof(char) / sizeof(BYTE)];
	//memcpy(_real, _source.c_str(), _source.size() * sizeof(char) / sizeof(BYTE));
	return List<BYTE>((BYTE*)_source.c_str(), _source.size() * sizeof(char) / sizeof(BYTE));
}

wstring StringToWString(const string& str)
{
	int num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wide = new wchar_t[num];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, num);
	std::wstring w_str(wide);
	delete[] wide;
	return w_str;
}
string WStringToString(const wstring& wstr)
{
	std::string str;
	int nLen = (int)wstr.length();
	str.resize(nLen, ' ');
	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), nLen, NULL, NULL);
	if (nResult == 0)
	{
		return "";
	}
	return str;
}
string UnicodeToUtf8(const wstring& unicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, unicode.c_str(), -1, NULL, 0, NULL, NULL);
	char* szUtf8 = (char*)malloc(len + 1);
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, unicode.c_str(), -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}