//
//  HOGDescriptor.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 05/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__HOGDescriptor__
#define __ClassifierTrainer__HOGDescriptor__


#include "IDescriptor.h"

namespace descriptor
{
    
    class HOGDescriptor : public IDescriptor
    {
    public:
        
        HOGDescriptor(const cv::Size& windowSize = cv::Size(16,16),
                      const cv::Size& strideSize = cv::Size(8,8),
                      const cv::Size& cellSize = cv::Size(8,8),
                      const unsigned int bins = 9);
        
        const cv::Mat ComputeDescriptor(const cv::Mat& image, const cv::Mat& position);
        
        const size_t DescriptorSize() const;
        const cv::Size WindowSize() const;
        const cv::Size CellSize() const;
        
    private:
        
        const cv::Mat computeDescriptorForPoint(const cv::Mat& image, const PointType& point);
        const cv::Mat computeDescriptorForPoints(const cv::Mat& image, const PointArrayPtr& pointArray);
        
        typedef cv::HOGDescriptor DescriptorType;
        typedef std::shared_ptr<DescriptorType> DescriptorPtr;
        
        DescriptorPtr m_descriptorExtractor;
        cv::Size m_windowSize;
        cv::Size m_strideSize;
        cv::Size m_cellSize;
        unsigned int m_bins;
        
        const DescriptorPtr configureDescriptorExtractor();
        
    };
    
}

#endif /* defined(__ClassifierTrainer__HOGDescriptor__) */
