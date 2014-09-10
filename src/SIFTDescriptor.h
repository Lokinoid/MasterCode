//
//  SIFTDescriptor.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 06/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__SIFTDescriptor__
#define __ClassifierTrainer__SIFTDescriptor__

#include "IDescriptor.h"
#include <opencv2/opencv.hpp>


namespace descriptor
{
  
class SIFTDescriptor : public IDescriptor
{
public:
  
  SIFTDescriptor(const int diameter);
  
  const cv::Mat ComputeDescriptor(const cv::Mat& image, const cv::Mat& position);

  const size_t DescriptorSize() const;
  const cv::Size WindowSize() const;
  
private:
  
  const cv::Mat computeDescriptorForPoints(const cv::Mat& image, const PointArrayPtr& pointArray);

  typedef cv::DescriptorExtractor DescriptorType;
  typedef cv::Ptr<DescriptorType> DescriptorPtr;
  
  DescriptorPtr m_descriptorExtractor;
  int m_diameter;
  
  const DescriptorPtr configureDescriptorExtractor();  
  
};
  
}
#endif /* defined(__ClassifierTrainer__SIFTDescriptor__) */
