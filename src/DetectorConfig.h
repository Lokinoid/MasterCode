//
//  DetectorConfig.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/22/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__DetectorConfig__
#define __ClassifierTrainer__DetectorConfig__

#include <map>
#include <opencv2/opencv.hpp>
#include "ConfigBase.h"


class IConfigurationElement;
class IDescriptorConfig;

class DetectorConfig : public ConfigBase
{
public:
    
    const bool IsEnabled() const;
    const std::string DetectorDataFilePath() const;
    const int NumberOfIterations() const;
    const IDescriptorConfig* DescriptorConfiguration() const;
    
    virtual void ParseConfiguration(const cv::FileNode& node);
    
    virtual void Print() const;
    
private:
    
};

#endif /* defined(__ClassifierTrainer__DetectorConfig__) */
