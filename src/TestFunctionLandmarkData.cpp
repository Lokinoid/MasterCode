//
//  TestFunctionLandmarkData.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 10/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//


#include "TestFunctionLandmarkData.h"
#include "MatHelper.h"


TestFunctionLandmarkData::TestFunctionLandmarkData()
: ImageLandmarkData(),
  m_feature(TestFunctionFeature())
{
  ImageLandmarkData::ClearLandmarks();
  ImageLandmarkData::AddLandmarkArray(MatHelper::matToPoints(m_feature.Values()));
}

const cv::Mat TestFunctionLandmarkData::ImageSource()
{
  return m_feature.Values(); // landmarks are the best position of feature, so they should be a_0,...a_10
}

void TestFunctionLandmarkData::SetImageSource(const cv::Mat& imageSource, bool shouldSave)
{
}
