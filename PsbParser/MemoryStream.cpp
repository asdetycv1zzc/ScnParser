#include<vector>
#include<memory>
#include"MemoryStream.h"

using namespace std;

BYTE* MemoryStream::GetBase()
{
	return _base.get();
}
uint MemoryStream::GetPosition()
{
	return Position;
}
size_t MemoryStream::GetSize()
{
	return Length;
}
void MemoryStream::Close()
{
	_base.reset();
}
void MemoryStream::Dispose()
{
	Close();
}
long MemoryStream::Seek(unsigned int _offset, int _Mode)
{
	switch (_Mode)
	{
	case MEM_BEG:
	{
		Position = _offset;
		break;
	}
	case MEM_CUR:
	{
		Position += _offset;
		break;
	}
	case MEM_END:
	{
		Position = Length - _offset;
		break;
	}
	default:
	{
		break;
	}
	}
	return Position;
}
long MemoryStream::Read(vector<BYTE>& _buffer, unsigned int _size)
{
	Read(_buffer.data(), _size);
	return _size;
}
long MemoryStream::Read(void* _buffer, unsigned int _size)
{
	memcpy(_buffer, _base.get() + Position, _size);
	Position += _size;
	return _size;
}
void MemoryStream::Write(const vector<BYTE>& _source)
{
	Write(_source.data(), _source.size());
}
void MemoryStream::Write(const BYTE* _Src, size_t _Size)
{
	Write(Position, _Src, _Size);
}
void MemoryStream::Write(size_t _Pos, const BYTE* _Src, size_t _Size)
{
	memcpy(_base.get() + _Pos, _Src, _Size);
	Position += _Size;
}
MemoryStream::MemoryStream(BYTE* _Ptr,unsigned int _size)
{
	Position = 0;
	Length = _size;
	_base = std::unique_ptr<BYTE>(_Ptr);
}
MemoryStream::MemoryStream(const vector<BYTE>& _source)
{
	std::unique_ptr<BYTE> _real(new BYTE[_source.size()]);
	Length = _source.size();
	Position = 0;
	memcpy(_real.get(), _source.data(), _source.size() * sizeof(BYTE));
	_base = std::move(_real);
}
MemoryStream::~MemoryStream()
{
	this->Dispose();
}