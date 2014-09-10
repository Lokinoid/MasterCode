//
//  TestFunctionFeature.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 09/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "TestFunctionFeature.h"
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <time.h>
#include "MatHelper.h"


static const unsigned int kNumberOfLandmarks = 5;


TestFunctionFeature::TestFunctionFeature()
{
  m_values = createValueContainer();
}

const unsigned int TestFunctionFeature::NumberOfLandmarks() const
{
  return kNumberOfLandmarks;
}

const cv::Mat TestFunctionFeature::StartPosition(const cv::Mat& image)
{
  return createStartPosition();
}

const cv::Mat TestFunctionFeature::Descriptor(const cv::Mat& image, const cv::Mat& koords)
{
  cv::Mat descriptor = grad(image, koords);
  
  const int kNumberOfRandomValues = 10;
  descriptor.push_back(randomMat(kNumberOfRandomValues));
  
  return descriptor;
}

const cv::Mat TestFunctionFeature::Values() const
{
  return m_values;
}


const cv::Mat TestFunctionFeature::createValueContainer()
{
  const unsigned int kNumberOfValues = kNumberOfLandmarks * 2;
  
  cv::Mat_<double> container;
  
  const int kMax = 1000000;
  const int kMin = -1000000;
  
  for (int i = 0; i < kNumberOfValues; i++)
  {
    double value = static_cast<double>(rand() % (kMax - kMin) + kMin) / kMax;
    container.push_back(value);
  }

  return container;
}

const cv::Mat TestFunctionFeature::createStartPosition()
{
  const cv::Mat matX = cv::Mat::zeros(m_values.rows, 1, CV_64FC1);
  
  return matX;
}

const cv::Mat TestFunctionFeature::grad(const cv::Mat& values,
                                        const cv::Mat& koords)
{
  cv::Mat_<double> grad = (koords - values);
  return grad;
}

const cv::Mat TestFunctionFeature::randomMat(const int numberOfValues)
{
  cv::Mat randomPart = cv::Mat(numberOfValues, 1, CV_64FC1);
  cv::randu(randomPart, cv::Scalar::all(-0.5), cv::Scalar::all(0.5));

  return randomPart;
}
