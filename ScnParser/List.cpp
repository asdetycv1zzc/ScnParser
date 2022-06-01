#include<iostream>
#include"List.h"
using namespace std;

/*
template<typename T,size_t _size>
T List<T,_size>::at(size_t _Pos) const
{
	return _baseSpan[_Pos];
}

template<typename T, size_t _size>
constexpr span<T, _size>::iterator List<T, _size>::begin() const
{
	return _baseSpan.begin();
}
template<typename T, size_t _size>
constexpr span<T, _size>::iterator List<T, _size>::end() const
{
	return _baseSpan.end();
}

template<typename T, size_t _size>
size_t List<T, _size>::size() const
{
	return _baseSpan.size();
}

template<typename T, size_t _size>
void List<T, _size>::resize(size_t _Size)
{
	resize(_Size, (T)NULL);
}
template<typename T, size_t _size>
void List<T, _size>::resize(size_t _Size, const T& _Init_Element)
{
	this->_RealSize = _Size;
	if (this->_RealSize < _Size)
	{
		auto _needed_size = _Size - this->_RealSize;
		for (size_t i = 0; i < _needed_size; i++)
		{
			this->push_back(_Init_Element);
		}
	}
}

template<typename T, size_t _size>
void List<T, _size>::push_back(const T& _Element)
{
	_baseSpan[_RealSize] = _Element;
	_RealSize++;
}

template<typename T, size_t _size>
T& List<T, _size>::operator[](size_t _Pos) const
{
	return _baseSpan[_Pos];
}

template<typename T, size_t _size>
List<T, _size>::List()
{
	this->_baseSpan = span<T, _size>();
}
template<typename T, size_t _size>
List<T, _size>::List(const std::initializer_list<T>& _init)
{
	auto _Size = _init.size();
	for (auto i = _init.begin(); i != _init.end(); i++)
	{
		this->push_back(*i);
	}
}
template<typename T, size_t _size>
List<T, _size>::List(const std::vector<T>& _vec)
{
	//this = new List<T, _size>();
	auto _Size = _vec.size();
	for (size_t i = 0; i < _Size; i++)
	{
		this->push_back(_vec[i]);
	}
}
template<typename T, size_t _size>
List<T, _size>::List(size_t _Size)
{
	for (size_t i = 0; i < _Size; i++)
	{
		this->push_back(0);
	}
}
template<typename T, size_t _size>
List<T, _size>::List(size_t _Size, const T& _Init_Element)
{
	for (size_t i = 0; i < _Size; i++)
	{
		this->push_back(_Init_Element);
	}
}
*/