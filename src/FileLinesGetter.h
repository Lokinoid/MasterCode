/// @file FileLinesGetter.h
/// @brief CFileLinesGetter class declarations file
/// @author Daria Balabanova
/// @date Created on: 06-Sep-2013

#pragma once

#include <vector>
#include <fstream>

/// @class CFileLinesGetter
/// @brief Provides reading lines from given files
/// @collab Used in COrderXmlReader and COpReader ( composition )
class CFileLinesGetter
{
public:
   /// @brief Constructor with parameters
   /// @param i_filesNames in, files, from which lines shall be read
   /// @throws std::exception in case of error while opening or reading from files occurred
   explicit CFileLinesGetter( const std::vector< std::string >& i_filesNames );

   /// @see ILinesGetter::GetNextLine()
   bool GetNextLine( std::string& o_string );

   /// @see ILinesGetter::GetCurrentFileName()
   const std::string GetCurrentFileName() const;

   /// @see ILinesGetter::GetCurrentLineNumber()
   unsigned int GetCurrentLineNumber() const;

private:
   /// @brief Opens next file in m_filesNames, if current file is not the last one; closes current file, if it's open
   /// @return true, if opening file succeed; false - otherwise
   /// @throws std::exception in case of error while opening files occurred
   bool closeCurrentFileOpenNext();

   // @returns True, if next line can be obtained from current file; false - otherwise
   bool isNextLineAvailableInCurrentFile();

   std::vector< std::string > m_filesNames;  // < Files, which shall be read

   int m_currentFileNumber;   // < Number of current file among all files from m_filesNames ( -1 before the start )

   unsigned int m_currentLineNumber; // < Number of read line in current file ( starting from 1 )

   std::ifstream m_currentFileContent; // < Stream with current file content
};