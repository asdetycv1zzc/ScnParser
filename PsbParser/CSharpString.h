#pragma once
#include<iostream>
#include<cstring>
#include<string>
#include"csharp_types.h"

template<typename _Ty>
std::string ToString(const _Ty _source, const std::string& _Method);

std::string ToUpper(const std::string& _source);
size_t Find(const std::string& _source, const char* _Src, size_t _offset);
void ReplaceAll(std::string& _source, const char* _Src, const char* _Dst);
