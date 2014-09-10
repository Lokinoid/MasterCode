//
//  ImageLandmarkData.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 22/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__ImageLandmarkData__
#define __ClassifierTrainer__ImageLandmarkData__


#include <opencv2/opencv.hpp>
#include "CommonTypes.h"

//@brief Model class that represents image with corresponding set of landmarks.
//
class ImageLandmarkData
{
public:
    
    typedef PointType LandmarkType;
    
    typedef PointArrayType LandmarkArray;
    typedef PointArrayPtr LandmarkArrayPtr;
    
    ImageLandmarkData(const std::string& imagePath = "",
                      const LandmarkArrayPtr& landmarks = LandmarkArrayPtr(new LandmarkArray));
    
    //@brief Returns path of image file.
    const std::string& ImagePath() const;
    
    //@breif Set file path for image.
    void SetImagePath(const std::string& imagePath);
    
    //@breif Returns size of image.
    const cv::Size ImageSize();
    
    //@brief Returns array of landmarks for current image.
    const LandmarkArrayPtr& Landmarks() const;
    
    //@brief Returns landmarks array as a single matrix (x_1, y_1, ... x_n, y_n)^T.
    const cv::Mat LandmarksMat();
    
    //@breif Returns size of landmarks array.
    const size_t LandmarksSize() const;
    
    //@brief Adds landmark to landmark array.
    void AddLandmark(const LandmarkType& landmark);
    
    //@brief Adds landmark array to landmark array.
    void AddLandmarkArray(const LandmarkArrayPtr& landmarks);
    
    //@brief Removes all landmarks from model data.
    void ClearLandmarks();
    
    //@brief Prints landmarks data.
    void PrintLandmarkData();
    
    virtual ~ImageLandmarkData();
    virtual const cv::Mat ImageSource();
    virtual void SetImageSource(const cv::Mat& imageSource, bool shouldSave);
    
protected:
    
    PathType m_imagePath;
    LandmarkArrayPtr m_landmarks;
    cv::Mat m_landmarksMat;
    cv::Size m_imageSize;
    
    //  cv::Mat m_imageSource;
};

#endif /* defined(__ClassifierTrainer__ImageLandmarkData__) */
