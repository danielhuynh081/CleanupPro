#include "class.h"



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



//Display folder contents as user selects folder
vector<Entry> checkFolder(const fs::path &path){
    //Create vector for list of files
    vector<Entry> files;

    //iterate through directory
    for(auto const & dir_entry : fs::directory_iterator{path}){

        // cout << dir_entry.path() << endl;

        //ignore trash
        if(dir_entry.path().filename() == ".Trash")
            continue;
        
        Entry entry;
        entry.path = dir_entry.path();
        entry.isDirectory = dir_entry.is_directory();
        if(dir_entry.is_regular_file()){

        
            entry.size = dir_entry.file_size();
        }
        else{
            entry.size=0;
           // idx.size = getFolderSize(dir_entry.path());
    }
        files.push_back(entry);

    }

    //Return vector of file and folder paths
    return files;
}

//Display Folders

/*
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
*/
void displayFolder(const vector<Entry> &files){
    
    int count = 1;

    for(auto const & file : files){
        if (file.isDirectory)
            {
                cout << count
                    << ". Folder: "
                    << file.path.filename()
                    << '\n';
            }
        else
        {
            cout << count
                << ". File: "
                << file.path.filename()
                << " | "
                << formatSize(file.size)
                << '\n';
        }
    count++;    
    }
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

//Delete multiple files
/*void selectDelete(const fs::path& path, vector<int> files){

    for(int i = 0; i < files.size(); i++){
        //fs::path selectedFile = files[userInput - 1]; loop through file number array and delete paths to each one
    }
}
*/
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

