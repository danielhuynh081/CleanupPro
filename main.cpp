// August 6th 2026 Daniel Huynh, this project is an app that displays folders using the most memory on your device and helps you delete it, 
// it shows caches and other folders that arent always on surface level, using std::filesystem library
// functions: directory_iterator recursive_directory_iterator is_directory() file_size()
// https://en.cppreference.com/cpp/filesystem 
// 1. learn how to iterate through directories and files using std::filesystem  
// 2. make a function that takes a directory path and iterates through it, printing out the size of each file and folder
// 3. make the program run so a user can pick which folder they want to irate through
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <iomanip>



using namespace std;

namespace fs = filesystem;
const fs::path homeDirectory{getenv("HOME")};


string formatSize(uintmax_t bytes) {
    const double KB = 1024.0;
    const double MB = KB * 1024.0;
    const double GB = MB * 1024.0;

    if (bytes >= GB)
        return to_string(bytes / GB) + " GB";
    else if (bytes >= MB)
        return to_string(bytes / MB) + " MB";
    else if (bytes >= KB)
        return to_string(bytes / KB) + " KB";
    else
        return to_string(bytes) + " bytes";
}
/*
void printFolder(const fs::path dir_path){
    //ignore trash
    if(dir_path.filename() == ".Trash") return;
    
    cout <<"\n< -- Interating Through Folder: " << dir_path << " -- >\n\n";
  
    fs::directory_entry Directory{dir_path};
    for(auto const & dir_entry : fs::directory_iterator{Directory}){
        cout << dir_entry.path() << endl;
       
    }
}
*/
void printFolder(const fs::path path){
 //   const fs::path path{dir_path}; 
    //ignore trash
    if(path.filename() == ".Trash" ) return;
    if (!fs::exists(path)) {
    cout << path << "Path does not exist.\nPress Enter to continue.\n";
    cin.get();
    return;
}
    
    cout <<"\n< -- Interating Through Folder: " << path << " -- >\n\n";
  

    for(auto const & dir_entry : fs::directory_iterator{path}){
         if (dir_entry.is_regular_file()) {
            cout << "File: "
                 << dir_entry.path().filename()
                 << " | "
                 << formatSize(dir_entry.file_size())
                 << '\n';
        }
        else if (dir_entry.is_directory()) {
            cout << "Folder: "
                 << dir_entry.path().filename()
                 << '\n';
        }
    }
}
void checkHome(){
    //Get path and set entry to directory
   // const fs::path path{R"(/Users/danielhuynh)"};
    fs::directory_entry Directory{homeDirectory};

    /*Check if it exists
    Directory.exists() ? cout <<"location exists\n" : cout <<"location does not exist\n";

    //Check if it is a directory
    Directory.is_directory() ? cout <<"directory is a directory\n" : cout <<"directory is not a directory\n";

    //Check if it is a file
    Directory.is_regular_file() ? cout <<"directory is a file\n" : cout <<"directory is not a file\n";
    */
    //iterate through home directory
    for(auto const & dir_entry : fs::directory_iterator{Directory}){
       // cout << dir_entry.path() << endl;
        //print regular file size
        if(dir_entry.is_regular_file()){
        cout<<  "File:" << dir_entry.path().filename() << endl;        
        }else
        //print directory
        //printFolder(dir_entry.path());
            cout<< "Folder:" << dir_entry.path().filename() << endl;
        }

}   

void deletefile(){
    string userInput;
    fs::path path{};

    printFolder(homeDirectory);
    cout << "\ntype filename to delete or folder to step into ";
    getline(cin >> ws, userInput);

    path = fs::path(getenv("HOME")) / userInput;

    fs::directory_entry Directory{path};
     if(Directory.is_regular_file()){
        cout<<  "File:" << path << endl;        
        }else{
        //print directory
        //printFolder(dir_entry.path());
            cout<< "Folder:" << path << endl;
        }






   
}


int main()
{
    string userInput;

    while (true) {
        cout << "< -- Home Directory: " << homeDirectory << " -- >\n\n";

        checkHome();

        cout << "\nDelete a file or folder by typing its name (or type 'exit' to quit): ";
        getline(cin >> ws, userInput);

        if (userInput == "exit")
            break;
        if (userInput == "delete")
            deletefile();

        //fs::path path = fs::path(getenv("HOME")) / userInput;

     //   printFolder(path.string());
        cout<<"Press Enter to continue."<<endl;
        cin.get();
    }

    return 0;
}


