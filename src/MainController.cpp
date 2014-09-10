//
//  MainController.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 14/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "MainController.h"

#include <boost/assert.hpp>

#include "ConfigurationManager.h"
#include "DescriptorConfigurator.h"
#include "CheckQualityConfig.h"
#include "DetectorConfig.h"
#include "TrackerConfig.h"

#include "ImagePicker.h"
#include "ImageCollectionMultiplier.h"
#include "ClassifierTrainer.h"

#include "LandmarkTester.h"
#include "ClassifierTester.h"
#include "TrainingTest.h"
#include "FileDataPersister.h"
#include "StarFeature.h"
#include "FaceFeature.h"
#include "TestFunctionFeature.h"
#include "ImageCollectionDevider.h"
#include "ImageTransformer.h"
#include "CascadeClassifier.h"
#include "Detector.h"
#include "MatHelper.h"
#include "HOGDescriptor.h"
#include "SIFTDescriptor.h"


void MainController::StartWithConfigurationFilePath(const std::string& configurationFilePath)
{
    // create configuration manager to parse the configuration
    ConfigurationManager::ResetWithConfigurationFile(configurationFilePath);
    ConfigurationManager::ShowCurrentConfiguration();
    
    // get training data
    LandmarkCollectionDataPtr landmarkData = loadFaceLandmarkData(ConfigurationManager::TrainImageDirectory(),
                                                                  ConfigurationManager::LandmarkDataFilePath());
    
    std::cout<<"Collection size "<<landmarkData->CollectionSize()<<std::endl;
    
    // check landmark data if needed
    if (ConfigurationManager::CheckQualityConfiguration()->IsEnabled())
    {
        landmarkData = filterAndPersistLandmarkData(landmarkData);
    }
    
    // train detector if needed
    if (ConfigurationManager::DetectorConfiguration()->IsEnabled())
    {
        trainFaceDetectorWithConfiguration(ConfigurationManager::DetectorConfiguration(), landmarkData);
    }
    
    // train tracker if needed
    if (ConfigurationManager::TrackerConfiguration()->IsEnabled())
    {
        trainFaceTrackerWithConfiguration(ConfigurationManager::TrackerConfiguration(), landmarkData);
    }
}

void MainController::trainFaceDetectorWithConfiguration(const DetectorConfig *config, const LandmarkCollectionDataPtr& landmarkData)
{
    FeaturePtr feature(new FaceFeature);
    DescriptorPtr descriptor = DescriptorConfigurator::DescriptorWithConfig(config->DescriptorConfiguration());
    
    bool shouldLoadMatrixNotSolve = true;
    ClassifierTrainer trainer;
    trainer.TrainDetector(landmarkData,
                          feature,
                          descriptor,
                          config->NumberOfIterations(),
                          config->DetectorDataFilePath(),
                          shouldLoadMatrixNotSolve);
    
}

void MainController::trainFaceTrackerWithConfiguration(const TrackerConfig *config, const LandmarkCollectionDataPtr& landmarkData)
{
    const LandmarkCollectionDataPtr detectedData = trackerTrainingData();
    std::cout << "Tracker collection size " << detectedData->CollectionSize() << std::endl;
    
    FeaturePtr feature(new FaceFeature);
    DescriptorPtr descriptor = DescriptorConfigurator::DescriptorWithConfig(config->DescriptorConfiguration());
    
    bool shouldLoadMatrixNotSolve = true;
    ClassifierTrainer trainer;
    trainer.TrainTracker(landmarkData,
                         detectedData,
                         feature,
                         descriptor,
                         config->NumberOfIterations(),
                         config->TrackerDataFilePath(),
                         shouldLoadMatrixNotSolve);
}

const LandmarkCollectionDataPtr MainController::trackerTrainingData()
{
    const std::string detectedDataFilePath = ConfigurationManager::TrackerConfiguration()->DetectedLandmarkDataFilePath();
    std::cout << "Try to load detected landmark data set: " << detectedDataFilePath << std::endl;
    LandmarkCollectionDataPtr detectedData = loadFaceLandmarkData(ConfigurationManager::TrainImageDirectory(),
                                                                  detectedDataFilePath);
    if (!detectedData || detectedData->CollectionSize() == 0)
    {
        std::cout << "Detected landmark data set does not exist, creating..." << std::endl;
        
        detectedData = createDetectedDataSet();
    }
    return detectedData;
}

const LandmarkCollectionDataPtr MainController::createDetectedDataSet()
{
    const std::string trackerLandmarkDataFilePath = ConfigurationManager::TrackerConfiguration()->DetectedLandmarkDataFilePath();
    LandmarkCollectionDataPtr landmarkData = loadFaceLandmarkData(ConfigurationManager::TrainImageDirectory(),
                                                                  ConfigurationManager::LandmarkDataFilePath());
    LandmarkCollectionDataPtr detectedData(new LandmarkCollectionData(landmarkData->CollectionSize()));
    
    Detector detector(ConfigurationManager::DetectorConfiguration()->DetectorDataFilePath());
    landmarkData->EnumerateConstColectionWithCallback([&] (const ImageLandmarkDataPtr& landmarkData, const int index, bool* stop) {
        
        std::cout<<index<<std::endl;
        const cv::Mat landmarks = detector.Detect(landmarkData->ImageSource());
        const PointArrayPtr points = MatHelper::matToPoints(landmarks);
        ImageLandmarkDataPtr newLandmarkData(new ImageLandmarkData(landmarkData->ImagePath(), points));
        detectedData->AddImageLandmarkData(newLandmarkData);
    });
    
    std::cout<<"Persisting to file: " << trackerLandmarkDataFilePath << std::endl;
    FileDataPersister persister;
    persister.PersistData(trackerLandmarkDataFilePath, detectedData);
    
    return detectedData;
}

void MainController::testClassifier()
{
    FeaturePtr feature = FeaturePtr(new FaceFeature);
    LandmarkCollectionDataPtr data = loadFaceLandmarkData(ConfigurationManager::TrainImageDirectory(),
                                                          ConfigurationManager::LandmarkDataFilePath());
    
    std::cout<<"Landmark data loaded, size = " << data->CollectionSize()<<std::endl;
    
    ClassifierTester tester;
    tester.StartTest(data, ConfigurationManager::DetectorConfiguration()->DetectorDataFilePath(), INT_MAX, true);
}

const LandmarkCollectionDataPtr MainController::preprocessCollection(const std::string& imageDirectory,
                                                                     const std::string& landmarkDataFilePath,
                                                                     const std::string& imageDirectoryToSave,
                                                                     const std::string& landmarkDataFilePathToSave)
{
    LandmarkCollectionDataPtr landmarkData = loadFaceLandmarkData(imageDirectory,
                                                                  landmarkDataFilePath);
    
    const std::string otherImageDirectory = "";
    const std::string otherLandmarkDataFilePath = "";
    LandmarkCollectionDataPtr landmarkData2 = loadFaceLandmarkData(otherImageDirectory,
                                                                   otherLandmarkDataFilePath);
    
    // If it's nedded to fetch several collections, merge them into one and persist
    landmarkData->AddCollection(landmarkData2);
    
    // If it's needed to fit all images to the same face size
    landmarkData = transformDataToTheSameFaceSize(landmarkData, imageDirectoryToSave);
    
    FileDataPersister persister;
    persister.PersistData(landmarkDataFilePathToSave, landmarkData);

    // If it's needed to devide data into train and test sets
    landmarkData = devideCollection(landmarkData);
    
    return landmarkData;
}

const LandmarkCollectionDataPtr MainController::loadFaceLandmarkData(const std::string& imageDirectory,
                                                                     const std::string& collectionDataFilePath)
{
    std::cout<<"Try to load landmark data from "<<collectionDataFilePath << std::endl;
    ImagePicker picker(imageDirectory);
    LandmarkCollectionDataPtr data = picker.FetchLandmarkCollectionDataFromFile(collectionDataFilePath);
    
    return data;
}

const LandmarkCollectionDataPtr MainController::transformDataToTheSameFaceSize(const LandmarkCollectionDataPtr& collectionData,
                                                                               const std::string& imageDirectoryPath)
{
    ImageTransformer transformer(imageDirectoryPath);
    const LandmarkCollectionDataPtr newCollectionData = transformer.TransformCollectionUsingFaceCrop(collectionData);
    
    return newCollectionData;
}

const LandmarkCollectionDataPtr MainController::devideCollection(const LandmarkCollectionDataPtr& landmarkData)
{
    CollectionData entireData(landmarkData, "");

	const std::string testCollectionDataFilePath = ""; // need to paste the path of collection data file path (file with landmarks data)
	CollectionData testData(NULL, testCollectionDataFilePath); 

	const std::string trainCollectionDataFilePath = ""; // need to paste user's path
    CollectionData trainData(NULL, trainCollectionDataFilePath);
    
    ImageCollectionDevider::DevideCollection(entireData, testData, trainData);
    
    FileDataPersister persister;
    persister.PersistData(testData.collectionDataFilePath, testData.collection);
    persister.PersistData(trainData.collectionDataFilePath, trainData.collection);
    
    return trainData.collection;
}

const LandmarkCollectionDataPtr MainController::filterAndPersistLandmarkData(const LandmarkCollectionDataPtr& detectorData)
{
    // filteres landmark positions per image and deletes if they don't suite (for more information see comments about LandmarkTester class)
    LandmarkTester tester(detectorData);
    bool manualManagement = ConfigurationManager::CheckQualityConfiguration()->IsManual();
    LandmarkCollectionDataPtr filteredDetectorData = tester.StartTest(manualManagement);
    std::cout<<"Collection size "<<detectorData->CollectionSize()<<std::endl;
    
    // saves new set of images and landmarks
    const std::string filePath = ConfigurationManager::CheckQualityConfiguration()->LandmarkDataFilePath();
    FileDataPersister persister;
    persister.PersistData(filePath, filteredDetectorData);
    
    return filteredDetectorData;
}
