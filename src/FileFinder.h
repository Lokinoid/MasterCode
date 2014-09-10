#pragma once

#include <string>
#include <vector>
#include <memory>


class CFileFinder
{
public:
  
  typedef std::vector< std::string > TFileNamesContainer;
  typedef std::shared_ptr<TFileNamesContainer> TFileNamesContainerPtr;
  
  
  void SearchFiles(const std::string& i_directoryToSerachIn,
                   const bool i_cleanPreviousResults,
                   const std::string i_extension = ".txt");
  
  const TFileNamesContainerPtr GetFileNames() const;
  
private:
  
  TFileNamesContainerPtr m_fileNamesContainer;
  std::string m_extension;
  
  void checkDirectory(const std::string& directory);
};