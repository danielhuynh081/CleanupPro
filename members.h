
#include <filesystem>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <sstream>

using namespace std;
namespace fs = filesystem;

//Formatting
string formatSize(uintmax_t bytes);
uintmax_t getFolderSize(fs::path path);

//Displaying
void printFolder(const fs::path& path);
vector<fs::path> checkFolder(const fs::path& path);

//Deleting
void deleteAll(fs::path& currentDirectory);
void deleteFile(const fs::path& path);