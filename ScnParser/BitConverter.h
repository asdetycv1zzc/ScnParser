#pragma once
#include"csharp_types.h"
class BitConverter
{
public:
	static long ToInt64(const BYTE* _source);
	static uint ToUInt32(const BYTE* _source);
	static uint ToUInt32(const BYTE* _source,int StartIndex);
	static ulong ToUInt64(const BYTE* _source);
	static void TryWriteBytes(BYTE* _destination, uint _Val);
	template<typename T>
	static std::vector<BYTE> GetBytes(const T& _source)
	{
		auto _Ptr = &_source;
		std::vector<BYTE> _result;
		_result.resize(sizeof(T) / sizeof(BYTE));
		memcpy(_result.data(), _Ptr, sizeof(T) / sizeof(BYTE));
		return _result;
	}
};