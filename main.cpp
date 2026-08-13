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
#include <sstream>

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

//Delete folder
void deleteAll(fs::path &currentDirectory){
    cout << "Are you sure you want to delete "
         << currentDirectory
         << "? (y/n): ";

    char choice;
    cin >> choice;
    cin.ignore();

    if (choice != 'y' && choice != 'Y') {
        cout << "Deletion cancelled.\n";
        return;
    }

    try {
        for(auto const & dir_entry : fs::directory_iterator{currentDirectory}){
            
                fs::remove_all(dir_entry.path());
            
        }
                cout << "Folder has been deleted.\n";
                cin.get();
                cout << "Press Enter to continue.\n";

    }catch(const fs::filesystem_error& e) {

        cout << "Could not delete: "
             << e.what()
             << '\n';
    }
}

void selectDelete(const fs::path& path, vector<int> files){

    for(int i = 0; i < files.size(); i++){
        //fs::path selectedFile = files[userInput - 1]; loop through file number array and delete paths to each one
    }
}

//Delete one file
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
        cout << "\nType -1 to select multiple files to delete";
        cout << "\nType -2 to delete all files in this folder";
        cout << "\nType -3 to exit: ";

        cin >> userInput;


        if(userInput == -3)
            break;


        //go back one folder
        if(userInput == 0){

            if(currentDirectory != homeDirectory){
                currentDirectory = currentDirectory.parent_path();
            }

            continue;
        }
        if(userInput ==-2){

            if(currentDirectory != homeDirectory){
                deleteAll(currentDirectory);

            }
        }
        if(userInput == -1){
            
            cout << "Enter the numbers of the files you want to delete, separated by spaces (e.g., 1 3 5): ";
            cin.ignore(); // Clear the newline character from the input buffer
            string line;
            getline(cin, line);
            istringstream iss(line);
            vector<int> selectedFiles;
            int num;
            while (iss >> num) {
                if (num >= 1 && num <= files.size()) {
                    selectedFiles.push_back(num);
                } else {
                    cout << "Invalid file number: " << num << endl;
                }
            }

            for (int index : selectedFiles) {
                fs::path selectedFile = files[index - 1];
                deleteFile(selectedFile);
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
            cout <<"press 1 to go inside folder, press 2 to delete folder: ";
            cin >> userInput;
            if(userInput == 2){
                deleteAll(selectedFile);
                fs::remove(selectedFile);
                continue;
            }
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

