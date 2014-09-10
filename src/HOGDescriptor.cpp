//
//  HOGDescriptor.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 05/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "HOGDescriptor.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "MatHelper.h"


descriptor::HOGDescriptor::HOGDescriptor(const cv::Size& windowSize,
                                         const cv::Size& strideSize,
                                         const cv::Size& cellSize,
                                         const unsigned int bins)
: m_windowSize(windowSize),
    m_strideSize(strideSize),
    m_cellSize(cellSize),
    m_bins(bins)
{
    m_descriptorExtractor = configureDescriptorExtractor();
}

const size_t descriptor::HOGDescriptor::DescriptorSize() const
{
    return m_descriptorExtractor->getDescriptorSize();
}

const cv::Mat descriptor::HOGDescriptor::ComputeDescriptor(const cv::Mat& image, const cv::Mat& position)
{
    PointArrayPtr points = MatHelper::matToPoints(position);
    
    const cv::Mat descriptor = computeDescriptorForPoints(image, points);
    const cv::Mat resultDescriptor = descriptor.reshape(0, 1).t();
    
    const double kExtendedPart = 1;
    cv::Mat extendedDescriptor = descriptor;
    extendedDescriptor.push_back(kExtendedPart);
    
    return extendedDescriptor;
}

const cv::Mat descriptor::HOGDescriptor::computeDescriptorForPoint(const cv::Mat& image, const PointType& point)
{
    std::vector<float> descriptors(m_descriptorExtractor->getDescriptorSize());
    
    cv::Rect rect = MatHelper::rectAtPoint(point, image.size(), m_windowSize);
    cv::Mat imagePart = image(rect);
	const int RGB2GRAY = 7;
    cvtColor(imagePart, imagePart, RGB2GRAY);
    cv::equalizeHist(imagePart, imagePart);
    
    m_descriptorExtractor->compute(imagePart, descriptors);
    
    cv::Mat_<float> floatMat(descriptors, true);
    
    cv::Mat_<double> resultMat;
    floatMat.convertTo(resultMat, CV_64FC1);
    
    return resultMat;
}

const cv::Mat descriptor::HOGDescriptor::computeDescriptorForPoints(const cv::Mat& image, const PointArrayPtr& pointArray)
{
    cv::Mat resultDescriptor;
    for (const PointType& point : *pointArray)
    {
        resultDescriptor.push_back(computeDescriptorForPoint(image, point));
    }
    
    return resultDescriptor;
}

const descriptor::HOGDescriptor::DescriptorPtr descriptor::HOGDescriptor::configureDescriptorExtractor()
{
    CV_Assert(m_windowSize.width % m_cellSize.width == 0);
    
    const cv::Size blockSize = m_windowSize;
    
    DescriptorPtr descriptor = DescriptorPtr(new DescriptorType(m_windowSize,
                                                                blockSize,
                                                                m_strideSize,
                                                                m_cellSize,
                                                                m_bins));
    
    return descriptor;
}

const cv::Size descriptor::HOGDescriptor::WindowSize() const
{
    return m_windowSize;
}

const cv::Size descriptor::HOGDescriptor::CellSize() const
{
    return m_cellSize;
}
