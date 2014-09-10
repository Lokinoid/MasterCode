//
//  DescriptorTester.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 05/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__DescriptorTester__
#define __ClassifierTrainer__DescriptorTester__

#include <opencv2/opencv.hpp>


class CFileFinder;

class DescriptorTester
{
public:
  
  static void StartTest();
  void StopTest();
  
  void ShowDescriptors(const cv::Mat& image, const cv::Mat& points);
  
private:
  
  const cv::Mat loadImage(const CFileFinder& fileFinder);

  const cv::Mat createVisualImage(const cv::Mat& origImg,
                                  const std::vector<double>& descriptorValues,
                                  const cv::Size winSize,
                                  const cv::Size cellSize,
                                  const int scaleFactor);
  const cv::Mat addPointArea(const cv::Mat& image,
                             const cv::Rect& rect);

};

#endif /* defined(__ClassifierTrainer__DescriptorTester__) */
