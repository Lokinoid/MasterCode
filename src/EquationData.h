//
//  EquationData.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 25/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__EquationData__
#define __ClassifierTrainer__EquationData__

#include <opencv2/opencv.hpp>
#include <memory>


//! @brief Storage for data that is used in equation
//!        \Delta x = A*\phi + b
//!
class ImageLandmarkData;

class EquationData
{
public:
  
  typedef std::shared_ptr<ImageLandmarkData> ImageLandmarkDataPtr;
  
  EquationData();
  ~EquationData();
  
  const cv::Mat Descriptor() const;
  const cv::Mat DeltaPosition() const;
  const cv::Mat CurrentPosition() const;
  const cv::Mat BestPosition() const;
  
  void SetCurrentPosition(const cv::Mat& currentPosition);
  void SetBestPosition(const cv::Mat& bestPosition);
  void SetDescriptor(const cv::Mat& descriptor);
  
private:
  
  cv::Mat m_currentPosition;
  cv::Mat m_bestPosition;
  cv::Mat m_descriptor;
};

#endif /* defined(__ClassifierTrainer__EquationData__) */
