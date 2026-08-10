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
#include <vector>

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
    // const fs::path path{dir_path};

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


vector<fs::path> checkFolder(const fs::path path){

    //Get path and set entry to directory
    // const fs::path path{R"(/Users/danielhuynh)"};

    fs::directory_entry Directory{path};

    /*
    Check if it exists
    Directory.exists() ? cout <<"location exists\n" : cout <<"location does not exist\n";

    //Check if it is a directory
    Directory.is_directory() ? cout <<"directory is a directory\n" : cout <<"directory is not a directory\n";

    //Check if it is a file
    Directory.is_regular_file() ? cout <<"directory is a file\n" : cout <<"directory is not a file\n";
    */

    vector<fs::path> files;

    int count = 1;

    //iterate through directory
    for(auto const & dir_entry : fs::directory_iterator{Directory}){

        // cout << dir_entry.path() << endl;

        //ignore trash
        if(dir_entry.path().filename() == ".Trash")
            continue;

        files.push_back(dir_entry.path());

        //print regular file size
        if(dir_entry.is_regular_file()){

            cout << count
                 << ". File:"
                 << dir_entry.path().filename()
                 << " | "
                 << formatSize(dir_entry.file_size())
                 << endl;

        }else{

            //print directory
            //printFolder(dir_entry.path());

            cout << count
                 << ". Folder:"
                 << dir_entry.path().filename()
                 << endl;
        }

        count++;
    }

    return files;
}


void deleteFile(const fs::path& path) {

    // Safety checks
    if (!fs::exists(path)) {
        cout << "File does not exist.\n";
        return;
    }

    //dont delete folders
    if(fs::is_directory(path)){
        cout << "Cannot delete folders yet.\n";
        return;
    }

    if (path.filename() == ".Trash") {
        cout << "Cannot delete .Trash.\n";
        return;
    }

    cout << "Are you sure you want to delete "
         << path.filename()
         << "? (y/n): ";

    char choice;
    cin >> choice;
    cin.ignore();

    if (choice != 'y' && choice != 'Y') {
        cout << "Deletion cancelled.\n";
        return;
    }

    try {

        fs::remove(path);

        cout << "File deleted.\n";

    }

    catch (const fs::filesystem_error& e) {

        cout << "Could not delete: "
             << e.what()
             << '\n';
    }
}


int main()
{
    int userInput;

    //start at home directory
    fs::path currentDirectory = homeDirectory;

    while (true) {

        cout << "\n< -- Current Directory: "
             << currentDirectory
             << " -- >\n\n";


        vector<fs::path> files = checkFolder(currentDirectory);


        cout << "\nChoose a file or folder by typing its number";
        cout << "\nType 0 to go back";
        cout << "\nType -1 to exit: ";

        cin >> userInput;


        if(userInput == -1)
            break;


        //go back one folder
        if(userInput == 0){

            if(currentDirectory != homeDirectory){
                currentDirectory = currentDirectory.parent_path();
            }

            continue;
        }


        if(userInput < 1 || userInput > files.size()){

            cout << "Invalid choice." << endl;

            continue;
        }


        fs::path selectedFile = files[userInput - 1];


        //if folder, go inside folder
        if(fs::is_directory(selectedFile)){

            currentDirectory = selectedFile;

        }

        //if file, ask user if they want to delete
        else{

            deleteFile(selectedFile);

        }


        //fs::path path = fs::path(getenv("HOME")) / userInput;

        //   printFolder(path.string());

    }

    return 0;
}

