//
//  SIFTDescriptor.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 06/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "SIFTDescriptor.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "MatHelper.h"


descriptor::SIFTDescriptor::SIFTDescriptor(const int diameter)
: m_descriptorExtractor(DescriptorPtr()),
  m_diameter(diameter)
{
  m_descriptorExtractor = configureDescriptorExtractor();
}

const size_t descriptor::SIFTDescriptor::DescriptorSize() const
{
  return static_cast<size_t>(m_descriptorExtractor->descriptorSize());
}

const cv::Mat descriptor::SIFTDescriptor::ComputeDescriptor(const cv::Mat& image, const cv::Mat& position)
{
  const PointArrayPtr points = MatHelper::matToPoints(position);
  const cv::Mat descriptor = computeDescriptorForPoints(image, points);
  const cv::Mat resultDescriptor = descriptor.reshape(0, 1).t();
  
  const double kExtendedPart = 1;
  cv::Mat extendedDescriptor = resultDescriptor;
  extendedDescriptor.push_back(kExtendedPart);
  
  return extendedDescriptor;
}

const cv::Mat descriptor::SIFTDescriptor::computeDescriptorForPoints(const cv::Mat& image, const PointArrayPtr& pointArray)
{
  cv::Mat processedImage(image);
  
  const int BGR2GRAY = 6;
  cvtColor(processedImage, processedImage, BGR2GRAY);
  cv::equalizeHist(processedImage, processedImage);
  
  
  std::vector<cv::KeyPoint> keyPoints;
  keyPoints.reserve(pointArray->size());
  
  for (const PointType& point : *pointArray)
  {
    keyPoints.push_back(cv::KeyPoint(point, static_cast<float>(m_diameter)));
  }
  
  cv::Mat floatDescriptors;
  m_descriptorExtractor->compute(processedImage, keyPoints, floatDescriptors);
  
  cv::Mat_<double> resultMat;
  floatDescriptors.convertTo(resultMat, CV_64FC1);

  return resultMat;
}

const descriptor::SIFTDescriptor::DescriptorPtr descriptor::SIFTDescriptor::configureDescriptorExtractor()
{
  DescriptorPtr descriptor = DescriptorType::create("SIFT");
  
  return descriptor;
}

const cv::Size descriptor::SIFTDescriptor::WindowSize() const
{
  return cv::Size(m_diameter, m_diameter);
}
