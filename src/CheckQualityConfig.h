//
//  CheckQualityConfig.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/20/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__CheckQualityConfig__
#define __ClassifierTrainer__CheckQualityConfig__

#include <map>
#include <vector>
#include <opencv2/opencv.hpp>
#include "ConfigBase.h"


class IConfigurationElement;

class CheckQualityConfig : public ConfigBase
{
public:
    
    bool IsManual() const;
    const std::string LandmarkDataFilePath() const;
    
    void ParseConfiguration(const cv::FileNode& node);
    void Print() const;

private:
    
};

#endif /* defined(__ClassifierTrainer__CheckQualityConfig__) */
