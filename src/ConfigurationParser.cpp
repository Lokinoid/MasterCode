//
//  ConfigurationParser.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 22/06/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ConfigurationParser.h"

#include <opencv2/opencv.hpp>
#include <vector>

#include "ConfigurationKeyPaths.h"
#include "DescriptorConfigHOG.h"
#include "CheckQualityConfig.h"
#include "ExternalSolverConfig.h"
#include "DetectorConfig.h"
#include "TrackerConfig.h"


std::shared_ptr<CurrentConfiguration> ConfigurationParser::LoadConfiguration(const std::string& configurationFilePath)
{
    ConfigurationParser parser(configurationFilePath);
    std::shared_ptr<CurrentConfiguration> configuration = parser.parse();
    
    return configuration;
}

ConfigurationParser::ConfigurationParser(const std::string& configurationFilePath)
: m_filePath(configurationFilePath)
{
}

std::shared_ptr<CurrentConfiguration> ConfigurationParser::parse()
{
    // single parameters
    const std::vector<const std::string> keyPaths = {config::kImageDirectoryKeyPath,
                                                     config::kLandmarkDataFileNameKeyPath};
    
    TCurrentConfiguration configuration = TCurrentConfiguration(new CurrentConfiguration());
    
    cv::FileStorage storage(m_filePath, cv::FileStorage::READ);
    if (!storage.isOpened())
    {
        std::cout<<"Cannot open file to read: "<< m_filePath <<std::endl;
        CV_Assert(0);
    }
    
    for (const std::string keyPath : keyPaths)
    {
        std::string* parameterValue = new std::string();
        storage[keyPath] >> *parameterValue;
        if (parameterValue->length() == 0)
        {
            std::cout << "Empty value for key path" << keyPath << std::endl;
            break;
        }
        
        configuration->storage[keyPath] = new ConfigurationElement<std::string>(parameterValue);
    }
    
    parseInstance<CheckQualityConfig>(config::kCheckQualityParameterKeyPath, storage, configuration);     // check quality
    parseInstance<DetectorConfig>(config::kDetectorParametersKeyPath, storage, configuration);            // detector
    parseInstance<TrackerConfig>(config::kTrackerParametersKeyPath, storage, configuration);              // tracker
    parseInstance<ExternalSolverConfig>(config::kExternalSolverParameterKeyPath, storage, configuration); // external solver
    
    storage.release();
    
    return configuration;
}

template <class _Ty> void ConfigurationParser::parseInstance(const std::string& parameterKeyPath,
                                                             const cv::FileStorage& storage,
                                                             TCurrentConfiguration& configuration)
{
    _Ty* parameterInstance = new _Ty;
    parameterInstance->ParseConfiguration(storage[parameterKeyPath]);
    configuration->storage[parameterKeyPath] = new ConfigurationElement<_Ty>(parameterInstance);
}
