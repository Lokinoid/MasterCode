//
//  DescriptorConfigurationFactory.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/25/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "DescriptorConfigurationFactory.h"
#include "DescriptorConfigHOG.h"


IDescriptorConfig* DescriptorConfigurationFactory::DescriptorConfigInstanceForDescriptorNode(const cv::FileNode& node)
{
    std::string descriptorName;
    node[config::kDescriptorNameKeyPath] >> descriptorName;
    if (descriptorName == config::kDescriptorHOGName)
    {
        return new DescriptorConfigHOG(node);
    }
    
    std::cout<<"Descriptor is not recognized"<<std::endl;
    return NULL;
}


