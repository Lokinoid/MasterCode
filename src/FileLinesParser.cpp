#include "FileLinesParser.h"

#include <string>
#include <fstream>
#include <list>
#include <map>
#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <boost/tokenizer.hpp>
#include "FileLinesGetter.h"
#include <iostream>


typedef boost::char_separator<char>                       separatorType;
typedef boost::tokenizer< boost::char_separator<char> >   tokenizerType;


int numberOfRowFromFileName(const std::string& filePath)
{
  boost::filesystem::path path(filePath);
  std::string fileName = path.filename().string();
  int number = std::stoi(fileName.substr(5)); // matB_
  
  return number;
}

void CFileLinesParser::ParseFilesToContainerData( const std::vector<std::string>& i_fileNames,
                                                  TDataContainer& o_dataContainer)
{
   CFileLinesGetter linesGetter( i_fileNames );
   const separatorType separators(" \t\n");

   std::string line;
   std::string imagefileName;
   std::list< TCoordinates > coordinates;
   
   unsigned int currentLineCoordinatesQuantity = 0;
   unsigned int coordinatesQuantity = 0;
   bool firstIteration = true;

   while ( linesGetter.GetNextLine( line ) )
   {
      currentLineCoordinatesQuantity = 0;
      
      if ( line.empty() )
      {
         continue;
      }
      coordinates.clear();
      const tokenizerType tokens(line, separators);
      tokenizerType::iterator beg = tokens.begin();
     
      const boost::filesystem::path fullPath = boost::filesystem::path( beg->c_str() );
      imagefileName = fullPath.filename().string();

      TCoordinates currentCoordinate;
      for( ++beg; beg != tokens.end(); ++beg )
      {
         std::string str( beg->c_str() );
         const int coordinate = std::stoi( str );
         ++currentLineCoordinatesQuantity;
         if ( currentLineCoordinatesQuantity % 2 == 1 )
         {
            currentCoordinate.first = coordinate;
         }
         else
         {
            currentCoordinate.second = coordinate;
            coordinates.push_back( currentCoordinate );
         }
      }

      if ( firstIteration )
      {
         coordinatesQuantity = currentLineCoordinatesQuantity;
         firstIteration = false;
      }

      BOOST_ASSERT( currentLineCoordinatesQuantity % 2 == 0 );

      BOOST_ASSERT( currentLineCoordinatesQuantity == coordinatesQuantity );

      const std::pair<TDataContainer::iterator, bool> insertingResult =
         o_dataContainer.emplace( imagefileName, coordinates );
      
      // we can't insert element: there is an existing element with such key
      BOOST_ASSERT( insertingResult.second == true);
   }
}

void CFileLinesParser::ParseFilesToMatrixData( const std::vector<std::string>& i_fileNames,
                                              cv::Mat& o_mat)
{
  CV_Assert(i_fileNames.size() == 98);
  CFileLinesGetter linesGetter( i_fileNames );
  const separatorType separators(" \t\n");
  
  std::string line;
  std::string imagefileName;

  const int kNumberOfRows =static_cast<int>(i_fileNames.size()); // number of rows in matrix = nuber of files
  
// Descriptor dependencies: 49 (points) * descriptor_length + 1
// lenght: SIFT 128
//         HOG_16x8x9 9 (bins) * window.height / cell.height * window.width / cell.width = 9 * 4
//         HOG_24x8x9 9 * 9
//         HOG_24x8x6 6 * 9
//         HOG_24x6x6 12 * 9
//         HOG_36x8x9 8 * 16
  const int kNumberOfColumns = 49 * 128 + 1;
//  const int kNumberOfColumns = 5;
  cv::Mat_<double> resultMat(kNumberOfRows, kNumberOfColumns, CV_64FC1);
  
  unsigned int currentLineCoordinatesQuantity = 0;
  
  while ( linesGetter.GetNextLine( line ) )
  {
    int rowNumber = numberOfRowFromFileName(linesGetter.GetCurrentFileName());
    int lineNumber = linesGetter.GetCurrentLineNumber() - 1;
//    
//    currentLineCoordinatesQuantity = 0;
    
    if ( line.empty() )
    {
      continue;
    }

    const tokenizerType tokens(line, separators);
    
    for(tokenizerType::iterator beg = tokens.begin(); beg != tokens.end(); ++beg )
    {
      std::string str( beg->c_str() );
      const double matValue = std::stod( str );
      ++currentLineCoordinatesQuantity;
//      std::cout<<matValue<<std::endl;
      
      resultMat(rowNumber, lineNumber) = matValue;
    }
    
//    BOOST_ASSERT( currentLineCoordinatesQuantity <= 1 );
  }
  std::cout<<currentLineCoordinatesQuantity<<std::endl;
  o_mat = resultMat;
}

