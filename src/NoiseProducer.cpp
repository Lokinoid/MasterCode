//
//  NoiseProducer.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 26/05/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "NoiseProducer.h"


const cv::Mat NoiseProducer::AddNoise(const cv::Mat& image)
{
  NoiseProducer producer(128, 10);
  const cv::Mat newImage = producer.addNoise(image);
  
  return newImage;
}

NoiseProducer::NoiseProducer(const double mu, const double sigma)
: m_mu(mu),
  m_sigma(sigma)
{
}

const cv::Mat NoiseProducer::addNoise(const cv::Mat& image)
{
  cv::Mat noiseImage = image.clone();
  
  cv::randn(noiseImage, cv::Scalar::all(m_mu), cv::Scalar::all(m_sigma));
  
  cv::addWeighted(image, 1, noiseImage, 1, -128, noiseImage);
  
  cv::imshow("noise", noiseImage);
  cv::waitKey();
  
  return cv::Mat();
}
