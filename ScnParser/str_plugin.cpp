#include<iostream>
#include<string>
#include<vector>
#include<Windows.h>
#include"str_plugin.h"
using namespace std;

size_t Count(const wstring& _source, const wstring& _Val)
{
	size_t _result = 0;
	if (_source.find(_Val) == wstring::npos) return _result;
	auto _init_pos = 0;
	while (_source.find(_Val, _init_pos) != wstring::npos)
	{
		_result++;
		_init_pos = _source.find(_Val, _init_pos) + 1;
	}
	return _result;
}
size_t Count(const wstring& _source, const wchar_t _Val)
{
	size_t _result = 0;
	auto _size = _source.size();
	for (size_t i = 0; i < _size; i++)
	{
		if (_source[i] == _Val)_result++;
	}
	return _result;
}
void Delete(wstring& _source, const wchar_t _Val)
{
	if (_source.find(_Val) == wstring::npos) return;
	for (auto i = _source.begin(); i != _source.end();)
	{
		if (*i == _Val)
			_source.erase(i);
		else
			i++;
	}
	return;
}
size_t wstrlen(const wchar_t* _String)
{
	size_t _result = 0;
	while (*(_String + _result) != L'\0')
	{
		_result++;
	}
	return _result;
}
vector<wstring> splitwstr(const wstring& _source, const wstring& _seperate_mark)
{
	vector<wstring> _result;
	auto _seperate_size = _seperate_mark.size();
	size_t beginPos = 0, endPos, _size;
	while (_source.find(_seperate_mark, beginPos) != wstring::npos)
	{
		endPos = _source.find(_seperate_mark, beginPos);
		_size = endPos - beginPos;
		_result.push_back(_source.substr(beginPos, _size));
		beginPos = endPos + _seperate_size;
	}
	_result.push_back(_source.substr(beginPos));
	return _result;
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