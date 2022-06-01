#include"BitConverter.h"
using namespace std;

long BitConverter::ToInt64(const BYTE* _source)
{
	BYTE* _real = new BYTE[8];
	for (int i = 0; i < 8; i++)
	{
		_real[i] = *(_source + i);
	}
	long _result = *((long*)_real);
	delete[] _real;
	return _result;
}
uint BitConverter::ToUInt32(const BYTE* _source)
{
	return ToUInt32(_source, 0);
}
uint BitConverter::ToUInt32(const BYTE* _source,int StartIndex)
{
	BYTE* _real = new BYTE[4];
	for (int i = 0; i < 4; i++)
	{
		_real[i] = *(_source + i + StartIndex);
	}
	uint _result = *((uint*)_real);
	delete[] _real;
	return _result;
}
ulong BitConverter::ToUInt64(const BYTE* _source)
{
	BYTE* _real = new BYTE[8];
	for (int i = 0; i < 8; i++)
	{
		_real[i] = *(_source + i);
	}
	ulong _result = *((ulong*)_real);
	delete[] _real;
	return _result;
}
void BitConverter::TryWriteBytes(BYTE* _destination, uint _Val)
{
	auto _real = _Val;
	auto _Ptr =  (BYTE*) & _Val;
	memcpy(_destination, _Ptr, sizeof(uint));
}