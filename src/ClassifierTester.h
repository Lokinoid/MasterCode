//
//  ClassifierTester.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 04/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__ClassifierTester__
#define __ClassifierTrainer__ClassifierTester__

#include <opencv2/opencv.hpp>
#include <functional>
#include "CommonTypes.h"
#include "StatisticProduser.h"


class Detector;

class ClassifierTester
{
public:
    
  double StartTest(const LandmarkCollectionDataPtr& collection,
                   const std::string& savedTrainedDataFilePath,
                   const int numberOfIterations,
                   bool showSteps);
  void StopTest();
  
private:
  
  const cv::Mat processImage(const cv::Mat& image, const cv::Mat& trueLandmarks);
  const cv::Mat testImageWithProcessCallback(const cv::Mat& image, std::function<void (const cv::Mat& landmarksMat)> callback);
  
  const cv::Mat addLandmarksOnImage(const cv::Mat& image, const cv::Mat& landmarks, const int thick, const cv::Scalar& color);
  
  const cv::Mat checkAndConvertPosition(const cv::Mat& position, const cv::Size& imageSize);
  void collectStatistics(const cv::Mat& landmarks, const cv::Mat& trueLandmarks);
  double calculateStatistics();
  
  std::string m_savedTrainedDataFilePath;
  FeaturePtr m_feature;
  bool m_showSteps;
  int m_numberOfIterations;
  
  typedef statistics::TMatContainerPtr TMatContainerPtr;
  TMatContainerPtr m_statisticsContainer;
  
  LandmarkCollectionDataPtr m_collection;
  
  typedef std::shared_ptr<Detector> DetectorPtr;
  DetectorPtr m_detector;
};
#endif /* defined(__ClassifierTrainer__ClassifierTester__) */
