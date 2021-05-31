#pragma once
#include <iostream>
class FileInfo
{
private:
	friend std::ostream& operator<<(std::ostream&, const FileInfo&) noexcept;
public:
	char* FileName;
	char* TimeStamp;
	int FileSize;
	int PackedSize;
	int Offset;
	char* PatchFileName;
	FileInfo() noexcept;
	char* TimeStampChar();
	std::string GetFormattedString();
};

