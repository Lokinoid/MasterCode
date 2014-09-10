//
//  StarFeature.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 24/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "StarFeature.h"
#include <iostream>
#include <list>
#include "MatHelper.h"
#include "HOGDescriptor.h"


#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923


static const unsigned int kNumberOfLandmarks = 5;

StarFeature::StarFeature()
: m_startPosition(cv::Mat()),
  m_numberOfLandmarks(kNumberOfLandmarks)
{
}

StarFeature::~StarFeature()
{
}

const unsigned int StarFeature::NumberOfLandmarks() const
{
  return m_numberOfLandmarks;
}

const cv::Mat StarFeature::StartPosition(const cv::Mat& image)
{
  if (m_startPosition.empty())
  {
    m_startPosition = createStartPosition(image.size());
  }
  
  return m_startPosition;
}

const cv::Mat StarFeature::createStartPosition(const cv::Size& size)
{
  const double xCenter = size.width / 2.0;
  const double yCenter = size.height / 2.0;
  const double rStar = std::min(size.height, size.width) / 8.0;
  const double alphaStep = M_PI * 2 / 5;
  
  cv::Mat_<double> x(m_numberOfLandmarks, 1, CV_64FC1);
  cv::Mat_<double> y(m_numberOfLandmarks, 1, CV_64FC1);

//  double xCurrent;

  double alpha = -M_PI_2;
  for (int i = 0; i < m_numberOfLandmarks; i++)
  {
    x(i, 0) = static_cast<double>(std::round(xCenter + rStar*std::cos(alpha)));
    y(i, 0) = static_cast<double>(std::round(yCenter + rStar*std::sin(alpha)));

    alpha += alphaStep;
  }
  
  // result matrix as (x_1, y_1, ..., x_n, y_n)'
  cv::Mat resultMat = MatHelper::matsToMat(x, y);
  
  return resultMat;
}
