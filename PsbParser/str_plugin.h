#pragma once
#include<iostream>
#include<vector>
#include<string>

size_t Count(const std::wstring& _source, const std::wstring& _Val);
size_t Count(const std::wstring& _source, const wchar_t _Val);
void Delete(std::wstring& _source, const wchar_t _Val);
size_t wstrlen(const wchar_t* _String);
std::vector<std::wstring> splitwstr(const std::wstring& _source, const std::wstring& _seperate_mark);
std::string WStringToString(const std::wstring& ws);
std::wstring StringToWString(const std::string& s);