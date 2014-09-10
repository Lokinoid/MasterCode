//
//  TestFunctionFeature.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 09/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__TestFunctionFeature__
#define __ClassifierTrainer__TestFunctionFeature__


#include "IFeature.h"
#include <vector>


//! @brief This class represents polynom function as a new feature
//!        f(x) = (x_0 - a_0)^2 +  ... + (x_10 - a_10)^2
//!
class TestFunctionFeature : public IFeature
{
public:
  
  TestFunctionFeature();
  
  const unsigned int NumberOfLandmarks() const;
  
  //! @brief X_0 = (x_1, y_1, x_2, ... , y_5) = (0,...,0)
  const cv::Mat StartPosition(const cv::Mat& image);
  
  //! @brief D = grad(f)(X_0) = ( 2*(x_0 - a_0), ... , 2*(x_10 - a_10), some_noise )
  const cv::Mat Descriptor(const cv::Mat& image, const cv::Mat& koords);
  
  const cv::Mat Values() const;
  
private:
    
  cv::Mat m_values; // a = (a_0, a_1, ... , a_10)
  
  const cv::Mat createValueContainer();
  const cv::Mat createStartPosition();
  const cv::Mat grad(const cv::Mat& values,
                     const cv::Mat& koords);
  const cv::Mat randomMat(const int numerOfValues);
};

#endif /* defined(__ClassifierTrainer__TestFunctionFeature__) */
