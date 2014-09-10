//
//  DescriptorConfigHOG.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/13/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__DescriptorConfigHOG__
#define __ClassifierTrainer__DescriptorConfigHOG__

#include "DescriptorConfig.h"
#include <map>


class DescriptorConfigHOG : public IDescriptorConfig
{
public:
    
    typedef int ParameterType;
    
    DescriptorConfigHOG(const cv::FileNode& node);
    ~DescriptorConfigHOG();
    
    const bool IsEnabled() const;

    const std::string DescriptorName() const;
    const ParameterType WindowSize() const;
    const ParameterType StrideSize() const;
    const ParameterType CellSize() const;
    const ParameterType Bins() const;
    
    void Print() const;
    
private:
    
    void parseConfiguration(const cv::FileNode& node);
      
    typedef std::vector<const std::string> TKeyPathStorage;
    typedef std::shared_ptr<TKeyPathStorage> TKeyPathStoragePtr;
    TKeyPathStoragePtr m_keyPaths;
};

#endif /* defined(__ClassifierTrainer__DescriptorConfigHOG__) */
