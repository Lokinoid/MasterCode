//
//  ImageTransformer.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 31/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ImageTransformer.h"

#include <opencv2/imgproc/imgproc.hpp>

#include "LandmarkCollectionData.h"
#include "ImageLandmarkData.h"
#include "CascadeClassifier.h"
#include "MatHelper.h"
#include "ImageWriter.h"


static const int kImageSize = 200;

ImageTransformer::ImageTransformer(const std::string& newImagesDirectory)
: m_imageWriter(ImageWriter::sharedInstance()),
  m_imageDirectory(newImagesDirectory)
{
}

const ImageLandmarkDataPtr ImageTransformer::TransformDataWithMat(const ImageLandmarkDataPtr& landmarkData,
                                                                  const cv::Mat& transformMat,
                                                                  bool shouldSaveNewImage)
{
  
  PointArrayPtr newLandmarks = transformLandmarks(landmarkData->Landmarks(), transformMat, [&] (const PointType& point) {
    
    return (point.x >= 0 &&
            point.x < landmarkData->ImageSource().cols &&
            point.y >= 0 &&
            point.y < landmarkData->ImageSource().rows);
  });
  
  ImageLandmarkDataPtr newLandmarkData;
  if (newLandmarks)
  {
    const cv::Mat newImage = transformImage(landmarkData->ImageSource(), transformMat);
    newLandmarkData = createLandmarkData(newLandmarks, newImage, shouldSaveNewImage);
  }
  
  return newLandmarkData;
}

const cv::Mat CropImage(const cv::Mat& image, const cv::Rect& rect)
{
  return image(rect);
}

const ImageLandmarkDataPtr ImageTransformer::TransformData(const ImageLandmarkDataPtr& landmarkData,
                                                           const cv::Rect& activeRect,
                                                           const cv::Size& destSize,
                                                           bool shouldSaveNewImage)
{
  double scaleFactor = (double)destSize.width / (double)activeRect.size().width;
//  double scaleY = destSize.height / activeRect.size().height;
  
  const cv::Point2i center(activeRect.x + activeRect.width / 2, activeRect.y + activeRect.height / 2);
  cv::Mat transformMat = cv::getRotationMatrix2D(center, 0.f, scaleFactor);
  
  const ImageLandmarkDataPtr newLandmarkData = TransformDataWithMat(landmarkData, transformMat, shouldSaveNewImage);
  
  return newLandmarkData;
}

const LandmarkCollectionDataPtr ImageTransformer::TransformCollectionUsingFaceCrop(const LandmarkCollectionDataPtr& collectionData)
{
  LandmarkCollectionDataPtr newCollection(new LandmarkCollectionData(collectionData->CollectionSize()));
  
  CascadeClassifier classifier;
  collectionData->EnumerateConstColectionWithCallback([&] (const ImageLandmarkDataPtr& landmarkData, const int index, bool* stop) {

    const cv::Rect activeRect = classifier.DetectFace(landmarkData->ImageSource());
    if (activeRect.size().width != 0)
    {
      const ImageLandmarkDataPtr newData = TransformData(landmarkData, activeRect, {kImageSize, kImageSize}, true);
      newCollection->AddImageLandmarkData(newData);
    }
    else
    {
      std::cout<<"Face not found: "<<landmarkData->ImagePath()<<std::endl;
    }
    
    std::cout<<index<<std::endl;
  });
  
  return newCollection;
}

const cv::Mat ImageTransformer::transformImage(const cv::Mat& image, const cv::Mat& transformMat)
{
  cv::Mat newImage;
  cv::warpAffine(image, newImage, transformMat, image.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);

  return newImage;
}

const PointArrayPtr ImageTransformer::transformLandmarks(const PointArrayPtr& landmarks,
                                                         const cv::Mat& transformMat,
                                                         std::function<bool (const PointType&)> checkCallback)
{
  typedef ImageLandmarkData::LandmarkArray LandmarkArray;
  typedef ImageLandmarkData::LandmarkType LandmarkType;
  
  PointArrayPtr newLandmarks(new PointArrayType);
  newLandmarks->reserve(landmarks->size());
  
  for (const PointType& landmark : *landmarks)
  {
    cv::Mat landmarkMat = MatHelper::extPointToMat(landmark);
    cv::Mat resultLandmarkMat = transformMat * landmarkMat;
    
    PointType newLandmark = MatHelper::matToExtPoint(resultLandmarkMat);
    
    if (!checkCallback(newLandmark))
    {
      return PointArrayPtr();
    }
    
    newLandmarks->push_back(newLandmark);
  };
  
  return newLandmarks;
}

const ImageLandmarkDataPtr ImageTransformer::createLandmarkData(const PointArrayPtr& landmarks,
                                                                const cv::Mat& image,
                                                                bool shouldSave)
{
  PathType path = (PathType)m_imageWriter->GeneratePath(m_imageDirectory);
  
  ImageLandmarkDataPtr landmarkData = ImageLandmarkDataPtr(new ImageLandmarkData(path, landmarks));
  landmarkData->SetImageSource(image, shouldSave);
  
  return landmarkData;
}
