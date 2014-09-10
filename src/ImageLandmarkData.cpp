//
//  ImageLandmarkData.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 22/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ImageLandmarkData.h"

#include <string>
#include <list>
#include "ImageWriter.h"
#include "MatHelper.h"


ImageLandmarkData::ImageLandmarkData(const PathType& imagePath,
                                     const LandmarkArrayPtr& landmarks)
: m_imagePath(imagePath),
  m_landmarks(landmarks),
  m_imageSize(cv::Size(0,0))
{
}

ImageLandmarkData::~ImageLandmarkData()
{
}

const std::string& ImageLandmarkData::ImagePath() const
{
    return m_imagePath;
}

void ImageLandmarkData::SetImagePath(const PathType& imagePath)
{
    m_imagePath = imagePath;
}

const cv::Size ImageLandmarkData::ImageSize()
{
    if (m_imageSize.width == 0)
    {
        cv::Mat image = ImageSource();
        CV_Assert(!image.empty());
        m_imageSize = image.size();
    }
    
    return m_imageSize;
}

const ImageLandmarkData::LandmarkArrayPtr& ImageLandmarkData::Landmarks() const
{
    return m_landmarks;
}

const cv::Mat ImageLandmarkData::LandmarksMat()
{
    if (m_landmarksMat.empty())
    {
        m_landmarksMat = MatHelper::pointsToMat(m_landmarks) ;
    }
    
    return m_landmarksMat;
}

const size_t ImageLandmarkData::LandmarksSize() const
{
    return m_landmarks->size();
}

void ImageLandmarkData::AddLandmark(const LandmarkType& landmark)
{
    m_landmarks->push_back(landmark);
}

void ImageLandmarkData::AddLandmarkArray(const LandmarkArrayPtr& landmarks)
{
    std::copy_if(landmarks->cbegin(), landmarks->cend(), std::back_inserter(*m_landmarks), [&](const LandmarkType& landmark)
                 {
                     return std::find(m_landmarks->cbegin(), m_landmarks->cend(), landmark) == m_landmarks->cend();
                 });
}

const cv::Mat ImageLandmarkData::ImageSource()
{
    //  if (m_imageSource.empty())
    //  {
    //    m_imageSource = cv::imread(m_imagePath);
    //    CV_Assert(!m_imagePath.empty());
    //  }
    //
    //  return(m_imageSource);
    
    cv::Mat image = cv::imread(m_imagePath);
    if (image.empty())
    {
        std::cout<<"Image does not exist "<<m_imagePath<<std::endl;
    }
    
    m_imageSize = image.size();
    return image;
}

void ImageLandmarkData::SetImageSource(const cv::Mat& imageSource, bool shouldSave)
{
    //  if (m_imageSource.empty())
    //  {
    //    m_imageSource = imageSource;
    //  }
    //  if (shouldSave)
    //  {
    //    ImageWriter* writer = ImageWriter::sharedInstance();
    //    writer->SaveImageWithPath(m_imagePath, m_imageSource);
    //  }
    
    CV_Assert(!imageSource.empty());
    
    m_imageSize = imageSource.size();
    if (shouldSave)
    {
        std::cout<<"Write new image: " << m_imagePath<<std::endl;
        ImageWriter* writer = ImageWriter::sharedInstance();
        writer->SaveImageWithPath(m_imagePath, imageSource);
    }
}

void ImageLandmarkData::PrintLandmarkData()
{
    std::cout<<m_imagePath<<std::endl;
    std::cout<<"Landmarks: "<<std::endl;
    std::for_each(m_landmarks->begin(), m_landmarks->end(), [&] (const LandmarkType& landmark) {
        
        std::cout<<landmark.x <<" "<<landmark.y << std::endl;
    });
    std::cout<<"Landmarks mat: "<<std::endl;
    std::cout<<LandmarksMat()<<std::endl;
}

void ImageLandmarkData::ClearLandmarks()
{
    m_landmarks->clear();
}
