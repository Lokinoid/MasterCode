//
//  MainController.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 14/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__MainController__
#define __ClassifierTrainer__MainController__

#include <iostream>
#include "CommonTypes.h"

class IFeature;
class ConfigurationManager;
class DetectorConfig;
class TrackerConfig;

//@brief Class that manages all the work.
//
class MainController
{
public:
    
    typedef enum
    {
        ProcessTypeTrain,
        ProcessTypeTest,
        
    } ProcessType;
    
// @brief Starts business.
// @param configurationFilePath.
//        Path of configuration file.
    void StartWithConfigurationFilePath(const std::string& configurationFilePath);
    
private:

    void trainFaceDetectorWithConfiguration(const DetectorConfig* config, const LandmarkCollectionDataPtr& landmarkData);
    void trainFaceTrackerWithConfiguration(const TrackerConfig* config, const LandmarkCollectionDataPtr& landmarkData);
    
    const LandmarkCollectionDataPtr preprocessCollection(const std::string& imageDirectory,
                                                         const std::string& landmarkDataFilePath,
                                                         const std::string& imageDirectoryToSave,
                                                         const std::string& landmarkDataFilePathToSave);

    const LandmarkCollectionDataPtr trackerTrainingData();
    const LandmarkCollectionDataPtr createDetectedDataSet();
    
    void testClassifier();
    
    const LandmarkCollectionDataPtr landmarkDataForFeature(FeatureType featureType);
    const LandmarkCollectionDataPtr devideCollection(const LandmarkCollectionDataPtr& landmarkData);

    const LandmarkCollectionDataPtr loadFaceLandmarkData(const std::string& imageDirectory,
                                                         const std::string& collectionDataFilePath);
    const LandmarkCollectionDataPtr transformDataToTheSameFaceSize(const LandmarkCollectionDataPtr& landmarkData,
                                                                   const std::string& imageDirectoryPath);
    
    const LandmarkCollectionDataPtr getStarLandmarkData();
    const LandmarkCollectionDataPtr filterAndPersistLandmarkData(const LandmarkCollectionDataPtr& detectorData);
    
    const std::string directoryForFeature(FeatureType featureType);
};

#endif /* defined(__ClassifierTrainer__MainController__) */
