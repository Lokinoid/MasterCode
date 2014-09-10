//
//  ConfigBase.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/22/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ConfigBase.h"
#include "ConfigurationElement.h"
#include "ConfigurationKeyPaths.h"


ConfigBase::ConfigBase()
: m_parameterStorage(new TStorage)
{
}

const bool ConfigBase::IsEnabled() const
{
    return dynamic_cast<const ConfigurationElement<bool>*>(m_parameterStorage->at(config::kEnabledKeyPath))->Element();
}

void ConfigBase::ParseConfiguration(const cv::FileNode& node)
{
    std::string isEnabled;
    node[config::kEnabledKeyPath] >> isEnabled;
    (*m_parameterStorage)[config::kEnabledKeyPath] = new ConfigurationElement<bool>(stringToBool(isEnabled));
}

bool ConfigBase::stringToBool(const std::string& string)
{
    return string == config::kTrueStringKeyPath;
}

std::ostream& operator<<(std::ostream& stream, const ConfigBase* config)
{
    config->Print();
    return stream;
}
