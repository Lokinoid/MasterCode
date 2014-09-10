//
//  FileDataPersister.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 12/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "FileDataPersister.h"
#include <boost/filesystem.hpp>
#include <fstream>
#include "LandmarkCollectionData.h"
#include "ImageLandmarkData.h"
#include "EquationData.h"
#include "FileFinder.h"
#include "FileLinesParser.h"


namespace fs = boost::filesystem;

void FileDataPersister::PersistData(const std::string& filePath,
                                    const LandmarkCollectionDataPtr& collection)
{
  std::ofstream file = createFile(filePath);
  
  if (!file.is_open())
  {
    std::cout<<"Cannot open file " << filePath<< std::endl;
    return;
  }
  
  collection->EnumerateConstColectionWithCallback([&] (const ImageLandmarkDataPtr& landmarkData, const int, bool*) {
    
    if (landmarkData)
    {
      const std::string line = createLineFormLandmarkData(landmarkData);
      file << line <<std::endl;
    }
  });
  
  file.close();
}

std::ofstream FileDataPersister::createFile(const std::string& filePath)
{
  fs::path path(filePath);
  if (fs::exists(path))
  {
    std::cout<<"file "<<filePath <<" exists. Rewriting..."<<std::endl;
  }
  
  return std::ofstream(filePath);
}

const std::string FileDataPersister::createLineFormLandmarkData(const ImageLandmarkDataPtr& landmarkData)
{
  std::string line(landmarkData->ImagePath());

  for (const PointType& point : *landmarkData->Landmarks())
  {
    line += " " + std::to_string(point.x) + " " + std::to_string(point.y);
  }

  return line;
}

void FileDataPersister::PersistData(const std::string& directoryPath,
                                    const LLSSolver::LLSEquationArrayPtr& equationArray)
{
  for (int i = 0; i < equationArray->size(); i++)
  {
    std::cout<<"Persisted "<<double (i)/ equationArray->size() <<std::endl;
    persistEquation(directoryPath, equationArray->at(i), i);
  }
}

void FileDataPersister::persistEquation(const std::string& directoryPath, const LLSSolver::LLSEquationPtr& equation, const int counter)
{
  const std::string prefix1 = "matX_";
  const std::string filePath1 = generatePath(directoryPath, prefix1, counter);
  
  const std::string prefix2 = "matY_";
  const std::string filePath2 = generatePath(directoryPath, prefix2, counter);
  
  writeMatToFile(equation->X, filePath1);
  writeMatToFile(equation->Y, filePath2);
}

void FileDataPersister::writeMatToFile(const cv::Mat& mat, const std::string& filePath)
{
  BOOST_ASSERT(mat.depth() == CV_64FC1);
  
  std::ofstream file(filePath);
  
  std::string strToWrite;
  
  for (int i = 0; i < mat.rows; i++)
  {
    for (int j = 0; j < mat.cols; j++)
    {
      strToWrite += std::to_string(mat.at<double>(i, j)) + " ";
    }
    strToWrite += "\n";
  }
  
  file << strToWrite;
}

const std::string FileDataPersister::generatePath(const std::string& directoryPath,
                                                  const std::string& prefix,
                                                  const int counter)
{
  namespace fs = boost::filesystem;
  
  fs::path path(directoryPath);
  if (!fs::exists(path))
  {
    std::cout<<"Directory "<<directoryPath <<"does not exist. Assert..."<<std::endl;

    BOOST_ASSERT(0);
  }
  
  path /= prefix + std::to_string(counter) + ".txt";
  return path.string();
}

void FileDataPersister::LoadData(const std::string& directoryPath,
                                 cv::Mat& matrix)
{
  CFileFinder finder;
  finder.SearchFiles(directoryPath, true);
  
  CFileLinesParser::ParseFilesToMatrixData(*finder.GetFileNames(), matrix);
}