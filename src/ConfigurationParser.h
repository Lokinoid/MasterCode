//
//  ConfigurationParser.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 22/06/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__ConfigurationParser__
#define __ClassifierTrainer__ConfigurationParser__

#include <iostream>
#include <map>
#include <string>
#include "DescriptorConfig.h"
#include "ConfigurationElement.h"


class ConfigurationParser
{
public:
    
    static std::shared_ptr<CurrentConfiguration> LoadConfiguration(const std::string& configurationFilePath);
    
private:
    
    ConfigurationParser(const std::string& configurationFilePath);
    
    std::shared_ptr<CurrentConfiguration> parse();
    
    typedef std::shared_ptr<CurrentConfiguration> TCurrentConfiguration;
    template <class _Ty> void parseInstance(const std::string& parameterKeyPath,
                                            const cv::FileStorage& storage,
                                            TCurrentConfiguration& configuration);    
    const std::string m_filePath;
};

#endif /* defined(__ClassifierTrainer__ConfigurationParser__) */
