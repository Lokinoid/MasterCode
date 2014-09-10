//
//  ConfigurationTester.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/12/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ConfigurationTester.h"
#include <opencv2/opencv.hpp>
#include "ConfigurationManager.h"
#include "ConfigurationKeyPaths.h"
#include "DescriptorConfig.h"


void ConfigurationTester::StartTest()
{
	const std::string testFilePath = "../config.yml";
    
    ConfigurationTester tester;
//    tester.createTestConfigurationFile(testFilePath);
    tester.readTestConfiguration(testFilePath);
}

void ConfigurationTester::createTestConfigurationFile(const std::string& filePath)
{
    cv::FileStorage storage(filePath, cv::FileStorage::WRITE);
    const std::vector<const std::string> keyPaths = {config::kImageDirectoryKeyPath,
                                                     config::kLandmarkDataFileNameKeyPath};
    for (const std::string& keyPath : keyPaths)
    {
        storage << keyPath << std::string(keyPath) + "_value";
    }
    
    const std::vector<const std::string> descriptorKeyPaths = {config::kDescriptorHOGWindowSizeKeyPath,
                                                               config::kDescriptorHOGStrideSizeKeyPath,
                                                               config::kDescriptorHOGCellSizeKeyPath,
                                                               config::kDescriptorHOGBinSizeKeyPath};
    storage << config::kDescriptorNameKeyPath << "HOG";
    storage << config::kDescriptorParametersKeyPath;
    storage << "{";
    for (const std::string& descKeyPath : descriptorKeyPaths)
    {
        storage << descKeyPath << (int)descKeyPath.length();
    }
    storage << "}";
    storage.release();
}


void ConfigurationTester::readTestConfiguration(const std::string& filePath)
{
    ConfigurationManager::ResetWithConfigurationFile(filePath);
    ConfigurationManager::ShowCurrentConfiguration();
}
