//
//  ClassifierTrainer.h
//  ClassifierTrainer
//
//  Created by Stanislav on 23.02.14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__ClassifierTrainer__
#define __ClassifierTrainer__ClassifierTrainer__

#include "LandmarkCollectionData.h"
#include <opencv2/opencv.hpp>
#include "CommonTypes.h"
//#include "IDescriptor.h"


class IFeature;
class EquationData;

//@brief Class to train detector and tracker.
//       Detector's input is an image and output is set of landmarks.
//       Tracker's input is an image & set of previous detected landmarks.
//       Tracker's output is set of new landmarks that fit the input image.
//       After training class creates file where persists detector/tracker information.
//
class ClassifierTrainer
{
public:
        
    //@brief Function to train detector.
    //@param collectionData
    //       Array of images and corresponding to each image landmarks labelled manually (ideal).
    //@param feature
    //       Type of feature (face by default).
    //@param descriptor
    //       Type of descriptor.
    //@param numberOfTrainIterations
    //       Number of iterations.
    //@param detectorDataFilePath
    //       File path for trained detector data (should be used .xml or .yml).
    //@param shouldLoadMatrixNotSolve
    //       Indicates whether matrixes should be solved within OpenCV or wait for external solvation.
    //       Nonoptimized OpenCV solves them very slowly...
    void TrainDetector(const LandmarkCollectionDataPtr& collectionData,
                       const FeaturePtr& feature,
                       const DescriptorPtr& descriptor,
                       const int numberOfTrainIterations,
                       const std::string& detectorDataFilePath,
                       bool shouldLoadMatrixNotSolve = false);
    
    //@brief Function to train tracker.
    //@param collectionData
    //       Array of images and corresponding to each image landmarks labelled manually (ideal).
    //@param detectedData
    //       Array of images and corresponding to each image landmarks labelled by detector/tracker.
    //@param feature
    //       Type of feature (face by default).
    //@param descriptor
    //       Type of descriptor.
    //@param numberOfTrainIterations
    //       Number of iterations.
    //@param trackerDataFilePath
    //       File path for trained detector data (should be used .xml or .yml).
    //@param shouldLoadMatrixNotSolve
    //       Indicates whether matrixes should be solved within OpenCV or wait for external solvation.
    //       Nonoptimized OpenCV solves them very slowly...
    void TrainTracker(const LandmarkCollectionDataPtr& collectionData,
                      const LandmarkCollectionDataPtr& detectedData,
                      const FeaturePtr& feature,
                      const DescriptorPtr& descriptor,
                      const int numberOfTrainIterations,
                      const std::string& trackerDataFilePath,
                      bool shouldLoadMatrixNotSolve = false);
    
private:
    
    typedef std::vector<cv::Mat> PositionArrayType;
    typedef std::shared_ptr<PositionArrayType> PositionArrayPtr;
    
    LandmarkCollectionDataPtr m_collectionData;
    FeaturePtr m_feature;
    DescriptorPtr m_descriptor;
    
    bool m_shouldLoadMatrixNotSolve;
    bool m_isTracker;
    
    const FeaturePtr featureWithType(FeatureType type);
    const EquationArrayPtr createEquationArray(const LandmarkCollectionDataPtr& collectionData);
    const cv::Mat checkAndConvertPosition(const cv::Mat& position,
                                          const cv::Size& imageSize);
    
    const cv::Mat solveEquationArray(const EquationArrayPtr& equationArray);
    void updateEquationWithCurrentPosition(EquationArrayPtr& equationArray,
                                           const PositionArrayPtr& currentPositions);
    //  void persistData(const cv::Mat& mat);
    
    const PositionArrayPtr nextPosition(EquationArrayPtr& equationArray,
                                        const cv::Mat& mat);
    const PositionArrayPtr startPositionArray(const size_t size);
    const PositionArrayPtr detectedPositionArray(const LandmarkCollectionDataPtr& detectedData);
    
    void printEquationArray(const EquationArrayPtr& equationArray);
    void trainClassifier(const LandmarkCollectionDataPtr& collectionData,
                         const LandmarkCollectionDataPtr& detectedData,
                         const FeaturePtr& feature,
                         const DescriptorPtr& descriptor,
                         const int numberOfTrainIterations,
                         const std::string& savedTrainedDataFilePath,
                         bool shouldLoadMatrixNotSolve = false);
};

#endif /* defined(__ClassifierTrainer__ClassifierTrainer__) */
