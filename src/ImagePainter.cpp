//
//  ImagePainter.cpp
//  SDMOpenCVClient
//
//  Created by Stanislav Orlov on 21/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ImagePainter.h"
#include "CommonTypes.h"
#include "MatHelper.h"


const cv::Mat ImagePainter::DrawLandmarks(const cv::Mat* image, const cv::Mat* landmarks)
{
  if (!image ||
      image->empty() ||
      !landmarks ||
      landmarks->empty())
  {
    return *image;
  }
  
  const cv::Mat resultImage = addLandmarksOnImage(image, landmarks, 2);
  return resultImage;
}

const cv::Mat ImagePainter::addLandmarksOnImage(const cv::Mat* image,
                                                const cv::Mat* position,
                                                const int thickness)
{  
  cv::Mat newImage;
  image->copyTo(newImage);
  
  PointArrayPtr landmarks = MatHelper::matToPoints(*position);
  for (const PointType point : * landmarks)
  {
    cv::circle(newImage, point, 1, cv::Scalar(255,0,0), thickness);
  }
  
  return newImage;
}
