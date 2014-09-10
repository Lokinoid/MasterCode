//
//  ConfigurationElement.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/19/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ConfigurationElement.h"
#include <algorithm>

CurrentConfiguration::~CurrentConfiguration()
{
    std::for_each(storage.begin(), storage.end(), [&](std::pair<const std::string, const IConfigurationElement*> element){
        
        delete element.second;
    });
}
