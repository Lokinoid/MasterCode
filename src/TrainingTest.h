//
//  TrainingTest.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 09/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__TrainingTest__
#define __ClassifierTrainer__TrainingTest__


#include <string>
#include "CommonTypes.h"


class ClassifierTrainer;

class TrainingTest
{
public:
  
  static void StartTest();
  
private:
  
  void StartTraining(const std::string& trainedDataFilePath);
  void StartTesting(const std::string& trainedDataFilePath);

  const LandmarkCollectionDataPtr synthesizeData();
  const ImageLandmarkDataPtr createRandomLandmarkData();
  
  std::string m_trainedDetectorDataFilePath;
};

#endif /* defined(__ClassifierTrainer__TrainingTest__) */
