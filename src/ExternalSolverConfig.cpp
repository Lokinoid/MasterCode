//
//  ExternalSolverConfig.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/21/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ExternalSolverConfig.h"
#include "ConfigurationKeyPaths.h"
#include "ConfigurationElement.h"


void ExternalSolverConfig::ParseConfiguration(const cv::FileNode& node)
{
    ConfigBase::ParseConfiguration(node);
    
    std::string* inputMatrixesDirectory = new std::string();
    node[config::kExternalSolverTmpInputMatrixDirectoryKeyPath] >> *inputMatrixesDirectory;
    (*m_parameterStorage)[config::kExternalSolverTmpInputMatrixDirectoryKeyPath] = new ConfigurationElement<std::string>(inputMatrixesDirectory);

    std::string* outputMatrixesDirectory = new std::string();
    node[config::kExternalSolverTmpOutputMatrixDirectoryKeyPath] >> *outputMatrixesDirectory;
    (*m_parameterStorage)[config::kExternalSolverTmpOutputMatrixDirectoryKeyPath] = new ConfigurationElement<std::string>(outputMatrixesDirectory);
}

const std::string ExternalSolverConfig::InputMatrixDirectory() const
{
    return *generateParameter<std::string>(config::kExternalSolverTmpInputMatrixDirectoryKeyPath);
}

const std::string ExternalSolverConfig::OutputMatrixDirectory() const
{
    return *generateParameter<std::string>(config::kExternalSolverTmpOutputMatrixDirectoryKeyPath);
}

void ExternalSolverConfig::Print() const
{
    std::cout << "\nEnabled: " << IsEnabled()
              << "\nInput matrix directory: " << InputMatrixDirectory()
              << "\nOutput matrix directory: " << OutputMatrixDirectory()
              << std::endl;
}
