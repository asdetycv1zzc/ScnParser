#pragma once
#include <iostream>
#include "csharp_types.h"
#include <vector>
#include <span>
#include <Windows.h>
#include <memory>
#include <string>

template<typename T, size_t _size = std::dynamic_extent>
class List
{
private:
	std::vector<std::unique_ptr<T> > _PointerList;

	std::span<T, _size> _baseSpan;
	size_t _RealSize = 0;
	size_t _AllocatedSize = 0;

public:
	T at(size_t _Pos) const
	{
		return _baseSpan[_Pos];
	}

	std::span<T, _size>::iterator begin() const
	{
		return _baseSpan.begin();
	}
	std::span<T, _size>::iterator end() const
	{
		return _baseSpan.end();
	}

	size_t size() const
	{
		return _RealSize;
	}
	T* data() const
	{
		return _baseSpan.data();
	}

	void resize(size_t _Size)
	{
		resize(_Size, *(T*)nullptr);
	}
	void resize(size_t _Size, const T& _Init_Element)
	{
		if (_RealSize < _Size)
		{
			_PointerList.push_back(std::move(std::unique_ptr<T>(new T[_Size])));

			memcpy(_PointerList[_PointerList.size() - 1].get(), _baseSpan.data(), _RealSize * sizeof(T));
			if (&_Init_Element != nullptr)
			{
				for (size_t i = _RealSize; i < _Size; i++)
				{
					*(_PointerList[_PointerList.size() - 1].get() + i) = _Init_Element;
				}
			}
			std::span<T> _newSpan(_PointerList[_PointerList.size() - 1].get(), _Size);
			_baseSpan = _newSpan;
			_AllocatedSize = _Size;
			_RealSize = _AllocatedSize;
		}
		else if (_RealSize > _Size)
		{
			_PointerList.push_back(std::move(std::unique_ptr<T>(new T[_Size])));

			memcpy(_PointerList[_PointerList.size() - 1].get(), _baseSpan.data(), _Size * sizeof(T));
			std::span<T> _newSpan(_PointerList[_PointerList.size() - 1].get(), _Size);
			_baseSpan = _newSpan;
			_AllocatedSize = _Size;
			_RealSize = _AllocatedSize;
		}
	}
	void resize(const std::vector<T>& _vec)
	{
		//std::unique_ptr<T> _temp(new T[_RealSize + _vec.size()]);
		_PointerList.push_back(std::move(std::unique_ptr<T>(new T[_RealSize + _vec.size()])));

		memcpy(_PointerList[_PointerList.size() - 1].get(), _baseSpan.data(), _RealSize * sizeof(T));
		memcpy(_PointerList[_PointerList.size() - 1].get() + _RealSize, _vec.data(), _vec.size() * sizeof(T));
		std::span<T> _newSpan(_PointerList[_PointerList.size() - 1].get(), _RealSize + _vec.size());
		_baseSpan = _newSpan;
		_RealSize = _RealSize + _vec.size();
		_AllocatedSize = _RealSize;
	}
	void allocate(size_t _Size)
	{
		//std::unique_ptr<T> _temp(new T[_RealSize + _Size]);
		_PointerList.push_back(std::move(std::unique_ptr<T>(new T[_RealSize + _Size])));

		memcpy(_PointerList[_PointerList.size() - 1].get(), _baseSpan.data(), _RealSize * sizeof(T));
		std::span<T> _newSpan(_PointerList[_PointerList.size() - 1].get(), _Size + _RealSize);
		_baseSpan = _newSpan;
		_AllocatedSize = _Size + _RealSize;
	}

	void push_back(const T& _Element)
	{
		if (_AllocatedSize <= (_RealSize * 2))
		{
			auto _Pos = _RealSize;
			allocate((_RealSize + 1) * 2);
			_RealSize++;
			_baseSpan[_Pos] = _Element;
		}
		else
		{
			_baseSpan[_RealSize] = _Element;
			_RealSize++;
		}
	}

	const std::vector<T> AsVector() const
	{
		std::vector<T> _result(_RealSize);
		memcpy(_result.data(), _baseSpan.data(), _RealSize * sizeof(T) / sizeof(BYTE));
		return _result;
	}

	T& operator[](size_t _Pos)
	{
		if (_Pos < _RealSize)
			return _baseSpan[_Pos];
		else
		{
			resize(_Pos + 1);
			return _baseSpan[_Pos];
		}
	}
	T& operator[](size_t _Pos) const
	{
		return _baseSpan[_Pos];
	}
	void operator=(const List<T, _size>& _Right)
	{
		_PointerList.push_back(std::move(std::unique_ptr<T>(new T[_Right._RealSize])));

		memcpy(_PointerList[_PointerList.size() - 1].get(), _Right._baseSpan.data(), _Right._RealSize * sizeof(T));
		_baseSpan = std::span<T>(_PointerList[_PointerList.size() - 1].get(), _Right._RealSize);
		//memcpy(_baseSpan.data(), _Right._baseSpan.data(), _Right._RealSize * sizeof(T));
		_AllocatedSize = _Right._AllocatedSize;
		_RealSize = _Right._RealSize;
	}

	List<T, _size>()
	{
		this->_baseSpan = span<T, _size>();
	}
	List<T, _size>(const std::initializer_list<T>& _init)
	{
		auto _Size = _init.size();
		resize(_Size);
		for (size_t i = 0; i < _Size; i++)
		{
			_baseSpan[i] = *(_init.begin() + i);
		}
	}
	List<T, _size>(const std::vector<T>& _vec)
	{
		resize(_vec);
	}
	List<T, _size>(size_t _Size)
	{
		resize(_Size);
	}
	List<T, _size>(size_t _Size, const T& _Init_Element)
	{
		resize(_Size, _Init_Element);
	}
	List<T, _size>(const List<T, _size>& _init)
	{
		_PointerList.push_back(std::move(std::unique_ptr<T>(new T[_init._RealSize])));
		
		//memcpy(_baseSpan.data(), _init._baseSpan.data(), _init._RealSize * sizeof(T));
		memcpy(_PointerList[_PointerList.size() - 1].get(), _init._baseSpan.data(), _init._RealSize * sizeof(T));
		_baseSpan = std::span<T>(_PointerList[_PointerList.size() - 1].get(), _init._RealSize);
		_AllocatedSize = _init._AllocatedSize;
		_RealSize = _init._RealSize;
	}
	List<T, _size>(const T* _Ptr, size_t _Size)
	{
		_PointerList.push_back(std::move(std::unique_ptr<T>(new T[_Size])));

		memcpy(_PointerList[_PointerList.size() - 1].get(), _Ptr, _Size * sizeof(T));
		_baseSpan = std::span<T>(_PointerList[_PointerList.size() - 1].get(), _Size);
		//memcpy(_baseSpan.data(), _Ptr, _Size * sizeof(T));

		_AllocatedSize = _Size;
		_RealSize = _Size;
	}

	void clear()
	{
		for (auto& k : _PointerList)
		{
			if (k.get() != nullptr)
			{
				k.reset();
			}
		}
	}

	~List<T, _size>()
	{
		for (auto& k : _PointerList)
		{
			if (k.get() != nullptr)
			{
				k.reset();
			}
		}
	}
};
