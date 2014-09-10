//
//  LandmarkTester.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 02/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__LandmarkTester__
#define __ClassifierTrainer__LandmarkTester__


#include <memory>
#include <opencv2/opencv.hpp>
#include "LandmarkCollectionData.h"
#include "CommonTypes.h"


class CascadeClassifier;

//@brief Class to test landmarks for each image.
//       Show images with landmarks on it.
//       Used to test how landmarks fit particular image.
class LandmarkTester
{
public:

// @brief Starts testing.
// @param collection.
//        Collection to test.
    LandmarkTester(const LandmarkCollectionDataPtr& collection);
    
// @brief Starts testing.
// @param manual.
//        If true then user can navigate between images by himself and
//        delete any image with corresponding landmark information.
//        Usefull with
//        If false then tester goes through all images with landmarks and deletes images with landmarks
//        that are not inside of rectangular found by Viola-Jones face detector.
// @return New set of images with landmarks.
    const LandmarkCollectionDataPtr StartTest(bool manual);
    
private:
    
    typedef LandmarkCollectionData::LandmarkArrayPtr LandmarkArrayPtr;
    
    void showImageWithLandmarks(const cv::Mat& image,
                                const LandmarkArrayPtr& landmarks,
                                const cv::Rect& faceRect);
    
    const cv::Mat imageWithLandmarks(const cv::Mat& image,
                                     const LandmarkArrayPtr& landmarks,
                                     const cv::Rect& faceRect);
    LandmarkCollectionDataPtr m_collection;
    
    typedef std::shared_ptr<CascadeClassifier> CascadeClassifierPtr;
    CascadeClassifierPtr m_cascadeClassifier;
};
#endif /* defined(__ClassifierTrainer__LandmarkTester__) */
