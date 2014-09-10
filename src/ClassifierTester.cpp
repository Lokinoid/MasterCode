 //
//  ClassifierTester.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 04/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ClassifierTester.h"

#include <boost/assert.hpp>

#include "TrainedData.h"
#include "StarFeature.h"
#include "FileFinder.h"
#include "MatHelper.h"
#include "LandmarkCollectionData.h"
#include "ImageLandmarkData.h"
#include "StatisticProduser.h"
#include "Detector.h"


double ClassifierTester::StartTest(const LandmarkCollectionDataPtr& collection,
                                 const std::string& savedTrainedDataFilePath,
                                 const int numberOfIterations,
                                 bool showSteps)
{
  BOOST_ASSERT(savedTrainedDataFilePath.size() != 0);
  BOOST_ASSERT(collection->CollectionSize() != 0);
  
  m_collection = collection;
  m_savedTrainedDataFilePath = savedTrainedDataFilePath;
  m_showSteps = showSteps;
  m_numberOfIterations = numberOfIterations;
  
  m_detector = DetectorPtr(new Detector(m_savedTrainedDataFilePath));
  
  int i = 0;
  collection->EnumerateConstColectionWithCallback([&] (const ImageLandmarkDataPtr& landmarkData, const int index, bool* stop) {
    
//    std::cout<<index<<std::endl;
    
    const cv::Mat image = landmarkData->ImageSource();
    const cv::Mat landmarks = processImage(image, landmarkData->LandmarksMat());
    if (landmarks.empty())
    {
      i++;
//      std::cout<<"Fail image "<<landmarkData->ImagePath()<<std::endl;
    }
    else
    {
      collectStatistics(landmarks, landmarkData->LandmarksMat());
    }
  });
  
  std::cout<<"Numeber of fail images: "<<i<<std::endl;
  return calculateStatistics();
}

const cv::Mat ClassifierTester::processImage(const cv::Mat& image, const cv::Mat& trueLandmarks)
{
    std::function<void(const cv::Mat&)> callback = [&] (const cv::Mat& landmarksMat) {
      
      double mistake = statistics::StatisticProduser::AbsSumMistake(landmarksMat, trueLandmarks);

      cv::Mat tmpImage = addLandmarksOnImage(image, landmarksMat, 1, cv::Scalar(255,0,0));
      cv::Mat trueImage = addLandmarksOnImage(tmpImage, trueLandmarks, 1, cv::Scalar(0,255,0));
      cv::resize(trueImage, trueImage, cv::Size(), 2, 2);
//      cvtColor(trueImage,trueImage,CV_BGR2GRAY);
      
      cv::imshow("Abs mistake = " + std::to_string(mistake), trueImage);
      cv::waitKey(0);
  };
  
  cv::Mat landmarks = m_detector->Detect(image, m_numberOfIterations);
  if (m_showSteps)
  {
    callback(landmarks);
  }
  
  return landmarks;
}

const cv::Mat ClassifierTester::addLandmarksOnImage(const cv::Mat& image, const cv::Mat& position, const int thick, const cv::Scalar& color)
{
  cv::Mat newImage;
  image.copyTo(newImage);
  
  PointArrayPtr landmarks = MatHelper::matToPoints(position);
  for (const PointType point : * landmarks)
  {
    cv::circle(newImage, point, 1, color, thick);
  }
  
  return newImage;
}

void ClassifierTester::collectStatistics(const cv::Mat& landmarks, const cv::Mat& trueLandmarks)
{
  if (!m_statisticsContainer)
  {
    m_statisticsContainer = TMatContainerPtr(new statistics::TMatContainer);
    m_statisticsContainer->reserve(m_collection->CollectionSize());
  }
    
  const cv::Mat mistakeMat = statistics::StatisticProduser::AbsMistake(landmarks, trueLandmarks);
  m_statisticsContainer->push_back(mistakeMat);
}

double ClassifierTester::calculateStatistics()
{
	const cv::Mat statHist = statistics::StatisticProduser::Hist(m_statisticsContainer, 0, 100, 99);
//  std::cout<<statHist.t()<<std::endl;
  
  const double thresh = 3;
  
  const double amount = statistics::StatisticProduser::Cumsum(statHist, 0, INT_MAX);
  const double lowerCumsum = statistics::StatisticProduser::Cumsum(statHist, 0, thresh) / amount;
  const double upperCumsum = statistics::StatisticProduser::Cumsum(statHist, thresh, INT_MAX) / amount;
  
//  std::cout<<"lower then " << thresh << ": "<<lowerCumsum<<std::endl;
//  std::cout<<"upper then " << thresh << ": "<<upperCumsum<<std::endl;
//  
  return lowerCumsum;
}
