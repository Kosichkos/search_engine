# Search Engine

This project was created using the following technologies
- C++17
- CMake 3.27
- Nlohmann JSON Library
- GoogleTest
- MinGW w64 13.2.0

## Principle of operation and use
* Before starting the application, you must specify the paths to the text files that the program will search. This must be done in the config.json file as shown in the example below:
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
Also, you need to write a query text according to which the comparison will be made (requests.json):
{
  "requests": [
    "my favorite programming language is С++"
  ]
}
