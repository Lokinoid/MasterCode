//
//  StatisticProduser.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 25/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__StatisticProduser__
#define __ClassifierTrainer__StatisticProduser__

#include <opencv2/opencv.hpp>
#include <vector>
#include <memory>


namespace statistics
{
  typedef std::vector<cv::Mat> TMatContainer;
  typedef std::shared_ptr<TMatContainer> TMatContainerPtr;
  
  class StatisticProduser
  {
  public:
    
    // 1/N * sum_0^{N-1} sqrt( (x_1(i) - x_2(i))^2 + (y_1(i) - y_2(i))^2 )
    static const double AvgMistake(const cv::Mat& mat1, const cv::Mat& mat2);
    
    // abs( sqrt( (x_1 - x_2)^2 + (y_1 - y_2)^2 ) )
    static const cv::Mat AbsMistake(const cv::Mat& mat1, const cv::Mat& mat2);
    
    // sum_0^{N-1} sqrt( (x_1(i) - x_2(i))^2 + (y_1(i) - y_2(i))^2 )
    static const double AbsSumMistake(const cv::Mat& mat1, const cv::Mat& mat2);
    
    // histogram of abs mistakes
    // [min .. max] and [max .. INT_MAX]
    static const cv::Mat Hist(const TMatContainerPtr& matContainer,
                              const double min,
                              const double max,
                              const int number);
    
    static const cv::Mat Hist(const cv::Mat& mat,
                              const double min,
                              const double max,
                              const int number);
    
    static const double Cumsum(const cv::Mat& mat,
                               const double min,
                               const double max);
  };
  
}
#endif /* defined(__ClassifierTrainer__StatisticProduser__) */
