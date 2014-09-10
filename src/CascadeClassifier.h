//
//  CascadeClassifier.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 31/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__CascadeClassifier__
#define __ClassifierTrainer__CascadeClassifier__


#include <opencv2/opencv.hpp>
#include <memory>


class CascadeClassifier
{
public:
  
  CascadeClassifier();
  
  const cv::Rect DetectFace(const cv::Mat& image);
  
private:
  
  typedef std::shared_ptr<cv::CascadeClassifier> CascadeClassifierPtr;
  CascadeClassifierPtr m_cascadeClassifier;
  
  const CascadeClassifierPtr createCascadeClassifier();

};

#endif /* defined(__ClassifierTrainer__CascadeClassifier__) */
