//
//  ImageTransformer.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 31/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__ImageTransformer__
#define __ClassifierTrainer__ImageTransformer__

#include <opencv2/opencv.hpp>
#include "CommonTypes.h"


class ImageWriter;

class ImageTransformer
{
public:

  ImageTransformer(const std::string& newImagesDirectory = "");
  
  const ImageLandmarkDataPtr TransformDataWithMat(const ImageLandmarkDataPtr& landmarkData,
                                                  const cv::Mat& transformMat,
                                                  bool shouldSaveNewImage);
  
  const cv::Mat CropImage(const cv::Mat& image, const cv::Rect& rect);
  
  const ImageLandmarkDataPtr TransformData(const ImageLandmarkDataPtr& landmarkData,
                                           const cv::Rect& activeRect,
                                           const cv::Size& destSize,
                                           bool shouldSaveNewImage);
  const LandmarkCollectionDataPtr TransformCollectionUsingFaceCrop(const LandmarkCollectionDataPtr& collectionData);
  
private:
  
  const cv::Mat transformImage(const cv::Mat& image, const cv::Mat& transformMat);
  
  const PointArrayPtr transformLandmarks(const PointArrayPtr& landmarks,
                                         const cv::Mat& transformMat,
                                         std::function<bool (const PointType&)> checkCallback);

  const ImageLandmarkDataPtr createLandmarkData(const PointArrayPtr& landmarks,
                                                const cv::Mat& image,
                                                bool shouldSaveNewImage);
  
  typedef std::shared_ptr<ImageWriter> ImageWriterPtr;
  ImageWriterPtr m_imageWriter;
  
  std::string m_imageDirectory;
};
#endif /* defined(__ClassifierTrainer__ImageTransformer__) */
