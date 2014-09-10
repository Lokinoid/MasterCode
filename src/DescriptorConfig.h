//
//  DescriptorConfig.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 22/06/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef ClassifierTrainer_DescriptorConfig_h
#define ClassifierTrainer_DescriptorConfig_h

#include <opencv2/opencv.hpp>
#include "ConfigBase.h"
#include "ConfigurationKeyPaths.h"


class IDescriptorConfig : public ConfigBase
{
public:
    
    virtual ~IDescriptorConfig(){}
    
    virtual const bool IsEnabled() const = 0;
    virtual const std::string DescriptorName() const = 0;
    
    virtual void Print() const = 0;
};

#endif
