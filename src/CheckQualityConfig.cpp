//
//  CheckQualityConfig.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/20/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "CheckQualityConfig.h"
#include "ConfigurationKeyPaths.h"
#include "ConfigurationElement.h"


bool CheckQualityConfig::IsManual() const
{
    return dynamic_cast<const ConfigurationElement<bool>*>(m_parameterStorage->at(config::kManualKeyPath))->Element();
}

const std::string CheckQualityConfig::LandmarkDataFilePath() const
{
    return *generateParameter<std::string>(config::kCheckQualityNewLandmarkDataFileNameKeyPath);
}

void CheckQualityConfig::ParseConfiguration(const cv::FileNode& node)
{
    ConfigBase::ParseConfiguration(node);
    
    std::string isManual;
    node[config::kManualKeyPath] >> isManual;
    (*m_parameterStorage)[config::kManualKeyPath] = new ConfigurationElement<bool>(stringToBool(isManual));
    
    std::string* parameter = new std::string();
    node[config::kCheckQualityNewLandmarkDataFileNameKeyPath] >> *parameter;
    (*m_parameterStorage)[config::kCheckQualityNewLandmarkDataFileNameKeyPath] = new ConfigurationElement<std::string>(parameter);
}

void CheckQualityConfig::Print() const
{
    std::cout << "\nEnabled: " << IsEnabled()
              << "\nManual: " << IsManual()
              << "\nLandmark data file path: " << LandmarkDataFilePath()
    << std::endl;
}
