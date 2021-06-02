## Welcome to Age of wushu package extractor project

This program helps to extract and repack .package and patch files for age of wushu. You can modify game files to translate language and fix visual bugs in game. Keep in your mind, changing game files is forbidden and may end up with account block. Use it with your own risks.

### Usage

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Usage: AOWPackageExtractorCpp.exe [options]
Options:
	-i,--input:          input path to .package file to export or .lys file to repack.
			     use " (double quote) before and end of path if path contains space.
	-o,--output:         output directory to export package file.
	-l,--locale:         codepage for filenames. default codepage: windows-1254
	-ll,--locale-list:   prints codepage list supported by your system
	-s,--search:         searchs in file name and shows/extracts files which matched.
			     this, can be used multiple times
	-r,--regex:          searchs in file name with regular expression patterns and shows/extracts
			     files which matched. this, can be used multiple times
	-v,--version:        this program's version
	-h,--help:           shows this help message.

```
### CLI Parameters

```markdown


    - -i, --input: File full path to extract or repack. Supported extensions are .package, .patch and .lys for this program's repack extension. For valid path you should use " (double quotes) at beginning and at the and of path to avoid of reading wrong path. if path contains spaces you MUST to use "(double quotes).

    - -o, --output: Directory path for the extraction of files without filename just directory path. Valid path rules are as same as -i, -input for this.

    - -l, --locale: Determine a correct codepage for filenames. When you have Chinese characters inside the package, you should use a codepage that also supports Chinese characters. otherwise file names will be corrupted after extraction and you will have problem with repack them. Default value is "windows-1254" (Turkish) codepage for this parameter, it supports a lot of character sets.

    - -ll, --locale-list: Prints the list of codepages which your system are supported and you can use them.

    - -s, --search: Searchs the word in the file name. You can use that multiple times. Example: (-s word1 -s word2 -s "word 3").

    - -r, --regex: Searchs the word in the file name by using regular expression with joker characters. You can use that multiple times. Example: (-r word1 -r word2 -r "word 3"). More info about using regular expression patterns, please follow that link. https://en.wikipedia.org/wiki/Regular_expression

    - -v, --version: This program's current build version. You can check that and you can compare if there is new updates.

    - -h, --help: Prints short version of this.

```

### Usage hints



    1. You can use -i parameter to just print list of files in package. If no -o, -output parameter entered, it will just print files and will not extract them. But it is not same when repacking. repacking doesn't use -o parameter.

    2. When printing or extracting files, it will be formatted output with pattern in order "filename(tab)offset(tab)filesize(tab)packedsize". They are seperated by Tab

       * filename: relative path to file
       * offset: start offset of packed file in the package (informational)
       * filesize: File size after extract
       * packedsize: reduced size in package file


### Example usage

Open command prompt (cmd), if requires, run it as Administrator. Make sure cmd path is matches with Extractor.exe's directory
 ```markdown
 AOWPackageExtractorCpp -i "D:\Age of wushu\res\text.package" -o "D:\Extracts\"
 ```
 
 **Note: This application is using zlib library copied from [ZLib](https://github.com/madler/zlib)**
