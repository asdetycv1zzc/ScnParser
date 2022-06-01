#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <map>
#include "MemoryStream.h"
#include "json.h"

class ScnParser
{
protected:
	typedef struct ScnSingleString
	{
		uint _Ptr;
		size_t _beginPos = 0;
		size_t _endPos = 0;
		size_t _real_size = 0;
		size_t _memory_size = 0;
		size_t _in_file_size = 0;
		std::wstring _Content;
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

	
};
class ScnJsonParser : public ScnParser
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

	size_t FindTextMagic() noexcept;
	size_t FindTextMagic(size_t _offset) noexcept;
	size_t FindTextEnd() noexcept;
	size_t FindTextEnd(size_t _offset) noexcept;

	void FindAllText() noexcept;
public:
	_declspec(dllexport) bool WINAPI Parse(_Out_ BYTE* _Dest, _Out_ ulong& _size);
	_declspec(dllexport) bool WINAPI Init(_In_ LPCWSTR _FileAddress);
	_declspec(dllexport) WINAPI ScnJsonParser();
	_declspec(dllexport) WINAPI ScnJsonParser(const std::string& _fileAddress);
	_declspec(dllexport) WINAPI ScnJsonParser(const std::wstring& _fileAddress);
	_declspec(dllexport) WINAPI ScnJsonParser(const LPCSTR _fileAddress);
	_declspec(dllexport) WINAPI ScnJsonParser(const LPCWSTR _fileAddress);
	_declspec(dllexport) WINAPI ScnJsonParser(BYTE* _buffer, size_t _size);
	_declspec(dllexport) WINAPI ~ScnJsonParser();

	_declspec(dllexport) void WINAPI Dispose() noexcept;
};

extern "C" _declspec(dllexport) ScnJsonParser * WINAPI EstablishJsonParserPointer();
extern "C" _declspec(dllexport) void WINAPI DispatchJsonParserPointer(ScnJsonParser * _Ptr);
extern "C" _declspec(dllexport) bool WINAPI JsonParserInit(ScnJsonParser * _Ptr, _In_ LPCWSTR _FileAddress);
extern "C" _declspec(dllexport) bool WINAPI JsonParserParse(ScnJsonParser * _Ptr, _Out_ BYTE * _Dest, _Out_ ulong & _size);

class ScnRawParser : public ScnParser
{
private:
	struct PSBMagicHeader
	{
		uint Signature;
		uint Version;
		uint Unk;
		uint Unk2;
		uint StrOffPos;
		uint StrDataPos;
		uint ResOffPos;
		uint ResLenPos;
		uint ResDataPos;
		uint ResIndexTree;
	} Header;

	void ValidateMagic() noexcept;
	void ReadMagicHeader() noexcept;
	bool ReadAllText() noexcept;
};