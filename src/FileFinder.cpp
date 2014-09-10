#include "FileFinder.h"

#include <algorithm>
#include <exception>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/path_traits.hpp>

namespace fs = boost::filesystem;

void CFileFinder::SearchFiles(const std::string& i_directoryToSerachIn,
                              const bool i_cleanPreviousResults,
                              const std::string i_extension)
{
  m_extension = i_extension;
  
  if (!m_fileNamesContainer)
  {
    m_fileNamesContainer = TFileNamesContainerPtr(new TFileNamesContainer);
  }
  
  if ( i_cleanPreviousResults )
  {
    m_fileNamesContainer->clear();
  }
  
  const fs::path directoryToSearchIn( i_directoryToSerachIn );
  
  if ( !fs::exists( directoryToSearchIn ) )
  {
    throw std::runtime_error( "Error: " + i_directoryToSerachIn + "doesn't exist" );
  }
  
  checkDirectory(i_directoryToSerachIn);
  
}

//////////////////////////////////
const CFileFinder::TFileNamesContainerPtr CFileFinder::GetFileNames() const
{
  return m_fileNamesContainer;
}

void CFileFinder::checkDirectory(const std::string& directory)
{
  const fs::path directoryToSearchIn(directory);
  
  const fs::directory_iterator end;
  for ( fs::directory_iterator iter(directoryToSearchIn); iter != end; ++iter )
  {
    fs::path nextPath = iter->path();
    if (fs::is_directory(nextPath))
    {
      checkDirectory(nextPath.string());
    }
    else
    {
      const std::string filename = nextPath.string();
      if ( ( filename.length() > 4 ) && ( filename.substr( filename.length() - 4 ) == m_extension ) )
      {
        m_fileNamesContainer->push_back( filename );
      }
    }
  }
  
}
