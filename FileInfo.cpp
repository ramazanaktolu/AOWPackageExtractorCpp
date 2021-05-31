#include "FileInfo.h"
#include <iostream>
#include <string>

//unsigned char TimeStamp[9];
FileInfo::FileInfo() noexcept
{
	TimeStamp = new char[10]{ "\xDC\x07\x07\x1F\x06\x38\x23\x0\x0" }; //Default timestamp
	//printf("initialized fileinfo\n");
}

char* FileInfo::TimeStampChar()
{
	char c[9];
	for (int i = 0; i < 9; i++)
	{
		c[i] = TimeStamp[i];
	}
	return c;
}

std::string FileInfo::GetFormattedString()
{
	std::string str;
	str.append(FileName);
	str.append("\t");
	str.append(std::to_string(Offset));
	str.append("\t");
	str.append(std::to_string(FileSize));
	str.append("\t");
	str.append(std::to_string(PackedSize));
	return str;
}

std::ostream& operator<<(std::ostream& strm, const FileInfo& fi) noexcept
{
	//auto a = (std::string)fi.FileName  + "\t" + (std::string)(char*)fi.Offset + "\t" + (std::string)(char*)fi.FileSize + "\t" + (std::string)(char*)fi.PackedSize;
	return strm << fi.FileName << "\t" << fi.Offset << "\t" << fi.FileSize << "\t" << fi.PackedSize;
	// TODO: insert return statement here
}