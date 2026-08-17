#include "class.h"


const vector<fs::path> protectedPaths = {
    homeDirectory / ".Trash",
    homeDirectory / "Library/Application Support",
    homeDirectory / "Library/Containers",
    homeDirectory / "Library/Group Containers",
    homeDirectory / "Library/CloudStorage",
    homeDirectory / "Library/Keychains",
    homeDirectory / "Library/Mail",
    homeDirectory / "Library/Messages",
    homeDirectory / "Library/Safari",
    homeDirectory / "Library/Preferences",
    homeDirectory / "Library/IdentityServices",
    homeDirectory / "Library/Accounts",
    homeDirectory / "Library/Calendars",
    homeDirectory / "Library/Contacts",
    homeDirectory / "Library/AddressBook",
    homeDirectory / "Library/Cookies",
    homeDirectory / "Library/HomeKit",
    homeDirectory / "Library/Passes",
    homeDirectory / "Library/PersonalizationPortrait",
    homeDirectory / "Library/Reminders",
    homeDirectory / "Library/VoiceTrigger",
    homeDirectory / "Library/Metadata",
    homeDirectory / "Library/Application Scripts"
};
const vector<fs::path> dontDelete {
    homeDirectory / "Library"
};
unordered_map<fs::path, vector<Entry>> folderCache;

string formatSize(uintmax_t bytes) {
    const double KB = 1024.0;
    const double MB = KB * 1024.0;
    const double GB = MB * 1024.0;

    if (bytes >= GB)
        return to_string(bytes / GB) + " GB";
    else if (bytes >= MB)
        return to_string(bytes / MB) + " MB";
    else
        return to_string(bytes / KB) + " KB";
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
uintmax_t getFolderSize(fs::path path){
 
    if (!fs::is_directory(path))
        return 0;

    uintmax_t size{0};

    try
    {   
        for (const auto& dir_entry : fs::directory_iterator(path))
        {
            if (shouldSkip(dir_entry.path()))
                continue;

            if (dir_entry.is_regular_file())
                size += dir_entry.file_size();

            else if (dir_entry.is_directory())
                size += getFolderSize(dir_entry.path());
        }
    }
    catch (const fs::filesystem_error&)
    {
        return size;
    }

    return size;
}

bool shouldSkip(const fs::path& path)
{
    for (const auto& p : protectedPaths)
    {
        if (path == p)
            return true;
    }

    return false;
}

bool shouldSkipDelete(const fs::path& path)
{
    for (const auto& p : dontDelete)
    {
        if (path == p)
            return true;
    }

    return false;
}

//Display folder contents as user selects folder
vector<Entry> checkFolder(const fs::path &path){
    // Cache first scan
    auto it = folderCache.find(path);
    if (it != folderCache.end())
        return it->second;
    
    //Create vector for list of files
    vector<Entry> files;

    //iterate through directory
    for(auto const & dir_entry : fs::directory_iterator{path}){

        // cout << dir_entry.path() << endl;

        //ignore trash
        if(shouldSkip(dir_entry.path()))
            continue;
        
        Entry entry;
        entry.path = dir_entry.path();
        entry.isDirectory = dir_entry.is_directory();
        if(dir_entry.is_regular_file()){
            entry.size = dir_entry.file_size();
        }
        else{
            entry.size= getFolderSize(dir_entry.path());
           // idx.size = getFolderSize(dir_entry.path());
        }
        files.push_back(entry);

    }
    //Save to cache
    folderCache[path] = files;

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

void displayFolder( vector<Entry> &files){
    
    int count = 1;
    sort(files.begin(), files.end(), [](const Entry& a, const Entry& b) {
        return a.size > b.size;
    });

    for(auto const & file : files){
        if (file.isDirectory)
            {
                cout << count
                    << ". Folder: "
                    << file.path.filename()
                    << " | "
                    << formatSize(file.size)
       //             << formatSize(file.size);
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
    char choice{};

    if (shouldSkip(currentDirectory) || shouldSkipDelete(currentDirectory)) {
        cout << "This folder is protected and cannot be deleted.\n";
        return;
    }   
    
    cout << "Are you sure you want to delete " << currentDirectory << "? (y/n): ";

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
        
        fs::remove(currentDirectory);
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
    if (shouldSkip(path) || shouldSkipDelete(path)) {
        cout << "This folder is protected and cannot be deleted.\n";
        return;
    }
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

