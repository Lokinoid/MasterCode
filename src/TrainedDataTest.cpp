//
//  TrainedDataTest.cpp
//  ClassifierTrainer
//
//  Created by Stanislav on 21.04.14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "TrainedDataTest.h"
#include "TrainedData.h"


static const int kNumberOfIterations = 5;

void TrainedDataTest::Test()
{
  TrainedDataTest tester;
  tester.startTest();
}


TrainedDataTest::TrainedDataTest()
: m_trainedData(new TrainedData)
{
}

void TrainedDataTest::startTest()
{
  m_filePath = "/Users/stanislav/Dropbox/Master_Thesis/Code/test.xml";
  m_trainedData->Load(m_filePath);
  printTestData(NULL);
  
  TestDataPtr dataToWrite = createTestData();
  writeTestData(dataToWrite);
  
  TestDataPtr readData = readTestData();
  
  std::string res = isEqual(readData, dataToWrite) ? "Passed" : "Not Passed";
  std::cout<<res<<std::endl;
}

const TrainedDataTest::TestDataPtr TrainedDataTest::createTestData()
{
  TestDataPtr testData(new TestDataType);
  for (int i = 0; i < kNumberOfIterations; i++)
  {
    cv::Mat data = cv::Mat::ones(1, 5, CV_64FC1) * i;
    std::cout<<data<<std::endl;
    testData->push_back(data);
  }
  
  return testData;
}

void TrainedDataTest::writeTestData(const TestDataPtr& testData)
{
  for (int i = 0; i < testData->size(); i++)
  {
    m_trainedData->AddData(testData->at(i), true, m_filePath);
  }
}

const TrainedDataTest::TestDataPtr TrainedDataTest::readTestData()
{
  if (!m_trainedData->Load(m_filePath))
  {
    std::cout<<"Cannot load " << m_filePath<<std::endl;
    return TestDataPtr();
  }
  
  TestDataPtr testData(new TestDataType);
  for (int i = 0; i < m_trainedData->NumberOfIterations(); i++)
  {
    cv::Mat data = m_trainedData->at(i);
    testData->push_back(data);
  }
  
  return testData;
}

bool TrainedDataTest::isEqual(const TrainedDataTest::TestDataPtr& data1, const TrainedDataTest::TestDataPtr& data2)
{
  if (data1->size() != data2->size())
  {
    return false;
  }
  
  for (int i = 0; i < data1->size(); i++)
  {
    
    for (int c = 0; c < data1->at(i).cols; c++)
    {
      for (int r = 0; r < data2->at(i).rows; r++)
      {
        if (data1->at(i).at<double>(r,c) != data2->at(i).at<double>(r,c))
        {
          return false;
        }
      }
    }
  }
  
  return true;
}

void TrainedDataTest::printTestData(const TestDataPtr& data)
{
  if (data)
  {
    for (int i = 0; i < data->size(); i++)
    {
      std::cout<<data<<std::endl;
    }
  }
  
  for (int i = 0; i < m_trainedData->NumberOfIterations(); i++)
  {
    std::cout<<m_trainedData->at(i)<<std::endl;
  }
}