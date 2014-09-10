//
//  IFeature.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 24/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__IFeature__
#define __ClassifierTrainer__IFeature__


#include <opencv2/opencv.hpp>

class IFeature
{
public:
  
  virtual ~IFeature() {}
  
  virtual const unsigned int NumberOfLandmarks() const = 0;
  
  virtual const cv::Mat StartPosition(const cv::Mat& image) = 0;
};

#endif /* defined(__ClassifierTrainer__IFeature__) */
