//
//  DescriptorConfigurationFactory.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/25/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__DescriptorConfigurationFactory__
#define __ClassifierTrainer__DescriptorConfigurationFactory__

#include "DescriptorConfigHOG.h"
#include <opencv2/opencv.hpp>


class DescriptorConfigurationFactory
{
public:
    static IDescriptorConfig* DescriptorConfigInstanceForDescriptorNode(const cv::FileNode& node);
};


#endif /* defined(__ClassifierTrainer__DescriptorConfigurationFactory__) */
