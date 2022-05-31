#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <map>
#include "MemoryStream.h"
#include "json.h"

#ifdef __cplusplus

#define TEXPORT extern "c" _declspec(dllexport)

#else

#define TEXPORT _declspec(dllexport)

#endif


class ScnParser
{
private:
	
	const std::vector<BYTE> _magic
	{
		0x22,0x74,0x65,0x78,
		0x74,0x73,0x22,0x20,
		0x3A,0x20,0x0A,0x09,
		0x09,0x09
	};
	/*
	const std::vector<BYTE> _magic
	{
		0x22,0x74,0x65,0x78,
		0x74,0x73,0x22,0x3A,
		0x20
	};*/
	typedef struct ScnSingleString
	{
		size_t _beginPos = 0;
		std::wstring _Content;
		size_t _endPos = 0;
		size_t _realSize = 0;
		size_t _takeupSize = 0;
	};
	typedef struct ScnString
	{
		std::wstring Speaker;
		std::vector<ScnSingleString> Content;
	};

	std::vector<ScnString> ScnStringPool;

	MemoryStream* stream;
	std::fstream fileStream;

	void ReadFile(const std::wstring& _source) noexcept;
	void ReadFile(const std::string& _source) noexcept;
	void ReadFile(const LPCWSTR _source) noexcept;
	void ReadFile(const LPCSTR _source) noexcept;

	size_t FindTextMagic() noexcept;
	size_t FindTextMagic(size_t _offset) noexcept;
	size_t FindTextEnd() noexcept;
	size_t FindTextEnd(size_t _offset) noexcept;

	constexpr static ScnString GetText(const std::wstring& _TextSegment) noexcept;

	void FindAllText() noexcept;

public:
	_declspec(dllexport) ScnParser();
	_declspec(dllexport) ScnParser(const std::string& _fileAddress);
	_declspec(dllexport) ScnParser(const std::wstring& _fileAddress);
	_declspec(dllexport) ScnParser(const LPCSTR _fileAddress);
	_declspec(dllexport) ScnParser(const LPCWSTR _fileAddress);
	_declspec(dllexport) ScnParser(BYTE* _buffer, size_t _size);
	_declspec(dllexport) ~ScnParser();
};

