//
//  ConfigurationElement.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/19/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__ConfigurationElement__
#define __ClassifierTrainer__ConfigurationElement__

#include <map>
#include <string>
//#include "DescriptorConfig.h"


//@brief Interface for configuration element.
//
class IConfigurationElement
{
public:
    virtual ~IConfigurationElement(){}
};

//@brief Generic wrapper for element
//
template <class _Ty>
class ConfigurationElement : public IConfigurationElement
{
public:
    ConfigurationElement(const _Ty* element) : m_element(element)
    {}
    ~ConfigurationElement()
    {
        delete m_element;
        m_element = NULL;
    }
    const _Ty* Element() const
    {
        return m_element;
    }
    
private:
    const _Ty* m_element;
};

template <>
class ConfigurationElement<bool> : public IConfigurationElement
{
public:
    ConfigurationElement(bool element) : m_element(element)
    {}
    ~ConfigurationElement()
    {}
    bool Element() const
    {
        return m_element;
    }
private:
    bool m_element;
};


//@brief Class that represents current configuration.
//
class CurrentConfiguration
{
public:
    
    std::map<const std::string, const IConfigurationElement*> storage;
    ~CurrentConfiguration();
};

#endif /* defined(__ClassifierTrainer__ConfigurationElement__) */
