//
//  DetectorConfig.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/22/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "DetectorConfig.h"
#include "ConfigurationKeyPaths.h"
#include "ConfigurationElement.h"

#include "DescriptorConfig.h"
#include "DescriptorConfigurationFactory.h"



const bool DetectorConfig::IsEnabled() const
{
    return dynamic_cast<const ConfigurationElement<bool>*>(m_parameterStorage->at(config::kEnabledKeyPath))->Element();
}

const std::string DetectorConfig::DetectorDataFilePath() const
{
    return *generateParameter<std::string>(config::kDetectorDataFileNameKeyPath);
}

const int DetectorConfig::NumberOfIterations() const
{
    return *generateParameter<int>(config::kNumberOfTrainIterationsKeyPath);
}

const IDescriptorConfig* DetectorConfig::DescriptorConfiguration() const
{
    return generateParameter<IDescriptorConfig>(config::kDescriptorParametersKeyPath);
}

void DetectorConfig::ParseConfiguration(const cv::FileNode& node)
{
    ConfigBase::ParseConfiguration(node);
    
    std::string* parameter = new std::string();
    node[config::kDetectorDataFileNameKeyPath] >> *parameter;
    (*m_parameterStorage)[config::kDetectorDataFileNameKeyPath] = new ConfigurationElement<std::string>(parameter);
    
    IDescriptorConfig* descriptor = DescriptorConfigurationFactory::DescriptorConfigInstanceForDescriptorNode(node[config::kDescriptorParametersKeyPath]);
    (*m_parameterStorage)[config::kDescriptorParametersKeyPath] = new ConfigurationElement<IDescriptorConfig>(descriptor);
    
    int* numberOfIterations = new int;
    node[config::kNumberOfTrainIterationsKeyPath] >> *numberOfIterations;
    (*m_parameterStorage)[config::kNumberOfTrainIterationsKeyPath] = new ConfigurationElement<int>(numberOfIterations);
}

void DetectorConfig::Print() const
{
    std::cout << "\nEnabled: " << IsEnabled()
              << "\nDetector data file path: " << DetectorDataFilePath()
              << "\nDescriptor: " << DescriptorConfiguration()
              << "\nNumber of train iterations: " << NumberOfIterations()
    << std::endl;
}
