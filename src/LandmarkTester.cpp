//
//  LandmarkTester.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 02/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "LandmarkTester.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include "LandmarkCollectionData.h"
#include "ImageLandmarkData.h"
#include "CascadeClassifier.h"


LandmarkTester::LandmarkTester(const LandmarkCollectionDataPtr& collection)
: m_collection(collection),
  m_cascadeClassifier(new CascadeClassifier)
{
}

const LandmarkCollectionDataPtr LandmarkTester::StartTest(bool manual)
{
  int iter = 0;
  while (iter < m_collection->CollectionSize())
  {
    const ImageLandmarkDataPtr landmarkData = m_collection->at(iter);
    
//    std::cout<<landmarkData->ImagePath()<<std::endl;
    std::cout<<iter<<std::endl;

    const cv::Rect faceRect = m_cascadeClassifier->DetectFace(landmarkData->ImageSource());
    if (faceRect.width == 0)
    {
      m_collection->DeleteLandmarkData(landmarkData);
      std::cout<<landmarkData->ImagePath()<<std::endl;
      std::cout<<"No face. Deleting"<<std::endl;
      continue;
    }
    if (!manual)
    {
      iter++;
      continue;
    }
    
    showImageWithLandmarks(landmarkData->ImageSource(), landmarkData->Landmarks(), faceRect);
    
    int key = cv::waitKey();
    if (key == 44)
    {
      if (iter > 0)
      {
        iter--;
      }
    }
    else if (key == 46)
    {
      if (iter < m_collection->CollectionSize())
      {
        iter++;
      }
    }
    else if (key == 32)
    {
      m_collection->DeleteLandmarkData(landmarkData);
    }
  }
  
  return m_collection;
}

void LandmarkTester::showImageWithLandmarks(const cv::Mat& image,
                                            const LandmarkTester::LandmarkArrayPtr& landmarks,
                                            const cv::Rect& faceRect)
{
  cv::Mat imageToShow = imageWithLandmarks(image, landmarks, faceRect);
  
//  cvtColor(imageToShow, imageToShow, CV_BGR2GRAY);
//  cv::equalizeHist(imageToShow, imageToShow);
//  cv::resize(imageToShow, imageToShow, cv::Size(), 2, 2);

  cv::imshow("Test", imageToShow);
}

const cv::Mat LandmarkTester::imageWithLandmarks(const cv::Mat& image,
                                                 const LandmarkTester::LandmarkArrayPtr& landmarks,
                                                 const cv::Rect& faceRect)
{
  cv::Mat newImage(image);
  
  cv::Scalar color = cv::Scalar(0,255,0);
  cv::rectangle(newImage, faceRect, cv::Scalar(0, 0, 255));

  for (const ImageLandmarkData::LandmarkType& landmark : *landmarks)
  {
    cv::circle(newImage, landmark, 2, color, 1);
  }
  
  return newImage;
}
