# CleanupPro

CleanupPro is a personal C++ project I built to learn how to use std::filesystem, refresh myself on multithreadding, and to create something I'd actually use. The goal is to make it easier to find what's taking up space on your computer by displaying commonly missed directories such as `~/Library/Caches` on MacOS, as well as displaying folders taking the most memory. I plan on trying speeding up the deletion process and program speed by using multithreadding.

## Current Features

- Browse directories from the terminal
- View files and folders in the current directory
- Display file sizes in a readable format (KB, MB, GB)
- Navigate through folders
- Delete individual files
- Delete everything inside a folder with confirmation prompts
- Ignore system folders such as `.Trash`
- Sort folders by largest size
- Calculate total folder sizes recursively
- Cached folders after scans to prevent double scanning
- multithreadded using #include<thread> bringing initial scan time from 22 seconds to 6 seconds

## Built With

- C++17
- `std::filesystem`
- Standard Template Library (STL)

## What I Learned

Building CleanupPro has helped me become more comfortable with:

- Working with the C++ filesystem library
- Directory traversal and path manipulation
- Organizing larger projects across multiple source files
- Error handling and input validation
- Using STL containers like `vector`
- Designing a command-line application from scratch

## Planned Features

This project is still a work in progress. Some features I'd like to add include:

- Multithreadding
- A GUI interface
- Move deleted files to the system Trash instead of permanently deleting them
- Find the largest files on the system
- Search for files over a user-defined size

## Running the Project

Clone the repository:

```bash
git clone https://github.com/danielhuynh081/CleanupPro.git
cd CleanupPro
```

Compile:

```bash
g++ -std=c++17 main.cpp members.cpp -o CleanupPro
```

or use the makefile by typing

```bash
make
```

Run:

```bash
./main
```
