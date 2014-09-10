//
//  TrackerConfig.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/25/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "TrackerConfig.h"
#include "ConfigurationKeyPaths.h"
#include "ConfigurationElement.h"


void TrackerConfig::ParseConfiguration(const cv::FileNode& node)
{
    DetectorConfig::ParseConfiguration(node);
    
    std::string* trackerDataFilePath = new std::string();
    node[config::kTrackerDataFileNameKeyPath] >> *trackerDataFilePath;
    (*m_parameterStorage)[config::kTrackerDataFileNameKeyPath] = new ConfigurationElement<std::string>(trackerDataFilePath);
    
    std::string* detectedLandmarkDataFilePath = new std::string();
    node[config::kTrackerDetectedLandmarksDataFilePath] >> *detectedLandmarkDataFilePath;
    (*m_parameterStorage)[config::kTrackerDetectedLandmarksDataFilePath] = new ConfigurationElement<std::string>(detectedLandmarkDataFilePath);
}

const std::string TrackerConfig::TrackerDataFilePath() const
{
    return *generateParameter<std::string>(config::kTrackerDataFileNameKeyPath);
}

const std::string TrackerConfig::DetectedLandmarkDataFilePath() const
{
    return *generateParameter<std::string>(config::kTrackerDetectedLandmarksDataFilePath);
}

void TrackerConfig::Print()
{
    DetectorConfig::Print();
    std::cout << "\nTracker data file path: " << TrackerDataFilePath()
              << "\nDetected landmark data file path: " << DetectedLandmarkDataFilePath()
    << std::endl;
}
