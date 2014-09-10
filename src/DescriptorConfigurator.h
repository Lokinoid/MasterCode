//
//  DescriptorConfigurator.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/20/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__DescriptorConfigurator__
#define __ClassifierTrainer__DescriptorConfigurator__

#include <iostream>
#include "IDescriptor.h"


class IDescriptorConfig;


class DescriptorConfigurator
{
public:
    
    typedef descriptor::IDescriptor IDescriptor;
    typedef std::shared_ptr<descriptor::IDescriptor> TIDescriptor;
    static const TIDescriptor DescriptorWithConfig(const IDescriptorConfig* config);
    
private:
    
    static const TIDescriptor createDescriptorHOG(const IDescriptorConfig* config);
    
};

#endif /* defined(__ClassifierTrainer__DescriptorConfigurator__) */
