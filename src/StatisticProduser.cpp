//
//  StatisticProduser.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 25/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "StatisticProduser.h"
#include "CommonTypes.h"
#include "MatHelper.h"

#include <math.h>
#include <iterator>
#include <algorithm>


namespace statistics
{
  
  typedef cv::Point2d TPoint;
  typedef std::vector<TPoint> TPointArray;
  typedef std::shared_ptr<TPointArray> TPointArrayPtr;
  
  
  const cv::Mat pointsToMat(const TPointArrayPtr& points)
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
  
  const TPointArrayPtr matToPoints(const cv::Mat& mat)
  {
    CV_Assert(mat.cols == 1);
    CV_Assert(mat.rows % 2 == 0);
    CV_Assert(mat.depth() == CV_64FC1);
    
    TPointArrayPtr array = TPointArrayPtr(new TPointArray());
    
    TPoint pointToAdd;
    for (int i = 0; i < mat.rows; i += 2)
    {
      pointToAdd = {mat.at<double>(i, 0), mat.at<double>(i + 1, 0)};
      array->push_back(pointToAdd);
    }
    
    return array;
  }
  
  const double pointNorm(const TPoint& point)
  {
    return std::sqrt(point.x * point.x + point.y * point.y);
  }
  
  const cv::Mat matNorm(const cv::Mat& mat)
  {
    TPointArrayPtr points = matToPoints(mat);

    cv::Mat_<double> normMat(static_cast<int>(points->size()), 1, CV_64FC1);

    for (int i = 0; i < points->size(); i++)
    {
      normMat(i,0) = pointNorm(points->at(i));
    }

    return normMat;
  }
  
  const double StatisticProduser::AvgMistake(const cv::Mat& mat1, const cv::Mat& mat2)
  {
    CV_Assert(mat1.depth() == CV_64FC1);
    CV_Assert(mat2.depth() == CV_64FC1);
    CV_Assert(mat1.rows == mat2.rows);
    CV_Assert(mat1.cols == 1);
    CV_Assert(mat2.cols == 1);
    
    double result = AbsSumMistake(mat1, mat2) / mat1.rows;
    
    return result;
  }
  
  const cv::Mat StatisticProduser::AbsMistake(const cv::Mat& mat1, const cv::Mat& mat2)
  {
    CV_Assert(mat1.depth() == CV_64FC1);
    CV_Assert(mat2.depth() == CV_64FC1);
    CV_Assert(mat1.rows == mat2.rows);
    CV_Assert(mat1.cols == mat2.cols);
    CV_Assert(mat1.cols == 1);
    
    const cv::Mat norm = matNorm(mat1 - mat2);
    return norm;
  }
  
  const double StatisticProduser::AbsSumMistake(const cv::Mat& mat1, const cv::Mat& mat2)
  {
    CV_Assert(mat1.depth() == CV_64FC1);
    CV_Assert(mat2.depth() == CV_64FC1);
    CV_Assert(mat1.rows == mat2.rows);
    CV_Assert(mat1.cols == mat2.cols);
    CV_Assert(mat1.cols == 1);

    const cv::Mat norm = matNorm(mat1 - mat2);
    double result = cv::sum(norm)[0];
    return  result;
  }
  
  const cv::Mat StatisticProduser::Hist(const TMatContainerPtr& matContainer,
                                        const double min,
                                        const double max,
                                        const int number)
  {
    CV_Assert(matContainer->size() != 0);
    
    cv::Mat resultHist = Hist(matContainer->at(0), min, max, number);
    
    TMatContainer::const_iterator beginIter = matContainer->begin();
    for (TMatContainer::const_iterator iter = ++beginIter; iter != matContainer->end(); iter++)
    {
      resultHist += Hist(*iter, min, max, number);
    }
    
    return resultHist;
  }
  
  const cv::Mat StatisticProduser::Hist(const cv::Mat& mat,
                                        const double min,
                                        const double max,
                                        const int number)
  {
    CV_Assert(mat.depth() == CV_64FC1);
    CV_Assert(mat.rows != 0);
    CV_Assert(mat.cols == 1);

    const int numberOfBorders = number + 1;
    const double step = (max - min) / numberOfBorders;
    
    std::vector<double> histBorders;
    histBorders.reserve(numberOfBorders + 1);
    
    for (int i = 0; i < numberOfBorders; i++)
    {
      histBorders.push_back(min + i * step);
    }
    histBorders[numberOfBorders] = INT_MAX;
    
    cv::Mat_<int> histArray = cv::Mat::zeros(number + 1, 1, CV_32SC1);
    for (int matIter = 0; matIter < mat.rows; matIter++)
    {
      for (int i = 0; i < histBorders.size() - 1; i++)
      {
        if (mat.at<double>(matIter, 0) >= histBorders[i] &&
            mat.at<double>(matIter, 0) < histBorders[i + 1])
        {
          histArray(i, 0) += 1;
          break;
        }
      }
    }
    
    return histArray;
  }

  
  const double StatisticProduser::Cumsum(const cv::Mat& mat,
                                         const double min,
                                         const double max)
  {
    CV_Assert(mat.depth() == CV_32SC1);
    CV_Assert(mat.rows != 0);
    CV_Assert(mat.cols == 1);
    
    double result = 0;
    for (int i = min; i < MIN(mat.rows, max); i++)
    {
      result += mat.at<int>(i, 0);
    }
    
    return result;
  }
}

