//
//  ConfigurationTester.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/12/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__ConfigurationTester__
#define __ClassifierTrainer__ConfigurationTester__

#include <iostream>

class ConfigurationTester
{
public:
    
    static void StartTest();
    
private:
    
    void createTestConfigurationFile(const std::string& filePath);
    void readTestConfiguration(const std::string& filePath);
};
#endif /* defined(__ClassifierTrainer__ConfigurationTester__) */
