//
//  NoiseProducer.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 26/05/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__NoiseProducer__
#define __ClassifierTrainer__NoiseProducer__


#include <opencv2/opencv.hpp>


class NoiseProducer
{
public:
  static const cv::Mat AddNoise(const cv::Mat& image);
  
private:
  
  NoiseProducer(const double mu, const double sigma);
  const cv::Mat addNoise(const cv::Mat& image);
  
  double m_mu;
  double m_sigma;
};

#endif /* defined(__ClassifierTrainer__NoiseProducer__) */
