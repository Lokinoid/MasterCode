//
//  CommonTypes.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 28/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef ClassifierTrainer_CommonTypes_h
#define ClassifierTrainer_CommonTypes_h

#include <vector>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <memory>
#include <functional>
#include <math.h>

#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923
#define M_PI_4     0.785398163397448309616


namespace descriptor
{
  class IDescriptor;
}

class LandmarkCollectionData;
class ImageLandmarkData;
class EquationData;
class IFeature;


typedef enum
{
  FeatureTypeStar,
  FeatureTypeFace,
  FeatureTypeTestFunc
} FeatureType;

typedef std::string PathType;

//typedef cv::Point2d PointType; // for test function
typedef cv::Point2i PointType;
typedef std::vector<PointType> PointArrayType;
typedef std::shared_ptr<PointArrayType> PointArrayPtr;

typedef std::shared_ptr<ImageLandmarkData> ImageLandmarkDataPtr;
typedef std::shared_ptr<LandmarkCollectionData> LandmarkCollectionDataPtr;

typedef std::shared_ptr<EquationData> EquationDataPtr;
typedef std::vector<EquationDataPtr> EquationArray;
typedef std::shared_ptr<EquationArray> EquationArrayPtr;

typedef std::shared_ptr<IFeature> FeaturePtr;
typedef std::shared_ptr<descriptor::IDescriptor> DescriptorPtr;

#endif
