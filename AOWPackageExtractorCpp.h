#pragma once
#include <vector>
#include "FileInfo.h"
#if defined(_WIN32)
#define ZLIB_WINAPI
#endif
#include "zlib/zconf.h"
#include "zlib/zlib.h"


void Debug(std::string c, const char* _function, const char* _file, int _line);
std::string replace(std::string str, const std::string& from, const std::string& to);
std::string GetDirectory(char* str);

void ShowLocales();

void help();

bool StringEquality(const char*, const char*);

std::vector<FileInfo> GetFileList();

template <typename T>
T* charToType(char* c, int size);

unsigned short charToShort(char* c);

int* charToInt(char* c);

char* IntToChar(int i);

void NamePrinter(FileInfo file);

void InfoPrinter(FileInfo file);

bool CheckExtension(char* ext, std::string path);


/// <summary>
/// 
/// </summary>
/// <param name="list"></param>
/// <param name="filename"></param>
/// <param name="isRegex"></param>
/// <returns></returns>
bool Contains(std::vector<std::string>& list, std::string filename, bool isRegex);



void WritePackage(std::vector<FileInfo*>& files, std::string fileName, std::string directory);

void Extract();
std::string CharToHex(char* c);

void ParseArgs(int, char**);


int Overflow();

template<typename T>
inline T* charToType(char* c)
{
	/*switch (typeid(T).name())
	{
	case typeid(int).name():
	{
		int* val;
		std::memcpy(&val, c, sizeof(int));
		return val;
	}
	case typeid(short).name():
	{
		return (short*)((c[0] << 8) | c[1]);
	}
	case typeid(char).name():
	case typeid(byte).name():
	{
		return (T)c[0];
	}
	default:
		return (T)c;
	}*/
	return (T)c;
}

template<typename T>
inline T* charToType(char* c, int size)
{
	return nullptr;
}
