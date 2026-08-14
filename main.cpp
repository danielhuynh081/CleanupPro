// August 6th 2026 Daniel Huynh, this project is an app that displays folders using the most memory on your device and helps you delete it,
// it shows caches and other folders that arent always on surface level, using std::filesystem library
// functions: directory_iterator recursive_directory_iterator is_directory() file_size()
// https://en.cppreference.com/cpp/filesystem
// 1. learn how to iterate through directories and files using std::filesystem
// 2. make a function that takes a directory path and iterates through it, printing out the size of each file and folder
// 3. make the program run so a user can pick which folder they want to irate through
#include "class.h"

//Home directory
const fs::path homeDirectory{getenv("HOME")};

int main()
{
    int userInput{};

    //start at home directory
    fs::path currentDirectory = homeDirectory;

    while (true) {
        cout << "\n< -- Current Directory: " << currentDirectory << " -- >\n\n";
        vector<Entry> files = checkFolder(currentDirectory);

        displayFolder(files);

        cout << "\nChoose a file or folder by typing its number";
        cout << "\nType 0 to go back";
        cout << "\nType -1 to select multiple files to delete";
        cout << "\nType -2 to delete all files in this folder";
        cout << "\nType -3 to exit: ";

        cin >> userInput;

        //Return to parent folder
        if(userInput == 0){
            if(currentDirectory != homeDirectory){
                currentDirectory = currentDirectory.parent_path();
            }
            continue;
        }

        //Delete Multiple Files
        if(userInput == -1){
            cout << "Enter the numbers of the files you want to delete, separated by spaces (e.g., 1 3 5): ";
            cin.ignore();

            string line;
            getline(cin, line);

            istringstream iss(line);

            vector<int> selectedFiles;
            int num;

            while (iss >> num) {
                if (num >= 1 && num <= static_cast<int>(files.size())) {
                    selectedFiles.push_back(num);
                } else {
                    cout << "Invalid file number: " << num << endl;
                }
            }

            for (int index : selectedFiles) {
                fs::path selectedFile = files[index - 1].path;
                deleteFile(selectedFile);
            }

            continue;
        }

        //Delete All Files in Folder
        if(userInput == -2){
            if(currentDirectory != homeDirectory){
                deleteAll(currentDirectory);
            }
            continue;
        }

        //Exit program
        if(userInput == -3)
            break;

        //Error Handling
        if (userInput < 1 || userInput > static_cast<int>(files.size())){
            cout << "Invalid choice." << endl;
            continue;
        }

        //Get the selected file or folder from userInput
        Entry selection = files[userInput - 1];

        //if folder, go inside folder
        if(selection.isDirectory){
            cout << "Press 1 to go inside folder, press 2 to delete folder: ";
            cin >> userInput;

            if(userInput == 2){
                deleteAll(selection.path);
                fs::remove(selection.path);
                continue;
            }

            currentDirectory = selection.path;
        }

        //if file, ask user if they want to delete
        else{
            deleteFile(selection.path);
        }

        //fs::path path = fs::path(getenv("HOME")) / userInput;

        //printFolder(path.string());
    }

    return 0;
}