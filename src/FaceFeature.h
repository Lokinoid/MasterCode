//
//  FaceFeature.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 06/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__FaceFeature__
#define __ClassifierTrainer__FaceFeature__

#include <opencv2/opencv.hpp>
#include "IFeature.h"
#include "CommonTypes.h"


class CascadeClassifier;


class FaceFeature : public IFeature
{
public:
  
  FaceFeature();
  ~FaceFeature();
  
  const unsigned int NumberOfLandmarks() const;
  
  const cv::Mat StartPosition(const cv::Mat& image);
  
  // Debug
  void PrintFacePosition(const cv::Mat& image, const cv::Mat& position);

private:
  
  cv::Mat m_startPosition;
  cv::Size m_size;
  PointType m_origin;
  
  const unsigned int m_numberOfLandmarks;
  
  const cv::Mat createStartPosition(const cv::Rect& activeRect);
  const cv::Mat createDescriptor(const cv::Mat& image, const cv::Mat& koords);
  
  const cv::Mat descriptorForImageInPoints(const cv::Mat& image, const PointArrayPtr& points);

  const cv::Mat eyeLandmarks(const PointType& point);
  const cv::Mat browLandmarks(const PointType& point);
  const cv::Mat noseTopLandmarks();
  const cv::Mat noseBottomLandmarks();
  const cv::Mat mouthLandmarks();
  const cv::Mat contourLandmarks();

  const cv::Mat drawEllipse(const PointType& center,
                            const double alphaStart,
                            const double alphaEnd,
                            const double radiusX,
                            const double radiusY,
                            const int numberOfLandmarks,
                            bool includeEdges);

  
  typedef std::shared_ptr<CascadeClassifier> CascadeClassifierPtr;
  CascadeClassifierPtr m_cascadeClassifier;
};

#endif /* defined(__ClassifierTrainer__FaceFeature__) */
