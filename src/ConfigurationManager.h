//
//  ConfigurationManager.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 21/06/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__ConfigurationManager__
#define __ClassifierTrainer__ConfigurationManager__

#include <string>
#include <memory>
#include "ConfigurationElement.h"


class DetectorConfig;
class TrackerConfig;
class CheckQualityConfig;
class ExternalSolverConfig;

//@brief Singleton class to manage configurations.
//       Each configuration is represented by configuration file (eg config.yml).
//
class ConfigurationManager
{
public:
    
    //@brief Resets manager with new configuration.
    //@param configFile
    //       File path for configuration file. Should be .yml or .xml only.
    static void ResetWithConfigurationFile(const std::string& configFilePath);
    
    //@brief Gets path of directory with images.
    static const std::string TrainImageDirectory();
    
    //@brief Gets path of file with landmarks for each image.
    static const std::string LandmarkDataFilePath();
        
    //@brief Gets detector configuration.
    static const DetectorConfig* DetectorConfiguration();

    //@brief Gets tracker configuration.
    static const TrackerConfig* TrackerConfiguration();
    
    //@brief Gets check quality configuration.
    static const CheckQualityConfig* CheckQualityConfiguration();
    
    //@brief Gets external solver configuration.
    static const ExternalSolverConfig* ExternalSolverConfiguration();
    
    //@brief Outputs read configuration.
    static void ShowCurrentConfiguration();
    
    
private:
    
    ConfigurationManager(const std::string& configFilePath);

    static ConfigurationManager* getSharedInstance();
    template <class _Ty> static const _Ty* generateConfigurationParameter(const std::string& parameterKeyPath);
    
    typedef std::shared_ptr<CurrentConfiguration> CurrentConfigurationPtr;
    CurrentConfigurationPtr parseConfigurationFile();
    CurrentConfigurationPtr getCurrentConfiguration();
    
    CurrentConfigurationPtr m_configuration;
    const std::string m_configFilePath;
};

#endif /* defined(__ClassifierTrainer__ConfigurationManager__) */
