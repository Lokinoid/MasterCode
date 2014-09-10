//
//  ImageCollectionMultiplier.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 22/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__ImageCollectionMultiplier__
#define __ClassifierTrainer__ImageCollectionMultiplier__


#include "LandmarkCollectionData.h"
#include <opencv2/opencv.hpp>
#include "ImageWriter.h"
#include "CommonTypes.h"


class ImageCollectionMultiplier
{
public:
  
  typedef enum
  {
    eImageTransformTypeRotate = 1<<0,
    eImageTransformTypeScale = 1<<1
  } EImageTransformType;
  
  ImageCollectionMultiplier();
  ~ImageCollectionMultiplier();
  
  typedef LandmarkCollectionData::LandmarkArrayPtr LandmarkArrayPtr;
  typedef LandmarkCollectionData::LandmarkType LandmarkType;
  
  const LandmarkCollectionDataPtr MultiplyCollectionData(const LandmarkCollectionDataPtr& data,
                                                        int transformType,
                                                        bool shouldSaveNewImages);
  
private:
  
  typedef std::vector<cv::Mat> TransformListType;
  typedef std::shared_ptr<TransformListType> TransformListTypePtr;

  void transform(const ImageLandmarkDataPtr& data,
                 int transformType,
                 std::function<void (const ImageLandmarkDataPtr& data)> processCallback);

  TransformListTypePtr transformList(const ImageLandmarkDataPtr& data,
                                     int transformType);

//  ImageLandmarkDataPtr createLandmarkData(const ImageLandmarkData::LandmarkArrayPtr& landmarks,
//                                          const cv::Mat& image);

//  LandmarkArrayPtr transformLandmarks(const LandmarkArrayPtr& landmarks,
//                                      const cv::Mat& transformMat,
//                                      std::function<bool (const LandmarkType&)> checkCallback);
//  const cv::Mat transformImage(const cv::Mat& image, const cv::Mat& transformMat);
//
//  cv::Mat landmarkToMat(const LandmarkType& landmark);
//  LandmarkType matToLandmark(const cv::Mat& mat);
//
//  ImageWriter* m_writerInstance;
  bool m_shouldSaveNewImages;
};

#endif /* defined(__ClassifierTrainer__ImageCollectionMultiplier__) */
