/// @file FileLinesGetter.cpp
/// @brief CFileLinesGetter class definitions file
/// @author Daria Balabanova
/// @date Created on: 06-Sep-2013

#include "FileLinesGetter.h"

#include <sstream>
#include <iostream>

#include "PlatformDependingConstants.h"


//////////////////////////////////////////////////////////////////////////
CFileLinesGetter::CFileLinesGetter( const std::vector< std::string >& i_filesNames ):
   m_filesNames( i_filesNames ),
   m_currentFileNumber( -1 ),
   m_currentLineNumber( 0 )
{
   m_currentFileContent.exceptions( std::ifstream::failbit | std::ifstream::badbit );
   closeCurrentFileOpenNext();
}

//////////////////////////////////////////////////////////////////////////
bool CFileLinesGetter::GetNextLine( std::string& o_string )
{
   o_string.clear();
   try
   {
      while ( !isNextLineAvailableInCurrentFile() )
      {
         if ( !closeCurrentFileOpenNext() )
         {
            return false;
         }
      }

      ++m_currentLineNumber;
      std::getline( m_currentFileContent, o_string, g_Delimiter );
   }
   catch ( std::ifstream::failure& )
   {
      std::ostringstream errorMessage;
      errorMessage << "error while reading from file\n";
      errorMessage << GetCurrentFileName();
      errorMessage << "\nline number";
      errorMessage << GetCurrentLineNumber();
      throw std::runtime_error( errorMessage.str() );
   }
   return true;
}

//////////////////////////////////////////////////////////////////////////
bool CFileLinesGetter::isNextLineAvailableInCurrentFile()
{
   return m_currentFileContent && !m_currentFileContent.eof() && ( m_currentFileContent.peek() != EOF );
}

//////////////////////////////////////////////////////////////////////////
bool CFileLinesGetter::closeCurrentFileOpenNext()
{
   if ( m_currentFileContent.is_open() )
   {
      m_currentFileContent.close();
   }

   if ( m_currentFileNumber >= static_cast< int >( m_filesNames.size() ) - 1 )
   {
      return false;
   }

   ++m_currentFileNumber;
   m_currentLineNumber = 0;

   try
   {
     m_currentFileContent.open( m_filesNames.at( m_currentFileNumber ) );
   }
   catch (std::ifstream::failure exception)
   {
     std::cout<<"File does not exist "<<m_filesNames.at( m_currentFileNumber )<<std::endl;
     return false;
   }
  
   if (!m_currentFileContent.is_open())
   {
     std::cout<<"Cannot open file "<<m_filesNames.at( m_currentFileNumber )<<std::endl;
     return false;
   }
  
   return true;
}

//////////////////////////////////////////////////////////////////////////
const std::string CFileLinesGetter::GetCurrentFileName() const
{
   return ( m_currentFileNumber > -1 ) ? m_filesNames.at( m_currentFileNumber ) : std::string();
}

//////////////////////////////////////////////////////////////////////////
unsigned int CFileLinesGetter::GetCurrentLineNumber() const
{
   return m_currentLineNumber;
}