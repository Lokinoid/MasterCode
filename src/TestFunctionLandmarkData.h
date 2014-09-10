//
//  TestFunctionLandmarkData.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 10/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__TestFunctionLandmarkData__
#define __ClassifierTrainer__TestFunctionLandmarkData__

#include <opencv2/opencv.hpp>
#include "ImageLandmarkData.h"
#include "TestFunctionFeature.h"


class TestFunctionLandmarkData : public ImageLandmarkData
{
public:

  TestFunctionLandmarkData();
  
  const cv::Mat ImageSource();
  void SetImageSource(const cv::Mat& imageSource, bool shouldSave);
  
private:
  
  TestFunctionFeature m_feature;
  
};


#endif /* defined(__ClassifierTrainer__TestFunctionLandmarkData__) */
