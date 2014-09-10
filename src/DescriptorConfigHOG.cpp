//
//  DescriptorConfigHOG.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/13/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "DescriptorConfigHOG.h"
#include <opencv2/opencv.hpp>
#include "ConfigurationKeyPaths.h"
#include "ConfigurationElement.h"


DescriptorConfigHOG::DescriptorConfigHOG(const cv::FileNode& node)
: m_keyPaths(TKeyPathStoragePtr(new TKeyPathStorage{ config::kDescriptorHOGWindowSizeKeyPath,
config::kDescriptorHOGCellSizeKeyPath,
config::kDescriptorHOGStrideSizeKeyPath,
config::kDescriptorHOGBinSizeKeyPath }))
{
	parseConfiguration(node);
}

DescriptorConfigHOG::~DescriptorConfigHOG()
{
}

const bool DescriptorConfigHOG::IsEnabled() const
{
	return true;
}

const DescriptorConfigHOG::ParameterType DescriptorConfigHOG::WindowSize() const
{
	return *generateParameter<ParameterType>(config::kDescriptorHOGWindowSizeKeyPath);
}

const DescriptorConfigHOG::ParameterType DescriptorConfigHOG::StrideSize() const
{
	return *generateParameter<ParameterType>(config::kDescriptorHOGStrideSizeKeyPath);
}

const DescriptorConfigHOG::ParameterType DescriptorConfigHOG::CellSize() const
{
	return *generateParameter<ParameterType>(config::kDescriptorHOGCellSizeKeyPath);
}

const DescriptorConfigHOG::ParameterType DescriptorConfigHOG::Bins() const
{
	return *generateParameter<ParameterType>(config::kDescriptorHOGBinSizeKeyPath);
}

const std::string DescriptorConfigHOG::DescriptorName() const
{
	return config::kDescriptorHOGName;
}

void DescriptorConfigHOG::parseConfiguration(const cv::FileNode& node)
{
	ConfigBase::ParseConfiguration(node);

	std::string* name = new std::string();
	node[config::kDescriptorNameKeyPath] >> *name;
	(*m_parameterStorage)[config::kDescriptorNameKeyPath] = new ConfigurationElement<std::string>(name);

	for (const std::string& keyPath : *m_keyPaths)
	{
		ParameterType* parameter = new ParameterType;
		node[keyPath] >> *parameter;
		(*m_parameterStorage)[keyPath] = new ConfigurationElement<ParameterType>(parameter);
	}
}

void DescriptorConfigHOG::Print() const
{
	std::cout << "\n    Name: " << DescriptorName()
		<< "\n    Window size: " << WindowSize() << "x" << WindowSize()
		<< "\n    Stride size: " << StrideSize() << "x" << StrideSize()
		<< "\n    Cell size: " << CellSize() << "x" << CellSize()
		<< "\n    Bins: " << Bins();
}
