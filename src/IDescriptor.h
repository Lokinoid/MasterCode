//
//  IDescriptor.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 05/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef ClassifierTrainer_IDescriptor_h
#define ClassifierTrainer_IDescriptor_h

#include <opencv2/opencv.hpp>
#include "CommonTypes.h"

namespace descriptor
{
    
    class IDescriptor
    {
    public:
        
        virtual ~IDescriptor() {}
        
        //! Deprecated
        //  virtual const cv::Mat ComputeDescriptor(const cv::Mat& image, const PointType& point) = 0;
        
        //! Deprecated
        //  virtual const cv::Mat ComputeDescriptor(const cv::Mat& image, const PointArrayPtr& pointArray) = 0;
        
        //! @brief Computes descriptor in point area of windowSize size for mat representation of points.
        virtual const cv::Mat ComputeDescriptor(const cv::Mat& image, const cv::Mat& position) = 0;
        
        //! @brief Size of descriptor.
        virtual const size_t DescriptorSize() const = 0;
        
        //! @brief Size of window where descriptor will be cumputed.
        virtual const cv::Size WindowSize() const = 0;
    };
    
}
#endif
