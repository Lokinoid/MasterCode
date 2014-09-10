//
//  EquationData.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 25/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "EquationData.h"


EquationData::EquationData()
: m_currentPosition(cv::Mat_<double>()),
  m_bestPosition(cv::Mat_<double>()),
  m_descriptor(cv::Mat_<double>())
{
}

EquationData::~EquationData()
{
}

const cv::Mat EquationData::Descriptor() const
{
  return m_descriptor;
}

const cv::Mat EquationData::DeltaPosition() const
{
  CV_Assert(m_currentPosition.cols == m_bestPosition.cols &&
            m_currentPosition.rows == m_bestPosition.rows);

  return m_bestPosition - m_currentPosition;
}

void EquationData::SetCurrentPosition(const cv::Mat& currentPosition)
{
  CV_Assert(currentPosition.depth() == CV_64FC1);

  m_currentPosition = currentPosition;
}

void EquationData::SetBestPosition(const cv::Mat& bestPosition)
{
  CV_Assert(bestPosition.depth() == CV_64FC1);

  bestPosition.copyTo(m_bestPosition);
}

void EquationData::SetDescriptor(const cv::Mat& descriptor)
{
  CV_Assert(descriptor.depth() == CV_64FC1);
  
  m_descriptor = descriptor;
}

const cv::Mat EquationData::CurrentPosition() const
{
  return m_currentPosition;
}

const cv::Mat EquationData::BestPosition() const
{
  return m_bestPosition;
}
