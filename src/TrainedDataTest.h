//
//  TrainedDataTest.h
//  ClassifierTrainer
//
//  Created by Stanislav on 21.04.14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__TrainedDataTest__
#define __ClassifierTrainer__TrainedDataTest__

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <memory>


class TrainedData;


class TrainedDataTest
{
public:
  
  static void Test();
  
private:
  
  typedef std::vector<cv::Mat> TestDataType;
  typedef std::shared_ptr<TestDataType> TestDataPtr;

  TrainedDataTest();
  void startTest();

  const TestDataPtr createTestData();
  void writeTestData(const TestDataPtr& testData);
  const TestDataPtr readTestData();
  bool isEqual(const TestDataPtr& data1, const TestDataPtr& data2);
  
  typedef std::shared_ptr<TrainedData> TrainedDataPtr;
  TrainedDataPtr m_trainedData;

  std::string m_filePath;
  void printTestData(const TestDataPtr& data);
};
#endif /* defined(__ClassifierTrainer__TrainedDataTest__) */
