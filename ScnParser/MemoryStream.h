#pragma once
#define MEM_BEG 0
#define MEM_CUR 1
#define MEM_END 2
#include<iostream>
#include<vector>
#include<memory>
#include"csharp_types.h"


class MemoryStream
{
private:
	std::unique_ptr<BYTE> _base;
public:
	uint Position = 0;
	uint Length = 0;

	BYTE* GetBase();
	uint GetPosition();
	size_t GetSize();

	long Seek(unsigned int _offset, int _Mode = MEM_BEG);

	long Read(std::vector<BYTE>& _buffer, unsigned int _size);
	long Read(void* _buffer, unsigned int _size);

	void Write(const std::vector<BYTE>& _source);
	void Write(const BYTE* _Src, size_t _Size);
	void Write(size_t _Pos, const BYTE* _Src, size_t _Size);

	void Insert(const size_t _offset, const std::vector<BYTE>& _source);
	void Insert(const size_t _offset, const void* _source, size_t _Size);

	void Erase(const size_t _offset, size_t _Size);

	MemoryStream(BYTE* _Ptr, unsigned int _size);
	MemoryStream(const std::vector<BYTE>& _source);
	~MemoryStream();
	void Close();
	void Dispose();
};