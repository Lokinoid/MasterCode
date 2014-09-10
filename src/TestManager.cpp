//
//  TestManager.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 01/05/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "TestManager.h"
#include "ClassifierTester.h"
#include "ImagePicker.h"
#include "LandmarkCollectionData.h"
#include "ClassifierTester.h"


void TestManager::StartTests(const std::string& trainedDataFilePath)
{
  std::string dir = "/Users/stanislav/Documents/Images/Faces/";
  
  std::vector<std::pair<std::string, std::string>> directories = {
    {dir + "Aligned", dir + "Aligned/TrainFaceCollectionData.txt"},
//    {dir + "Aligned", dir + "Aligned/TestFaceCollectionData.txt"},
//    {dir + "AlignedTest/",dir + "AlignedTest/TestLandmarkData.txt"},
//    {dir + "AlignedTest2/", dir + "AlignedTest2/TestLandmarkData.txt"},
    {dir + "AlignedTest5/", dir + "AlignedTest5/TestLandmarkData.txt"}
  };
  
  TestManager manager(trainedDataFilePath);
  manager.Start(directories);
}

TestManager::TestManager(const std::string& trainedDataFilePath)
: m_detectorDataFilePath(trainedDataFilePath)
{
}

void TestManager::Start(std::vector<std::pair<std::string, std::string>>& directories)
{

  for (std::vector<std::pair<std::string, std::string>>::iterator it = directories.begin(); it != directories.end(); it++)
  {
    std::cout<<"\n\nRead: "<<it->second<<std::endl;
    
    ImagePicker picker(it->first);
    LandmarkCollectionDataPtr data = picker.FetchLandmarkCollectionDataFromFile(it->second);
    std::cout << "Number of test images: " << data->CollectionSize() << std::endl<< std::endl;
    
    ClassifierTester tester;
    for (int i = 1; i < 5; i++)
    {
      double result = tester.StartTest(data, m_detectorDataFilePath, i, false);
      std::cout << "Iteration " << i << " result: " << result << std::endl << std::endl;
    }
  }
}