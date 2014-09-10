//
//  Detector.h
//  SDMOpenCVClient
//
//  Created by Stanislav Orlov on 21/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __SDMOpenCVClient__Detector__
#define __SDMOpenCVClient__Detector__


#include "CommonTypes.h"
#include "IDescriptor.h"


class TrainedData;
class CascadeClassifier;

//! brief Facade class for detector
//
class Detector
{
public:
  
  Detector(const std::string& detectorDataFilePath);
  
  const cv::Mat Detect(const cv::Mat& image, const int numberOfIterations = INT_MAX);
  const cv::Mat Track(const cv::Mat& image, const cv::Mat& position);
  bool isFace(const cv::Mat& landmarks, const cv::Mat& prevLandmarks);
  
private:
  
  FeaturePtr m_feature;
  
  typedef std::shared_ptr<TrainedData> TrainedDataPtr;
  TrainedDataPtr m_detectorData;
  TrainedDataPtr m_trackerData;

//  typedef std::shared_ptr<ImageTransformer> ImageTransformerPtr;
//  ImageTransformerPtr m_transformer;
  
  typedef std::shared_ptr<descriptor::IDescriptor> DescriptorPtr;
  DescriptorPtr m_detectorDescriptor;
  DescriptorPtr m_trackerDescriptor;
  
  const cv::Mat checkAndConvertPosition(const cv::Mat& position, const cv::Size& imageSize);
  const cv::Mat calculateFromPosition(const cv::Mat& image,
                                      const TrainedDataPtr& trainedData,
                                      const DescriptorPtr& descriptor,
                                      const cv::Mat& startPosition,
                                      const int numberOfItrations);
  void createDescriptors(DescriptorPtr& detectorDescriptor,
                         DescriptorPtr& trackerDescriptor,
                         const std::string& detectorDataFilePath);
  void parseFilePath(const std::string& filePath, int* par1, int* par2, int* par3);
  void parseSIFTFilePath(const std::string& filePath, int* par1);

};

#endif /* defined(__SDMOpenCVClient__Detector__) */
