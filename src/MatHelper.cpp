//
//  MatHelper.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 28/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "MatHelper.h"
#include <vector>


const cv::Mat MatHelper::extPointToMat(const PointType& point)
{
  cv::Mat resultMat(3, 1, CV_64FC1);
  
  resultMat.at<double>(0, 0) = point.x;
  resultMat.at<double>(1, 0) = point.y;
  resultMat.at<double>(2, 0) = 1;
  
  return resultMat;
}

const PointType MatHelper::matToExtPoint(const cv::Mat& mat)
{
  return {static_cast<int>(mat.at<double>(0, 0)),
          static_cast<int>(mat.at<double>(1, 0))};
}

const cv::Mat MatHelper::pointsToMat(const PointArrayPtr& points)
{
  int numberOfRows = static_cast<int>(2 * points->size());
  cv::Mat resultMat(numberOfRows, 1, CV_64FC1);
  for (int i = 0; i < points->size(); i++)
  {
    resultMat.at<double>(2*i, 0) = points->at(i).x;
    resultMat.at<double>(2*i + 1, 0) = points->at(i).y;
  }
  
  return resultMat;
}

const cv::Mat MatHelper::pointToMat(const PointType& point)
{
  int numberOfRows = 2;
  cv::Mat resultMat(numberOfRows, 1, CV_64FC1);
  resultMat.at<double>(0, 0) = point.x;
  resultMat.at<double>(1, 0) = point.y;
  
  return resultMat;
}

const PointArrayPtr MatHelper::matToPoints(const cv::Mat& mat)
{
  CV_Assert(mat.cols == 1);
  CV_Assert(mat.rows % 2 == 0);
  CV_Assert(mat.depth() == CV_64FC1);
  
  PointArrayPtr array = PointArrayPtr(new PointArrayType());
  array->reserve(mat.rows / 2);
  
  PointType pointToAdd;
  for (int i = 0; i < mat.rows; i += 2)
  {
//    pointToAdd = {mat.at<double>(i, 0), mat.at<double>(i + 1, 0)};
    pointToAdd = {static_cast<int>(mat.at<double>(i, 0)), static_cast<int>(mat.at<double>(i + 1, 0))};
    array->push_back(pointToAdd);
  }
  
  return array;
}

const PointType MatHelper::matToPoint(const cv::Mat& mat)
{
  CV_Assert(mat.cols == 1);
  CV_Assert(mat.rows % 2 == 0);
  CV_Assert(mat.depth() == CV_64FC1);
  
  PointType point;
  point = {static_cast<int>(mat.at<double>(0, 0)),
           static_cast<int>(mat.at<double>(1, 0))};
  
  return point;
}

const cv::Mat MatHelper::matsToMat(const cv::Mat& matX, const cv::Mat& matY)
{
  CV_Assert(matX.depth() == matY.depth());
  CV_Assert(matX.depth() == CV_64FC1);
  CV_Assert(matX.rows == matY.rows);
  CV_Assert(matX.cols == 1);
  CV_Assert(matY.cols == 1);
  
  cv::Mat resultMat(2 * matX.rows, 1, matX.depth());
  for (int i = 0; i < matX.rows; i++)
  {
    resultMat.at<double>(2*i, 0) = matX.at<double>(i, 0);
    resultMat.at<double>(2*i + 1, 0) = matY.at<double>(i, 0);
  }
  
  return resultMat;
}

const cv::Rect MatHelper::rectAtPoint(const PointType& point,
                                      const cv::Size& imageSize,
                                      const cv::Size& windowSize)
{
  const int kMaxX = imageSize.width - 1 - windowSize.width;
  const int kMaxY = imageSize.height - 1 - windowSize.height;
  
  PointType shiftedPoint = {point.x - static_cast<int>(windowSize.width / 2),
                            point.y - static_cast<int>(windowSize.height / 2)};
  PointType correctPoint = {MIN(MAX(0, shiftedPoint.x), kMaxX),
                            MIN(MAX(0, shiftedPoint.y), kMaxY)};
  
  cv::Rect rect(correctPoint, windowSize);
  
  return rect;
}
