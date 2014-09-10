//
//  ConfigurationManager.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 21/06/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ConfigurationManager.h"
#include "ConfigurationKeyPaths.h"
#include "ConfigurationParser.h"
#include "CheckQualityConfig.h"
#include "ExternalSolverConfig.h"
#include "DetectorConfig.h"
#include "TrackerConfig.h"


static ConfigurationManager* sharedInstance = NULL;


template <class _Ty> const _Ty* ConfigurationManager::generateConfigurationParameter(const std::string& parameterKeyPath)
{
    CurrentConfigurationPtr currentConfiguration = ConfigurationManager::getSharedInstance()->getCurrentConfiguration();
    const _Ty* parameter = dynamic_cast<const ConfigurationElement<_Ty>*>(currentConfiguration->storage.at(parameterKeyPath))->Element();

    return parameter;
}

void ConfigurationManager::ResetWithConfigurationFile(const std::string& configFilePath)
{
    delete sharedInstance;
    sharedInstance = new ConfigurationManager(configFilePath);
}

const std::string ConfigurationManager::TrainImageDirectory()
{
    return *generateConfigurationParameter<std::string>(config::kImageDirectoryKeyPath);
}

const std::string ConfigurationManager::LandmarkDataFilePath()
{
    return *generateConfigurationParameter<std::string>(config::kLandmarkDataFileNameKeyPath);
}

const DetectorConfig* ConfigurationManager::DetectorConfiguration()
{
    return generateConfigurationParameter<DetectorConfig>(config::kDetectorParametersKeyPath);
}

const TrackerConfig* ConfigurationManager::TrackerConfiguration()
{
    return generateConfigurationParameter<TrackerConfig>(config::kTrackerParametersKeyPath);
}

const CheckQualityConfig* ConfigurationManager::CheckQualityConfiguration()
{
    return generateConfigurationParameter<CheckQualityConfig>(config::kCheckQualityParameterKeyPath);
}

const ExternalSolverConfig* ConfigurationManager::ExternalSolverConfiguration()
{
    return generateConfigurationParameter<ExternalSolverConfig>(config::kExternalSolverParameterKeyPath);
}

ConfigurationManager* ConfigurationManager::getSharedInstance()
{
    return sharedInstance;
}

ConfigurationManager::ConfigurationManager(const std::string& configFilePath)
: m_configFilePath(configFilePath)
{
    m_configuration = parseConfigurationFile();
}

ConfigurationManager::CurrentConfigurationPtr ConfigurationManager::getCurrentConfiguration()
{
    return m_configuration;
}

ConfigurationManager::CurrentConfigurationPtr ConfigurationManager::parseConfigurationFile()
{
    return ConfigurationParser::LoadConfiguration(m_configFilePath);
}

void ConfigurationManager::ShowCurrentConfiguration()
{
    std::cout<<"Config loaded:"
    << "\nImage directory: " << TrainImageDirectory()
    << "\nLandmark data file path: " << LandmarkDataFilePath()
    << "\nCheck landmark quality data: " << CheckQualityConfiguration()
    << "\nDetector data: " << DetectorConfiguration()
    << "\nTracker data: " << TrackerConfiguration()
    << "\nExternal solver data: " << ExternalSolverConfiguration();
}
