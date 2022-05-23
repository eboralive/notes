

#include "fileOperations.h"
#include <iostream>
#include <fstream>
#include <string>
#include <pwd.h>



fileOperations::fileOperations()
{

}


fileOperations::~fileOperations()
{
}


std::string fileOperations::GetNoteFilesDir() {
   std::string home_dir;
   home_dir = getenv("HOME");
    return home_dir + "/.notes/text";
}

std::map<std::string, std::string> fileOperations::GetFileList() 
{
    std::map<std::string,std::string> paths;
    std::string path = GetNoteFilesDir();
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        std::string fName = entry.path();
        fName.erase(0, GetNoteFilesDir().length() + 1);
        paths[fName] = ReadFile(entry.path());
    }
    return paths;
}
  

std::string fileOperations::ReadFile(std::filesystem::path path)
{
    std::ifstream f(path, std::ios::in | std::ios::binary);
    const auto file_size = std::filesystem::file_size(path);
    std::string result(file_size, '\0');
    f.read(result.data(), file_size);
    return result;
}


void fileOperations::SaveFile(std::string fileName, std::string data)
{
    std::ofstream outfile;
    std::string path = GetNoteFilesDir() + "/" + fileName;
    outfile.open(path);
    outfile << data; 
}


void fileOperations::RenameFile(std::string fileName, std::string newFileName)
{
    std::rename((GetNoteFilesDir() + "/" + fileName).c_str(), (GetNoteFilesDir() + "/" + newFileName).c_str());
}

void fileOperations::RemoveFile(std::string fileName)
{
    const int result = std::remove((GetNoteFilesDir() + "/" + fileName).c_str());
}

