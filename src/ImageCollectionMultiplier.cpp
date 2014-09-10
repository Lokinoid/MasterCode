//
//  ImageCollectionMultiplier.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 22/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ImageCollectionMultiplier.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include "ImageLandmarkData.h"
#include "ImageTransformer.h"


ImageCollectionMultiplier::ImageCollectionMultiplier()
: m_shouldSaveNewImages(true)
{
  
}

ImageCollectionMultiplier::~ImageCollectionMultiplier()
{
  
}

const LandmarkCollectionDataPtr ImageCollectionMultiplier::MultiplyCollectionData(const LandmarkCollectionDataPtr& data,
                                                                                  int transformType,
                                                                                  bool shouldSaveNewImages)
{
  typedef std::function<void (const ImageLandmarkDataPtr& landmarks)> CallbackType;

  m_shouldSaveNewImages = shouldSaveNewImages;
  LandmarkCollectionDataPtr collection = LandmarkCollectionDataPtr(new LandmarkCollectionData);
  CallbackType processCallback = [&] (const ImageLandmarkDataPtr& data) {
    
    collection->AddImageLandmarkData(data);
    data->PrintLandmarkData();
    
  };
  
  CallbackType callback = [&](const ImageLandmarkDataPtr& data){
    
    transform(data, transformType, processCallback);
  };
  
  data->EnumerateColectionWithCallback(callback);
  
  return collection;
}


void ImageCollectionMultiplier::transform(const ImageLandmarkDataPtr& data,
                                          int transformType,
                                          std::function<void (const ImageLandmarkDataPtr& data)> processCallback)
{
  ImageTransformer transformer;
  TransformListTypePtr transforms = transformList(data, transformType);
  
  for (const cv::Mat& transformMat : *transforms)
  {
    ImageLandmarkDataPtr newData = transformer.TransformDataWithMat(data, transformMat, m_shouldSaveNewImages);
    if (newData)
    {
      processCallback(newData);
    }
  }
}

ImageCollectionMultiplier::TransformListTypePtr ImageCollectionMultiplier::transformList(const ImageLandmarkDataPtr& data,
                                                                                         int transformType)
{
  double startScaleRate = 1;
  double deltaScaleRate = 0.05 ;
  double endScaleRate = startScaleRate;
  
  if (transformType & eImageTransformTypeScale)
  {
    endScaleRate = 0.7;
  }
  
  double startAngle = -10;
  double deltaAngle = 0.1;
  double endAngle = startAngle + deltaAngle;
  
  if (transformType & eImageTransformTypeRotate)
  {
    endAngle = 10;
  }
  
  TransformListTypePtr transforms = TransformListTypePtr(new TransformListType);
  
  const cv::Mat& imageSource = data->ImageSource();
  cv::Point center = cv::Point(imageSource.cols / 2, imageSource.rows / 2);
  
  for (double scale = startScaleRate; scale > endScaleRate; scale -= deltaScaleRate)
  {
    for (double angle = startAngle; angle < endAngle; angle += deltaAngle)
    {
      transforms->push_back(cv::getRotationMatrix2D(center, angle, scale));
    }
  }
  
  return transforms;
}
