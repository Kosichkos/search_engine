# __Search Engine__

## This project was created using the following technologies
- C++17
- CMake 3.27
- Nlohmann JSON Library
- GoogleTest
- MinGW w64 13.2.0
  ___Important: the program in its current form is intended to work only on Windows. To work correctly on Linux, you need to slightly modify the paths to the files with which the program interacts.___

## Principle of operation and use
* Before starting the application, you must specify the paths to the text files that the program will search. This must be done in the config.json file as shown in the example below:
```
{
  "config": {
    "name": "search_engine",
    "version": "0.1",
    "max_responses": 5
  },
  "files": [
    ".\\resources\\file001.txt",
    ".\\resources\\file002.txt",
    ".\\resources\\file003.txt",
    ".\\resources\\file004.txt",
    ".\\resources\\file005.txt",
    ".\\resources\\file006.txt"
  ]
}
```
Also, you need to write a query text according to which the comparison will be made (requests.json):
```
{
  "requests": [
    "my favorite programming language is С++"
  ]
}
```
* After launching the application, the "ConverterJSON" class object will look for the "config.json" and "requests.json" files and try to serialize them. If "config.json" or "requests.json" is missing, "config.json" is empty, or the program version in "config.json" does not match the actual program version, the engine will throw an appropriate exception. Otherwise, it will receive the necessary data in the object's private field, such as paths to text files and query strings.
* Immediately after this, the "InvertedIndex" class object will parse the .txt files (from the paths that the "ConverterJSON" object received from the "config.json" file) for unique words and count the number of entries of each word in each file.
* As a result, the "SearchServer" object will calculate the relative relevance for each query in each file, and the results of the calculation will be placed in the "answers.json" file.

## Building using the Windows command line:
* Create folder for building app and go to it:
```
mkdir build
cd build
```
* Input command:
```
cmake .. -G "MinGW Makefiles"
cmake --build
```
* Go to source directory of build and run SearchEngine
```
cd src
start SearchEngine.exe
```
