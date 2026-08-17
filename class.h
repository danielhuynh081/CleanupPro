
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <sstream>

using namespace std;
namespace fs = filesystem;
const fs::path homeDirectory{getenv("HOME")};

//Struct for each file/folder entry
struct Entry {
    fs::path path;
    uintmax_t size;
    bool isDirectory;
};

//Formatting
string formatSize(uintmax_t bytes);
uintmax_t getFolderSize(fs::path path);


//Displaying
void printFolder(const fs::path& path);
vector<Entry> checkFolder(const fs::path& path);
void displayFolder( vector<Entry> &files);
bool shouldSkip(const fs::path& path);
bool shouldSkipDelete(const fs::path& path);


//Deleting
void deleteAll(fs::path& currentDirectory);
void deleteFile(const fs::path& path);