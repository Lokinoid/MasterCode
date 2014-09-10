//
//  FaceFeature.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 06/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "FaceFeature.h"

#include <iostream>
#include <list>
#include <opencv2/objdetect/objdetect.hpp>

#include "MatHelper.h"
#include "CascadeClassifier.h"


static const unsigned int kNumberOfLandmarks = 49;

FaceFeature::FaceFeature()
: m_startPosition(cv::Mat()),
  m_size(),
  m_numberOfLandmarks(kNumberOfLandmarks),
  m_cascadeClassifier(new CascadeClassifier)
{
}

FaceFeature::~FaceFeature()
{
}

const unsigned int FaceFeature::NumberOfLandmarks() const
{
  return m_numberOfLandmarks;
}

const cv::Mat FaceFeature::StartPosition(const cv::Mat& image)
{
  CV_Assert(!image.empty());
  
  const cv::Rect activeRect = m_cascadeClassifier->DetectFace(image);
  if (activeRect.width == 0)
  {
    return cv::Mat();
  }
  
  const cv::Mat startPosition = createStartPosition(activeRect);

//  PrintFacePosition(image, startPosition);

  return startPosition;
}

const cv::Mat FaceFeature::createStartPosition(const cv::Rect& activeRect)
{
  // we draw start position landmarks using ellipses
  
  if (activeRect.size().width == 0)
  {
    return cv::Mat();
  }
  
  m_size = activeRect.size();
  m_origin = activeRect.tl();
  
  const int kEyeCenterY = m_origin.y + m_size.height * 2 / 5;
  const int kLeftEyeCenterX = m_origin.x + m_size.width / 3;
  const int kRightEyeCenterX = m_origin.x + m_size.width * 2 / 3;
  
  cv::Mat_<double> face;
  face.push_back(eyeLandmarks({kLeftEyeCenterX, kEyeCenterY}));
  face.push_back(eyeLandmarks({kRightEyeCenterX, kEyeCenterY}));
  face.push_back(browLandmarks({kLeftEyeCenterX, kEyeCenterY}));
  face.push_back(browLandmarks({kRightEyeCenterX, kEyeCenterY}));
  face.push_back(noseTopLandmarks());
  face.push_back(noseBottomLandmarks());
  face.push_back(mouthLandmarks());
//  face.push_back(contourLandmarks());

  return face;
}

const cv::Mat FaceFeature::eyeLandmarks(const PointType& center)
{
  const int kNumberOfLandmarksForEye = 6;
  
  const double kAlphaStart = 0;
  const double kAlphaEnd = M_PI * 2;
  const double kEyeRadiusX = m_size.width / 10;
  const double kEyeRadiusY = m_size.height / 18;
  
  cv::Mat result = drawEllipse(center,
                               kAlphaStart,
                               kAlphaEnd,
                               kEyeRadiusX,
                               kEyeRadiusY,
                               kNumberOfLandmarksForEye,
                               false);
  
  return result;
}

const cv::Mat FaceFeature::browLandmarks(const PointType& center)
{
  const int kNumberOfLandmarksForEyeBrow = 5;
  
  const double kAlphaStart = -M_PI_4;
  const double kAlphaEnd = -M_PI - kAlphaStart;
  const double kBrowRadiusX = m_size.width / 5;
  const double kBrowRadiusY = m_size.height / 8;
  
  cv::Mat result = drawEllipse(center,
                               kAlphaStart,
                               kAlphaEnd,
                               kBrowRadiusX,
                               kBrowRadiusY,
                               kNumberOfLandmarksForEyeBrow,
                               true);
  
  return result;
}

const cv::Mat FaceFeature::noseTopLandmarks()
{
  const int kNumberOfLandmarksForNoseTop = 4;
  const int kTopNoseLeftOffset = m_origin.x + m_size.width / 2;
  cv::Mat_<double> x = cv::Mat::ones(kNumberOfLandmarksForNoseTop, 1, CV_64FC1) * kTopNoseLeftOffset;
  cv::Mat_<double> y(kNumberOfLandmarksForNoseTop, 1, CV_64FC1);
  
  const double yStart = m_origin.y + m_size.height * 2 / 5;
  const double yStride = m_size.height / 4 / kNumberOfLandmarksForNoseTop;
  
  for (int i = 0; i < kNumberOfLandmarksForNoseTop; i++)
  {
    y(i, 0) = static_cast<double>(std::round(yStart + i * yStride));
  }
  
  cv::Mat result = MatHelper::matsToMat(x, y);
  
  return result;
}

const cv::Mat FaceFeature::noseBottomLandmarks()
{
  const int kNumberOfLandmarksForNoseBottom = 5;
  
  const double kAlphaStart = M_PI * 3 / 8;
  const double kAlphaEnd = M_PI - kAlphaStart;
  const double kNoseBottomRadiusX = m_size.width / 5;
  const double kNoseBottomRadiusY = m_size.height / 10;
  const PointType kCenterPoint = m_origin + PointType(m_size.width / 2, m_size.height * 3 / 5);
  
  cv::Mat result = drawEllipse(kCenterPoint,
                               kAlphaStart,
                               kAlphaEnd,
                               kNoseBottomRadiusX,
                               kNoseBottomRadiusY,
                               kNumberOfLandmarksForNoseBottom,
                               true);
  
  return result;
}

const cv::Mat FaceFeature::mouthLandmarks()
{
  const PointType kCenterPoint = m_origin + PointType(m_size.width / 2, m_size.height * 5 / 6);
  
  const int kExternalEllipseLandmarkCount = 12;
  const double kExternalEllipseAlphaStart = 0;
  const double kExternalEllipseAlphaEnd = 2 * M_PI;
  const double kExternalEllipseRadiusX = m_size.width / 5;
  const double kExternalEllipseRadiusY = m_size.width / 15;
  const cv::Mat externalEllipse = drawEllipse(kCenterPoint,
                                              kExternalEllipseAlphaStart,
                                              kExternalEllipseAlphaEnd,
                                              kExternalEllipseRadiusX,
                                              kExternalEllipseRadiusY,
                                              kExternalEllipseLandmarkCount,
                                              false);
  
  const int kInternalEllipseLandmarkCount = 3;
  const double kInternalTopEllipseRadiusX = kExternalEllipseRadiusX / 2;
  const double kInternalTopEllipseRadiusY = kExternalEllipseRadiusY / 2;
  
  const double kInternalTopEllipseAlphaStart = -M_PI_4;
  const double kInternalTopEllipseAlphaEnd = -M_PI - kInternalTopEllipseAlphaStart;
  const cv::Mat internalTopEllipse = drawEllipse(kCenterPoint,
                                                 kInternalTopEllipseAlphaStart,
                                                 kInternalTopEllipseAlphaEnd,
                                                 kInternalTopEllipseRadiusX,
                                                 kInternalTopEllipseRadiusY,
                                                 kInternalEllipseLandmarkCount,
                                                 true);
  
  const double kInternalBottomEllipseAlphaStart = M_PI_4;
  const double kInternalBottomEllipseAlphaEnd = M_PI - kInternalBottomEllipseAlphaStart;
  const cv::Mat internalBottomEllipse = drawEllipse(kCenterPoint,
                                                    kInternalBottomEllipseAlphaStart,
                                                    kInternalBottomEllipseAlphaEnd,
                                                    kInternalTopEllipseRadiusX,
                                                    kInternalTopEllipseRadiusY,
                                                    kInternalEllipseLandmarkCount,
                                                    true);
  
  cv::Mat mouthLandmarks(externalEllipse);
  mouthLandmarks.push_back(internalTopEllipse);
  mouthLandmarks.push_back(internalBottomEllipse);
  
  return mouthLandmarks;
}

const cv::Mat FaceFeature::contourLandmarks()
{
  const int kNumberOfLandmarksForContour = 17;
  const double kAlphaStart = 0;
  const double kAlphaEnd = M_PI;
  const double kContourRadiusX = m_origin.x + m_size.width * 2 / 5;
  const double kContourRadiusY = m_origin.y + m_size.height * 2 / 5;
  const PointType kCenterPoint = m_origin + PointType(m_size.width / 2, m_size.height / 2);
  
  cv::Mat result = drawEllipse(kCenterPoint,
                               kAlphaStart,
                               kAlphaEnd,
                               kContourRadiusX,
                               kContourRadiusY,
                               kNumberOfLandmarksForContour,
                               true);
  
  return result;
}

const cv::Mat FaceFeature::drawEllipse(const PointType& center,
                                       const double alphaStart,
                                       const double alphaEnd,
                                       const double radiusX,
                                       const double radiusY,
                                       const int numberOfLandmarks,
                                       bool includeEdges)
{
  cv::Mat_<double> x(numberOfLandmarks, 1, CV_64FC1);
  cv::Mat_<double> y(numberOfLandmarks, 1, CV_64FC1);
  
  double alpha = alphaStart;
  double alphaStride = includeEdges ? (alphaEnd - alphaStart) / (numberOfLandmarks - 1)
                                    : (alphaEnd - alphaStart) / numberOfLandmarks;
  
  for (int i = 0; i < numberOfLandmarks; i++)
  {
    x(i, 0) = static_cast<double>(std::round(center.x + radiusX * std::cos(alpha)));
    y(i, 0) = static_cast<double>(std::round(center.y + radiusY * std::sin(alpha)));
    
    alpha += alphaStride;
  }
  
  cv::Mat result = MatHelper::matsToMat(x, y);
  return result;
}

void FaceFeature::PrintFacePosition(const cv::Mat& image, const cv::Mat& position)
{
  cv::Mat imageToDraw;
  image.copyTo(imageToDraw);
  
  const PointArrayPtr points = MatHelper::matToPoints(position);
  for (PointType point : *points)
  {
    cv::circle(imageToDraw, point, 1, cv::Scalar(255, 0, 0));
  }
  
  cv::rectangle(imageToDraw, cv::Rect(m_origin, m_size), cv::Scalar(0, 255, 0));

  cv::imshow("", imageToDraw);
  cv::waitKey();
}
