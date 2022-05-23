

#ifndef FILE_OPERATIONS
#define FILE_OPERATIONS


#include <vector>
#include <string>
#include <filesystem>
#include <map>


class fileOperations 
{

public:
  fileOperations();
  virtual ~fileOperations();
  std::map<std::string, std::string>  GetFileList();
  void SaveFile(std::string fileName, std::string data);
  std::string ReadFile(std::filesystem::path path);
  std::string GetNoteFilesDir();
  void RenameFile(std::string fileName, std::string newFileName);
  void RemoveFile(std::string fileName);

protected:
  

  
  
};

#endif



