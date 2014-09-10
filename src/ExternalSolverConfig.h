//
//  ExternalSolverConfig.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/21/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__ExternalSolverConfig__
#define __ClassifierTrainer__ExternalSolverConfig__

#include <map>
#include <vector>
#include <opencv2/opencv.hpp>
#include "ConfigBase.h"


class IConfigurationElement;

class ExternalSolverConfig : public ConfigBase
{
public:
    
    const std::string InputMatrixDirectory() const;
    const std::string OutputMatrixDirectory() const;
    
    void ParseConfiguration(const cv::FileNode& node);

    void Print() const;
private:
            
};
#endif /* defined(__ClassifierTrainer__ExternalSolverConfig__) */
