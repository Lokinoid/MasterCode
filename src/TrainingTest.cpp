//
//  TrainingTest.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 09/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "TrainingTest.h"

#include <stdlib.h>
#include "ClassifierTrainer.h"
#include "LandmarkCollectionData.h"
#include "TestFunctionLandmarkData.h"
#include "TestFunctionFeature.h"

void TrainingTest::StartTest()
{
  const std::string trainedDataFilePath("/Users/stanislav/Dropbox/Master_Thesis/Code/TestFunctionTrainedData.xml");
  
  TrainingTest tester;
  tester.StartTraining(trainedDataFilePath);
  
  tester.StartTesting(trainedDataFilePath);
}

void TrainingTest::StartTraining(const std::string& trainedDataFilePath)
{
  FeaturePtr feature(new TestFunctionFeature);
  
  LandmarkCollectionDataPtr data = synthesizeData();
  ClassifierTrainer trainer;
  const int kNumberOfIterations = 4;
  std::cout<<"Start training test function"<<std::endl;
  trainer.TrainDetector(data, feature, NULL, kNumberOfIterations, trainedDataFilePath, true);
  std::cout<<"Finished training test function"<<std::endl;
}

void TrainingTest::StartTesting(const std::string &trainedDataFilePath)
{
  
}


const LandmarkCollectionDataPtr TrainingTest::synthesizeData()
{
  LandmarkCollectionDataPtr collection(new LandmarkCollectionData());
  
  const int kNumberOfAssets = 1000;
  for (int i = 0; i < kNumberOfAssets; i++)
  {
    ImageLandmarkDataPtr landmarkData = createRandomLandmarkData();
    collection->AddImageLandmarkData(landmarkData);
  }

  return collection;
}

const ImageLandmarkDataPtr TrainingTest::createRandomLandmarkData()
{
  ImageLandmarkDataPtr landmarkData(new TestFunctionLandmarkData);
  landmarkData->PrintLandmarkData();
  
  return landmarkData;
}
