//
//  StarFeature.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 24/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__StarFeature__
#define __ClassifierTrainer__StarFeature__

#include <opencv2/opencv.hpp>
#include "IFeature.h"
#include "CommonTypes.h"
#include "IDescriptor.h"


class StarFeature : public IFeature
{
public:
  
  StarFeature();
  ~StarFeature();
  
  const unsigned int NumberOfLandmarks() const;
  
  const cv::Mat StartPosition(const cv::Mat& image);
  
private:
  
  cv::Mat m_startPosition;
  const unsigned int m_numberOfLandmarks;
  
  const cv::Mat createStartPosition(const cv::Size& size);
};

#endif /* defined(__ClassifierTrainer__StarFeature__) */
