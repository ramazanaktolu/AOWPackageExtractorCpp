// AOWPackageExtractorCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//#pragma warning(suppress : 4996)

#include <Windows.h>
#include <iostream>
#include <iterator>
#include "AOWPackageExtractorCpp.h"
#include "NULLPTR_EXCEPTION.h"
#include <vector>
#include <winver.h>
#include <fstream>
#include "FileInfo.h"
#include <libloaderapi.h>
#if defined(_WIN32)
#define ASMINF
#endif


#include <cstdio>
#include <iosfwd>
#include <iomanip>
#include "zlib/zconf.h"
#include "zlib/zlib.h"
#include <filesystem>
#include <regex>
#include <map>
#include <chrono>
#include <sstream>


namespace fs = std::filesystem;
//#include <fileapi.h>


std::vector<std::wstring> locals;



BOOL CALLBACK MyFuncLocaleEx(LPWSTR pStr, DWORD dwFlags, LPARAM lparam)
{
    locals.push_back(pStr);
    return TRUE;
}


bool extract;
bool helpRequest;
std::string input;
bool version;
std::string output;
bool localeList;
bool debug;
bool IsPatchFile;
bool benchmark;
std::string FileName = "";
std::ifstream inputFile;
std::vector<FileInfo> Files;
std::string repacklocation;
std::vector<std::string> createdDirectories;
std::vector<std::string> searchList;
std::vector<std::string> regexList;
char* pathSepertor = (char*)"\\";
char* pathReplacement = (char*)"/";

char* hex;
bool fast;
int main(int argc, char** argv)
{
    //const char* test = new char[] { '\x81', 0};
    //std::string strtest = (std::string)test;

    //std::cout << +std::strtol(strtest.c_str(), nullptr, 2) << std::endl;
    //std::cout << +(unsigned char)test << std::endl;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    pathSepertor = (char*)"/";
    pathReplacement = (char*)"\\";
#endif

    ParseArgs(argc, argv);

    //setlocale(LC_ALL, "tr-TR"); //default locale works without problem

    hex = new char[16] { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B','C','D','E','F' };
    std::string directory = std::filesystem::current_path().generic_string();
    directory = replace(directory, pathSepertor, pathReplacement);
    directory.append(pathReplacement);

    std::string fullpath = ((std::string)argv[0]);

    fullpath = replace(fullpath, pathSepertor, pathReplacement);

    if (fullpath.find(pathReplacement))
    {
        auto lastindex = fullpath.find_last_of(pathReplacement);
        FileName = fullpath.substr(lastindex+1);
    }
    else
    {
        FileName = fullpath;
    }
    input = replace(input, pathSepertor, pathReplacement);


    if (version)
    {
        printf("%s", "1.0.0.1");
    }
    else if (helpRequest)
    {
        help();
    }
    else if (localeList)
    {
        ShowLocales();
    }
    else
    {
        inputFile.exceptions(std::ios::failbit | std::ios::badbit);
        inputFile = std::ifstream(input, std::ios::binary);
        if (extract)
        {
            output = replace(output, pathSepertor, pathReplacement);
            if (!CheckExtension(pathReplacement, output))
            {
                output.append(pathReplacement);
                //printf("%s\n", output.c_str());
            }
            //outFile = std::ofstream(output, std::ios::binary);
        }

        if (inputFile.is_open())
        {
            //long long benchmarkResult = {-1};
            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            


			Extract();

			inputFile.close();

            
            if (benchmark)
            {
                try
                {
                    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                    auto benchmarkResult = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 1000.0f;
                    //std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
                    std::cout << "Elapsed time: " << std::fixed << std::setprecision(3) << benchmarkResult << " seconds" << std::endl;
                }
                catch (const std::exception& e)
                {
                    std::cout << e.what();
                }
                
            }
            
        }
        else
        {
            std::cerr << "Error opening file : " << input << std::endl;
        }


    }
    return 0;
}

void Debug(std::string c, const char* _function = __builtin_FUNCTION(), const char* _file = __builtin_FILE(), int _line = __builtin_LINE())
{
    if (debug)
    {
        /*char drive[1];
        char dir[300];
        char fn[100];
        char ext[10];
        auto sp = _splitpath_s(_file, drive, 1, dir, 300, fn, 100, ext, 10);*/
        
        std::clog << "Debug: " << _function << "():" << _line << " - " << c << std::endl;
    }
}

std::string replace(std::string str, const std::string& from, const std::string& to)
{

    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

/// <summary>
/// Gets directory name from the path string
/// </summary>
/// <param name="str">string to path</param>
/// <returns>
/// std::string
/// </returns>
std::string GetDirectory(char* str)
{
    std::string str2 = str;
    size_t index = str2.find_last_of(pathReplacement);
    if (index == std::string::npos)
    {
        return "";
    }
    auto str3 = str2.substr(0, index);
    return str3;
}

void ShowLocales()
{
    EnumSystemLocalesEx(MyFuncLocaleEx, LOCALE_ALL, NULL, NULL);

    for (std::vector<std::wstring>::const_iterator str = locals.begin(); str != locals.end(); ++str)
        std::wcout << *str << std::endl;

    std::wcout << "Total " << locals.size() << " locals found." << std::endl;
}


void help()
{
    printf("Usage: %s [options]\n", FileName.data());
    printf("   Options:\n");

    //input
    //printf("\033[0; 31m");
    //printf("\033[31m");
    printf("\t%s:\t\t%s\n", "-i, --input", "input path to .package file to export or .lys file to repack.");
    printf("\t\t\t\t%s\n\n", "use \" (double quote) before and end of path if path contains space.");

    //output
    //printf("\033[32m");
    printf("\t%s:\t\t%s\n\n", "-o, --output", "output directory to export package file.");

    //set-locale
    //printf("\033[33m");
    printf("\t%s:\t%s\n\n", "-sl, --set-locale", "locale for filenames. default locale: tr-TR");

    //locale-list
    //printf("\033[31m");
    printf("\t%s:\t%s\n\n", "-ll, --locale-list", "prints locale list supported by your system");

    //search
    //printf("\033[35m");
    printf("\t%s:\t\t%s\n", "-s, --search", "searchs in file name and shows/extracts files which matched.");
    printf("\t\t\t\t%s\n\n", "this option, can be used multiple times");

    //regex
    //printf("\033[36m");
    printf("\t%s:\t\t%s\n", "-r, --regex", "searchs in file name with regular expression patterns and shows/extracts");
    printf("\t\t\t\t%s\n\n", "files which matched. this option, can be used multiple times");

    //version
    //printf("\033[32m");
    printf("\t%s:\t\t%s\n\n", "-v, --version", "this program's version");

    //help
    //printf("\033[31m");
    printf("\t%s:\t\t%s\n\n", "-h, --help", "shows this help message.");
    //printf("\033[0m");

    printf("\t%s:\t\t%s\n\n", "-f, --fast", "fast extraction. does not prints file names while extracting/repacking");

    printf("\t%s:\t\t%s\n\n", "-d, --debug", "debug mode. use if getting error.");

    printf("\t%s:\t%s\n\n", "-b, --benchmark", "prints total time elapsed for extract");


}

bool StringEquality(const char* a, const char* b)
{
    int la = strlen(a);
    int lb = strlen(b);
    if (la != lb) return false;
    for (lb = 0; lb < la; lb++)
    {
        if (tolower(a[lb] != tolower(b[lb])))
        {
            return false;
        }
    }

    return true;
}

std::vector<FileInfo> GetFileList()
{
    std::string idstr = "PCK0";
    std::vector<FileInfo> files;
    Debug("Started to get files");
    char* id = new char[4];
    inputFile.read(id, 4);//PCK0
    std::string id2 = ((std::string)id).substr(0, 4);
    if (idstr != id2)
    {
        std::clog << id2 << " is not equals to " << idstr << std::endl;
        return files;
    }
    //int *Tid = charToInt(id);
    //printf("Value: %p\n", Tid);
    Debug("getting name size");
    //printf("current position %x\n", inputFile.tellg());
    char* namesize = new char[2];
    inputFile.read(namesize, 2);//short
    unsigned short ns = charToShort(namesize);

    Debug("getting zeros");
    //printf("current position %x\n", inputFile.tellg());
    char* zeros = new char[4];
    inputFile.read(zeros, 4);
    int* TZeros = charToInt(zeros);
    //printf("Value: %p\n", TZeros);

    Debug("getting file count");
    //printf("current position %x\n", inputFile.tellg());
    char* filecount = new char[4];
    inputFile.read(filecount, 4);
    int* TFileCount = charToInt(filecount);
    //int* fc = charToType<int>(filecount);
    //printf("File count: %d\n", (int)TFileCount);

    Debug("getting headersize");
    //printf("current position %x\n", inputFile.tellg());
    char* headsize = new char[4];
    inputFile.read(headsize, 4);
    int* THeadSize = charToInt(headsize);

    //printf("current position %x\n", inputFile.tellg());
    char* tmp = new char[1];
    //delete ns;
    //inputFile.read(tmp, 1);
    //inputFile.read(tmp, 1);

    for (int i = 0; i < (int)TFileCount; i++)
	{

        
		Debug("");
        //char* namesize1 = new char[1];
        //char * namesize2 = new char[1];
        //inputFile.read(namesize1, 1);//short
        //inputFile.read(namesize2, 1);//short
        //namesize = (char*)"\0\0";
        inputFile.read(namesize, 2);//short
		ns = charToShort(namesize);

        inputFile.read(tmp, 1);
		Debug("");


		char* address = new char[4];

		inputFile.read(address, 4);
		Debug("");

		int* TAddress = charToInt(address);

		inputFile.read(zeros, 4);
		Debug("");
		int* TZeros = charToInt(zeros);


		char* filesize = new char[4];

		inputFile.read(filesize, 4);
		Debug("");

		int* TFileSize = charToInt(filesize);

		char* compresssize = new char[4];

		inputFile.read(compresssize, 4);
		Debug("");


		int* TCompressSize = charToInt(compresssize);

		char* timestamp = new char[9];

		inputFile.read(timestamp, 9);
		Debug("");



		int namelen = (short)ns - 28;//9
        Debug(std::to_string(namelen));
		char* name = new char[namelen];
        //delete ns;
		inputFile.read(name, namelen);
		Debug(((std::string)name).substr(0, namelen));

        
        FileInfo fi;
        size_t index = ((std::string)name).find("\0");
        Debug("");
        if (IsPatchFile && index > -1)
        {
            Debug("");
            auto str1 = ((std::string)name).substr(index);
            fi.PatchFileName = new char[str1.size()];
            std::strcpy(fi.PatchFileName, str1.data());
            //fi.PatchFileName = (char*)((std::string)name).substr(index).data();
            Debug("");
            auto str2 = ((std::string)name).substr(0, index);
            fi.FileName = new char[str2.size()];
            std::strcpy(fi.FileName, str2.data());
            //fi.FileName = (char*)((std::string)name).substr(0, index).data();
            Debug("");
        }
        else
        {
            Debug("");
            fi.FileName = name;
        }
        Debug("");
        name[namelen] = '\0';
        Debug("");
        //printf("Name Len: %d\n", namelen);
        //std::cout << name << std::endl;
        //printf("Name: %s\n", name);

        //inputFile.read(tmp, 1);
        Debug("");
        auto ts = timestamp;
        fi.FileName = name;
        fi.FileSize = (int)TFileSize;
        fi.Offset = (int)TAddress;
        fi.PackedSize = (int)TCompressSize;
        fi.TimeStamp = ts;
        //std::memcpy(fi.TimeStamp, ts, 9);
        //fi.TimeStamp = new int[] { ts[0], ts[1], ts[2], ts[3], ts[4], ts[5], ts[6], ts[7], ts[8]};
        files.push_back(fi);
        Debug("");
    }
    Debug("end of GetFileList");
    return files;
    //inputFile.seekg(std::streampos::seekpos())
}


unsigned short charToShort(char* c)
{
    //Debug(std::to_string(std::strlen(c) ));
    //auto s1 = unsigned short(c);
    //auto aut = std::strtoul(c, ,16)
    unsigned short s  = (unsigned short)((c[0] << 8) | c[1]);
    //short s2 = (short)s;
    Debug(std::to_string(s));
    ////Debug(std::to_string(s));
    ////return s;

    //unsigned short val;
    //std::memcpy(&val, c, 2);


    return s;
}

int* charToInt(char* c)
{
    Debug("");
    int* val;
    std::memcpy(&val, c, sizeof(val));
    return val;
}

char* IntToChar(int i)
{
    Debug("");
    char* c = new char[4];
    c[0] = i & 0x000000ff;
    c[1] = (i & 0x0000ff00) >> 8;
    c[2] = (i & 0x00ff0000) >> 16;
    c[3] = (i & 0xff000000) >> 24;
    return c;
    //return (char*)"";
}

void NamePrinter(FileInfo file)
{
    if (!fast)
    {
        
        std::cout << "Extracting: " << file.FileName << std::endl;//CPU usage 41%
        //std::cout << formatted << "\n";//CPU usage 45%
        //printf("%s\n", formatted.data()); //41%
        

    }
}

void InfoPrinter(FileInfo file)
{
    std::string formatted = file.GetFormattedString();
    std::cout << formatted << std::endl;
}


bool CheckExtension(char* ext, std::string path)
{
    Debug("");
    int input_len = path.size();
    int ext_len = std::strlen(ext);
    std::string lastCharacters = path.substr(input_len - ext_len);
    if (input_len > ext_len && StringEquality(lastCharacters.data(), ext))
    {
        return true;
    }
    return false;
}

int windowbits = 0;


bool Contains(std::vector<std::string>& list, std::string filename, bool isRegex = false)
{
    Debug("");
    if (isRegex)
    {
        for (int i = 0; i < (int)list.size(); i++)
        {
            auto regex = std::regex(list[i], std::regex_constants::icase);
            
            if (std::regex_search(list[i], regex))
            {
                return true;
            }
        }
    }
    else
    {
        for (int i = 0; i < (int)list.size(); i++)
        {
            if (filename.find(list[i]))
            {
                return true;
            }
        }
    }
    return false;
}


#define NAME_SIZE_ATTRIBUTE 28

/// <summary>
/// write fileinfo to .package file
/// </summary>
/// <param name="files">File list to compress</param>
/// <param name="fileName">package's file name (includes .package)</param>
/// <param name="directory">directory of lys</param>
void WritePackage(std::vector<FileInfo*>& files, std::string fileName, std::string directory)
{
    std::ofstream out(directory + fileName, std::ios::trunc | std::ios::binary); //truncate if file exists
    Debug("");
    if (out.is_open())
    {
        //out.xalloc();
        out.write("PCK0\xf\x0\x0\x0\x0\x0", 10); //write file ID
        char* fs = IntToChar(files.size());
        out.write(fs, 4); //file count
        delete fs;
        //header size for each file (28 + name len) 
        int headersize = 19;//starts from 19 since id + file count + header size = 18 bytes +1
        for (int i = 0; i < (int)files.size(); i++)
        {
            auto name = files[i]->FileName;
            headersize += std::strlen(name) + NAME_SIZE_ATTRIBUTE;
        }
        char* hs = IntToChar(headersize);
        out.write(hs, 4);
        delete hs;

        out.write("\0", 1);

        //write header for each file
        for (int i = 0; i < files.size(); i++)
        {
            auto file = files[i];
            auto ns = IntToChar(std::strlen(file->FileName) + NAME_SIZE_ATTRIBUTE); //28 is header attributes size
            out.write(ns, 2);//name size of file 2 bytes
            delete ns;

            file->Offset = out.tellp(); //reserve offset position to use later
            out.write("\x0\x0\x0\x0", 4); //file offset (will be updated after compressed)

            out.write("\x0\x0\x0\x0", 4); //4 zeros

            char* fls = IntToChar(file->FileSize);
            out.write(fls, 4); //original file size
            delete fls;

            file->PackedSize = out.tellp(); //reserve compressed size position to use later
            out.write("\x0\x0\x0\x0", 4); //compressed file size
            //char *c = static_cast<char*>(file->TimeStamp);
            for (int i = 0; i < 9; i++)
            {
                int ts1 = file->TimeStamp[i];
                out.write(IntToChar(ts1), 1);
            }
            //out.write((char*)file->TimeStamp, 9); //timestamp

            out.write(file->FileName, std::strlen(file->FileName)); //write file name
            out.write("\0", 1);//extra 1 byte terminator
        }

        int latestPosition = out.tellp();
        for (int i = 0; i < files.size(); i++)
        {
            auto file = files[i];
            
            uLong outsize = compressBound(file->FileSize);
            BYTE* b = new BYTE[outsize];
            std::string fullpath = directory;
            fullpath.append(fileName);
            fullpath.append(".files");
            fullpath.append(pathReplacement);
            fullpath.append(file->FileName);
            if (!fast)
            {
                auto justFileName = fullpath.substr(fullpath.find_last_of(pathReplacement) + 1);
                std::cout << "Compressing file: " << justFileName << std::endl;
            }
            std::ifstream infile(fullpath, std::ios::binary);
            
            if (infile.is_open())
            {
                char* filebytes = (char*)malloc(file->FileSize);
                infile.read(filebytes, file->FileSize);
                infile.close();
                int result = compress(b, &outsize, (BYTE*)filebytes, file->FileSize);
                if (result == Z_OK)
                {
                    out.seekp(file->Offset);
                    char* lp = IntToChar(latestPosition);
                    out.write(lp, 4);
                    delete lp;
                    file->Offset = latestPosition;
                    latestPosition += outsize;
                    out.seekp(file->PackedSize);
                    char* os = IntToChar(outsize);
                    out.write(os, 4);
                    delete os;
                    file->PackedSize = outsize;
                    out.seekp(file->Offset);
                    out.write((char*)b, outsize);
                }
                free(filebytes);
                //infile.close();
            }
            //delete file;

            delete[] b;
           
        }

        out.close();
    }
}


typedef struct Dictionary
{
    std::string Key;
    std::string Value;
};
#define CHUNK 16384

void Extract()
{

    bool lys = CheckExtension((char*)".lys", input);
    IsPatchFile = CheckExtension((char*)".patch", input);

    if (lys)
    {
        //repack here
        Debug("lys file");
        
        std::map<std::string, std::string> m;//File info map from lys file
        std::ifstream lysFile(input);
        std::vector<std::string> lines;
        
        if (lysFile.is_open())
        {
            
            std::string key, val;
            while (std::getline(std::getline(lysFile, key, '=') >> std::ws, val))
                m[key] = val;

            lysFile.close();//close the lys file which no more required
            if (m["fn"].empty())//file name is required to continue
            {
                throw EXCEPTION_ARRAY_BOUNDS_EXCEEDED;
            }

            std::string path;
            std::string directory = GetDirectory((char*)input.data());//gets directory where lys file stays
            directory.append(pathReplacement);
            path = directory;
            //path.append("\\");
            path.append(m["fn"]);
            path.append(".files");
            path.append(pathReplacement);
            int pathLen = path.size();
            std::vector<FileInfo*> files;
            
            for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
            {
                if (!entry.is_directory())
                {
                    FileInfo* fi = new FileInfo;
                    auto p = entry.path();
                    //auto fn3 = p.filename().string();
                    auto p2 = p.string().substr(pathLen);
                    fi->FileName = new char[p2.size()];
                    std::strcpy(fi->FileName, p2.data());
                    fi->FileSize = entry.file_size();
                    if (!m[p2].empty())//get timestamp info from lys file if exists otherwise uses default timestamp in FileInfo class
                    {
                        std::string ts = m[p2];
                        std::istringstream f1 (ts);
                        unsigned int s;
                        std::vector<std::string> tsList;
                        int tsIterator = 0;
                        fi->TimeStamp = new char[9];
                        while (f1 >> std::hex >> s)
                        {
                            fi->TimeStamp[tsIterator] = s;
                            tsIterator++;
                        }
                        Debug("");
                    }
                    files.push_back(fi);
                    //delete fi;
                }
            }
            WritePackage(files, m["fn"], directory);
            //delete& files;


        }
    }
    else
    {
        Debug("Getting files");
        std::vector<FileInfo> files = GetFileList();
        Debug("File count: " + files.size());
        if (!files.empty())
        {
            if (!extract)
            {
                Debug("Printing names only");
                for (int i = 0; i < (int)files.size(); i++)
                {
                    FileInfo file = files[i];
                    InfoPrinter(file);
                }
            }
            else
            {

                Debug("Extraction starting");
#pragma region Create LYS file for repack
                std::string lysfile = output;

                if (!std::filesystem::exists(output))
                {
                    std::filesystem::create_directories(output);
                    createdDirectories.push_back(output);
                    Debug("Directory created: " + output);
                }
                //inputFile.rdbuf().
                lysfile.append("items.lys");
                std::ofstream lys(lysfile, std::ios::trunc | std::ios::out);
                Debug("lys file created");
                std::cout << GetDirectory((char*)input.data()) << std::endl;
                int plus = input.find(pathReplacement) == std::string::npos ? 0 : 1;
                std::string inputname = input.substr(GetDirectory((char*)input.data()).size() + plus);
                lys << "fn=" << inputname << std::endl;
                Debug("");
#pragma endregion

#pragma region Memory Allocation
                int highestSize = 1;// to prevent memory allocation and release, select highest size and create memory allocation once
                for (int i = 0; i < (int)files.size(); i++)
                {
                    FileInfo f = files[i];
                    if (f.PackedSize > highestSize)
                    {
                        highestSize = f.PackedSize;
                    }
                }
                char* buffer = (char*)malloc(highestSize);
                Debug("");

#pragma endregion
                

#pragma region stream decompress
                /*inputFile.close();
                gzFile infile = gzopen(input.data(), "rb");
                if (infile == NULL) {
                    printf("Error: Failed to gzopen %s\n", input.data());
                    exit(1);
                }*/
#pragma endregion

                
                for (int i = 0; i < (int)files.size(); i++)
                {



                    

                    FileInfo file = files[i];

#pragma region stream decompress


                    ////infile->pos = file.Offset;
                    //long seek_l = gzseek((gzFile_s*)infile, file.Offset, SEEK_SET);
                    //int buff = gzbuffer(infile, file.PackedSize);
                    ////std::cout << seek_l << std::endl;
                    ////CHUNK = file.FileSize;
                    ////infile->have = file.Offset;
                    ////std::ofstream f2("test.txt", std::ios::trunc | std::ios::binary);
                    //auto f2 = fopen("test.txt", "wb");
                    //unsigned int filePackedSize = file.PackedSize;
                    ////int result1 = uncompress((BYTE*)f2, &u, (BYTE*)infile, (ULONG)file.PackedSize);
                    //char* buffer = (char*)malloc(file.FileSize);
                    //int result = gzread(infile, buffer, filePackedSize);
                    //fwrite(&buffer, file.FileSize, 1, f2);
                    ////gzclose(f2);
                    //if (result != -1)
                    //{
                    //    printf("%s\n", "done");
                    //}
                    //else
                    //{
                    //    printf("failed");
                    //}
                    //free(buffer);
                    ////fclose(f2);
                    //return;
#pragma endregion




                    

                    std::string str = output;
                    str.append(file.FileName);

                    auto dir = GetDirectory((char*)str.data());
                    
                    
#pragma region Search and compare
                    if (!searchList.empty() || !regexList.empty())
                    {
                        bool matched = false;
                        if (!searchList.empty())
                        {
                            matched = Contains(searchList, file.FileName);
                        }
                        if (!matched && !regexList.empty())
                        {
                            matched = Contains(regexList, file.FileName, true);
                        }

                        if (!matched)
                        {
                            continue;
                        }
                    }
#pragma endregion

                    Debug((std::string)file.FileName + " is extracting");
                    inputFile.seekg(file.Offset);
                    std::istream& abc = inputFile.read(buffer, file.PackedSize);
                    
                    Debug("buffer readed");


                    Bytef* b = (BYTE*)buffer;

                    ULONG sizeDataUncompressed = file.FileSize;
                    BYTE* dataUncompressed = (BYTE*)malloc(sizeDataUncompressed);

                    Debug("uncompress starting");
                    ////uncompress2(dataUncompressed, &sizeDataUncompressed, b, (ULONG*)file.PackedSize);
                    int z_result = uncompress(

                        dataUncompressed,       // destination for the uncompressed
                                                // data.  This should be the size of
                                                // the original data, which you should
                                                // already know.

                        &sizeDataUncompressed,  // length of destination (uncompressed)
                                                // buffer
                        //inputFile,
                        b,   // source buffer - the compressed data

                        file.PackedSize);   // length of compressed data in bytes
                    Debug("uncompress done with code : " + z_result);
                    switch (z_result)
                    {
                    case Z_OK:
                    {
                        Debug("data uncompressed");
                        std::string str = output;
                        str.append(pathReplacement);
                        str.append(inputname);
                        str.append(".files");
                        str.append(pathReplacement);
                        str.append(file.FileName);
                        auto dir = GetDirectory((char*)str.data());
                        if (std::find(createdDirectories.begin(), createdDirectories.end(), dir) == createdDirectories.end())
                        {
                            bool directoryexists = std::filesystem::exists(dir);
                            if (!directoryexists)
                            {
                                try
                                {
                                    std::filesystem::create_directories(dir);
                                    Debug("directory created : " + dir);
                                }
                                catch (const std::exception& ex)
                                {
                                    printf("%s\n", ex.what());
                                }
                                
                            }
                            createdDirectories.push_back(dir);
                        }
                        std::ofstream outFile(str.data(), std::ios::binary | std::ios::out | std::ios::trunc);
                        if (outFile.is_open())
                        {
                            NamePrinter(file);
                            //write decompressed file to disk
                            outFile.write((char*)dataUncompressed, file.FileSize);
                            outFile.close();
                            Debug("File written to disk");

                            //write timestamp into lys
                            lys << file.FileName << "=" << std::hex << CharToHex(file.TimeStamp) << std::endl;
                            Debug("file info added into lys file");
                        }

                        break;
                    }
                    case Z_MEM_ERROR:
                        printf("out of memory\n");
                        exit(1);    // quit.
                        break;

                    case Z_BUF_ERROR:
                        printf("output buffer wasn't large enough!\n");
                        exit(1);    // quit.
                        break;
                    }
                    free(dataUncompressed);
                }
                //gzclose(infile);
                free(buffer);
                Debug("buffer freed");
                lys.close();
                Debug("lys closed");
                //extract here
			}
			files.clear();
		}
		else
		{
		    Debug("Files empty");
		}
    
    }
}








std::string CharToHex(char* c)
{
    char* str1 = new char;
    std::string str;
    for (int i = 0; i < 9; ++i) {
        const unsigned char ch = c[i];
        sprintf(str1, "%02X", ch);
        str.append(str1);
        //str.append(&hex[ch & 0xF], 1);
        str.append(" ");
    }
    return str;
}



void insertSearch(void* list, char* query)
{
    auto l = (std::vector<std::string>*)list;
    if (std::find(l->begin(), l->end(), query) == l->end())
    {
        l->push_back(query);
    }
}

void ParseArgs(int argc, char** args)
{
    if (args == nullptr || argc < 2)
    {
        helpRequest = true;
        return;
    }
    for (int i = 1; i < argc; i++)
    {
        char* arg = args[i];
        if (StringEquality("-i", arg) || StringEquality("--input", arg) && argc > i)
        {
            input = args[++i];
        }
        else if (StringEquality("-sl", arg) || StringEquality("--set-locale", arg) && argc > i)
        {
            char* locale = args[++i];
            std::setlocale(LC_ALL, locale);
        }
        else if (StringEquality("-s", arg) || StringEquality("--search", arg) && argc > i)
        {
            char* searchStr = args[++i];
            insertSearch(&searchList, searchStr);
        }
        else if (StringEquality("-r", arg) || StringEquality("--regex", arg) && argc > i)
        {
            char* searchStr = args[++i];
            insertSearch(&regexList, searchStr);
        }
        else if (StringEquality("-ll", arg) || StringEquality("--locale-list", arg))
        {
            localeList = true;
            return;
        }
        else if (StringEquality("-o", arg) || StringEquality("--output", arg) && argc > i)
        {
            extract = true;
            output = args[++i];
        }
        else if (StringEquality("-v", arg) || StringEquality("--version", arg))
        {
            version = true;
            return;
        }
        else if (StringEquality("-h", arg) || StringEquality("--help", arg))
        {
            helpRequest = true;
            return;
        }
        else if (StringEquality("-d", arg) || StringEquality("--debug", arg))
        {
            debug = true;
        }
        else if (StringEquality("-f", arg) || StringEquality("--fast", arg))
        {
            fast = true;
        }
        else if (StringEquality("-b", arg) || StringEquality("--benchmark", arg))
        {
            benchmark = true;
        }
    }
}


int Overflow()
{

    return 1;


}
