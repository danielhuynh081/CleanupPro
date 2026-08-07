// August 6th 2026 Daniel Huynh, this project is an app that displays folders using the most memory on your device and helps you delete it, 
// it shows caches and other folders that arent always on surface level, using std::filesystem library
// functions: directory_iterator recursive_directory_iterator is_directory() file_size()
// https://en.cppreference.com/cpp/filesystem 
#include <iostream>
#include <filesystem>
#include <cstdlib>


using namespace std;

namespace fs = filesystem;

void printFolder(const fs::path dir_path){
    //ignore trash
    if(dir_path.filename() == ".Trash") return;
    
    cout <<"\n< -- Interating Through Folder: " << dir_path << " -- >\n\n";
  
    fs::directory_entry Directory{dir_path};
    for(auto const & dir_entry : fs::directory_iterator{Directory}){
        cout << dir_entry.path() << endl;
       
    }
}

void checkDir(){
    //Get path and set entry to directory
   // const fs::path path{R"(/Users/danielhuynh)"};
    const fs::path path{getenv("HOME")};

    fs::directory_entry Directory{path};

    //Check if it exists
    Directory.exists() ? cout <<"location exists\n" : cout <<"location does not exist\n";

    //Check if it is a directory
    Directory.is_directory() ? cout <<"directory is a directory\n" : cout <<"directory is not a directory\n";

    //Check if it is a file
    Directory.is_regular_file() ? cout <<"directory is a file\n" : cout <<"directory is not a file\n";

    //iterate through home directory
    for(auto const & dir_entry : fs::directory_iterator{Directory}){
       // cout << dir_entry.path() << endl;
        //print regular file size
        if(dir_entry.is_regular_file()){
            cout<< (dir_entry.file_size() / (1024 * 1024 * 1024)) << "gb\n";
        }else{
        //print directory
        printFolder(dir_entry.path());
        }
    }   
}

int main()
{
    checkDir();

    return 0;

}


