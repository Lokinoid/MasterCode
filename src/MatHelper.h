//
//  MatHelper.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 28/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__MatHelper__
#define __ClassifierTrainer__MatHelper__

#include <opencv2/opencv.hpp>
#include "CommonTypes.h"


class MatHelper
{
public:
  
  static const cv::Mat extPointToMat(const PointType& point);
  
  static const PointType matToExtPoint(const cv::Mat& mat);
  
  static const cv::Mat pointsToMat(const PointArrayPtr& poins);
  
  static const cv::Mat pointToMat(const PointType& point);
  
  static const PointArrayPtr matToPoints(const cv::Mat& mat);

  static const PointType matToPoint(const cv::Mat& mat);
  
  static const cv::Mat matsToMat(const cv::Mat& matX, const cv::Mat& matY);
  
  static const cv::Rect rectAtPoint(const PointType& point,
                                    const cv::Size& imageSize,
                                    const cv::Size& windowSize);
};

#endif /* defined(__ClassifierTrainer__MatHelper__) */
