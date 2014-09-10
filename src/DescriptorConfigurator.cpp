//
//  DescriptorConfigurator.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/20/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "DescriptorConfigurator.h"
#include "ConfigurationKeyPaths.h"
#include "DescriptorConfigHOG.h"
#include "HOGDescriptor.h"


const DescriptorConfigurator::TIDescriptor DescriptorConfigurator::DescriptorWithConfig(const IDescriptorConfig* config)
{
    if (config->DescriptorName() == config::kDescriptorHOGName)
    {
        return createDescriptorHOG(config);
    }
    else
    {
        std::cout << "Cannot configura descriptor: " << config << std::endl;
        CV_Assert(0);
    }
    
    return NULL;
}

const DescriptorConfigurator::TIDescriptor DescriptorConfigurator::createDescriptorHOG(const IDescriptorConfig* config)
{
	const DescriptorConfigHOG* configHOG = dynamic_cast<const DescriptorConfigHOG*>(config);
	const int kWindowSize = static_cast<int>(configHOG->WindowSize());
	const int kStrideSize = static_cast<int>(configHOG->StrideSize());
	const int kCellSize = static_cast<int>(configHOG->CellSize());
	const int kBins = static_cast<int>(configHOG->Bins());
 
	const DescriptorPtr descriptor(new descriptor::HOGDescriptor(cv::Size(kWindowSize, kWindowSize),
                                                                 cv::Size(kStrideSize, kStrideSize),
                                                                 cv::Size(kCellSize, kCellSize),
                                                                 kBins));
    
    return descriptor;
}
