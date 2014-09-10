//
//  ConfigBase.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/22/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__ConfigBase__
#define __ClassifierTrainer__ConfigBase__

#include <string>
#include <map>
#include <memory>
#include <opencv2/opencv.hpp>
#include "ConfigurationElement.h"


class IConfigurationElement;

class ConfigBase
{
public:
    
    ConfigBase();
    
    virtual ~ConfigBase(){}
    
    virtual const bool IsEnabled() const;
    
    virtual void ParseConfiguration(const cv::FileNode& node);
    
    virtual void Print() const = 0;
    
    template <class _Ty> const _Ty* generateParameter(const std::string& parameterKeyPath) const
    {
        return dynamic_cast<const ConfigurationElement<_Ty>*>(m_parameterStorage->at(parameterKeyPath))->Element();
    }

protected:
    
    bool stringToBool(const std::string& string);
    
    typedef std::map<const std::string, const IConfigurationElement*> TStorage;
    typedef std::shared_ptr<TStorage> TStoragePtr;

    TStoragePtr m_parameterStorage;
    
};


std::ostream& operator<<(std::ostream& stream, const ConfigBase* config);

#endif /* defined(__ClassifierTrainer__ConfigBase__) */
