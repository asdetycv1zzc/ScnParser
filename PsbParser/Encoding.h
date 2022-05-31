#pragma once
#include<iostream>
#include<string>
#include"List.h"
#include"csharp_types.h"

static class Encoding
{
public:
	static class _Unicode
	{
	public:
		static std::string GetString(const std::vector<BYTE>& _stream);
		static std::wstring GetWstring(const std::vector<BYTE>& _stream);
		static List<BYTE> GetBytes(const std::string& _source);
	};
	static class _UTF8
	{
	public:
		static std::string GetString(const std::vector<BYTE>& _stream);
		static std::wstring GetWstring(const std::vector<BYTE>& _stream);
		static List<BYTE> GetBytes(const std::string& _source);
	};
	static _Unicode Unicode;
	static _UTF8 UTF8;
	template<typename T>
	static List<BYTE> GetBytes(const std::vector<T>& _source);
	static List<BYTE> GetBytes(const std::string& _source);
	static List<BYTE> GetBytes(const std::wstring& _source);
	//static std::string GetString(const std::vector<BYTE>& _stream);
};


std::wstring StringToWString(const std::string& str);
std::string WStringToString(const std::wstring& wstr);
std::string UnicodeToUtf8(const std::wstring& unicode);



