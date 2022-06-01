#include<iostream>
#include<stdio.h>
#include<string>
#include"Parser.h"
#include"Encoding.h"
using namespace std;

int wmain(int argc, wchar_t* argv[])
{
	wstring _fileAddress = L"N:\\Users\\John\\source\\repos\\PsbParser\\Debug\\サガプラ講義　あてな　２.txt.json";
	//_fileAddress.reserve(0x200);
	//setlocale(LC_ALL, "");
	if (argc > 1)
	{
		_fileAddress = argv[1];
	}
	else
	{
		//scanf("%s",_fileAddress.c_str());
		ScnParser parser;
		parser.Init(_fileAddress.c_str());
		ulong _size;
		parser.Parse(NULL, _size);
	}
}