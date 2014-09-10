//
//  Detector.cpp
//  SDMOpenCVClient
//
//  Created by Stanislav Orlov on 21/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "Detector.h"

#include <boost/regex.hpp>

#include "TrainedData.h"
#include "FaceFeature.h"
#include "CascadeClassifier.h"
#include "ImagePainter.h"
#include "StatisticProduser.h"
#include "IDescriptor.h"
#include "HOGDescriptor.h"
#include "SIFTDescriptor.h"


static const int kTrainedFaceSize = 200;


Detector::Detector(const std::string& detectorDataFilePath)
: m_feature(new FaceFeature),
  m_detectorData(new TrainedData),
  m_trackerData(new TrainedData)
{
  if (!m_detectorData->Load(detectorDataFilePath))
  {
    std::cout<<"[Detector] Cannot load file: " << detectorDataFilePath<<std::endl;
  }
  
//  if (!m_trackerData->Load(PathKeys::kTrackerFacesDataFilePathXml))
//  {
//    std::cout<<"[Detector] Cannot load file: " << PathKeys::kTrackerFacesDataFilePathXml<<std::endl;
//  }
  
  createDescriptors(m_detectorDescriptor, m_trackerDescriptor, detectorDataFilePath);
}

const cv::Mat Detector::Detect(const cv::Mat& image, const int numberOfIterations)
{
//  const cv::Mat croppedImage = m_transformer->CropImage(image, activeRect);
//  const cv::Mat scaledImage = m_transformer->
  
  const cv::Mat startPosition = m_feature->StartPosition(image);
  if (startPosition.empty())
  {
//    std::cout<<"[Detector] Bad face rect"<<std::endl;
    return cv::Mat();
  }
  
//  std::cout<<"Detect"<<std::endl;
  const cv::Mat resultPosition = calculateFromPosition(image,
                                                       m_detectorData,
                                                       m_detectorDescriptor,
                                                       startPosition,
                                                       numberOfIterations);
  
  return resultPosition;
}

const cv::Mat Detector::Track(const cv::Mat& image, const cv::Mat& position)
{
  CV_Assert("CANNOT USE TRACKER" == NULL);
  
  std::cout<<"Track"<<std::endl;
  const cv::Mat resultPosition = calculateFromPosition(image,
                                                       m_trackerData,
                                                       m_trackerDescriptor,
                                                       position,
                                                       INT_MAX);
  
  return resultPosition;
}

bool Detector::isFace(const cv::Mat& landmarks, const cv::Mat& prevLandmarks)
{
  const double thresh = 2;
  double mistake = statistics::StatisticProduser::AvgMistake(landmarks, prevLandmarks);
//  std::cout<<mistake<<std::endl;

  return mistake < thresh;
}

const cv::Mat Detector::checkAndConvertPosition(const cv::Mat& position, const cv::Size& imageSize)
{
  const int maxSizeX = imageSize.width - 1;
  const int maxSizeY = imageSize.height - 1;
  
  cv::Mat_<double> resultPosition(position.size(), CV_64FC1);
  double currentX;
  double currentY;
  for (int i = 0; i < position.rows; i+=2)
  {
    currentX = static_cast<double>(MIN(MAX(0, position.at<int>(i,0)), maxSizeX));
    currentY = static_cast<double>(MIN(MAX(0, position.at<int>(i + 1,0)), maxSizeY));
    
    resultPosition(i,0) = currentX;
    resultPosition(i + 1,0) = currentY;
  }
  
  return resultPosition;
}

const cv::Mat Detector::calculateFromPosition(const cv::Mat& image,
                                              const TrainedDataPtr& trainedData,
                                              const DescriptorPtr& descriptor,
                                              const cv::Mat& startPosition,
                                              const int numberOfIterations)
{
  CV_Assert(!image.empty());
  CV_Assert(!startPosition.empty());
  
  cv::Mat currentPosition;
  startPosition.copyTo(currentPosition);
  
//  std::cout<<"start"<<currentPosition<<std::endl;

  int iterationsAmount = std::min(numberOfIterations, trainedData->NumberOfIterations());
  for (int i = 0; i < iterationsAmount; i++)
  {
    cv::Mat positionDescriptor = descriptor->ComputeDescriptor(image, currentPosition);
    cv::Mat_<int> nextPosition = currentPosition + trainedData->at(i) * positionDescriptor;
    
    cv::Mat correctPosition = checkAndConvertPosition(nextPosition, image.size());
    
//    ImagePainter painter;
//    cv::Mat img = painter.DrawLandmarks(&image, &currentPosition);
//    cv::imshow("Landmarks",img);
//    cv::waitKey();
    
    currentPosition = correctPosition;
  }
  
//  std::cout<<"end"<<currentPosition<<std::endl;

  return currentPosition;
}

void Detector::createDescriptors(DescriptorPtr& detectorDescriptor,
                                 DescriptorPtr& trackerDescriptor,
                                 const std::string& detectorDataFilePath)

{
  int descriptorDiameter;
  parseSIFTFilePath(detectorDataFilePath, &descriptorDiameter);
  std::cout<<"Detector SIFT: " << descriptorDiameter << std::endl;

  detectorDescriptor = DescriptorPtr(new descriptor::SIFTDescriptor(descriptorDiameter));
  
//  int detectorWindowSize;
//  int detectorCellSize;
//  int detectorBins;
//  
//  parseFilePath(detectorDataFilePath, &detectorWindowSize, &detectorCellSize, &detectorBins);
//  std::cout<<"Detector: " << detectorWindowSize << " " << detectorCellSize << " " << detectorBins <<std::endl;
//  
//  detectorDescriptor = DescriptorPtr(new descriptor::HOGDescriptor(cv::Size(detectorWindowSize, detectorWindowSize),
//                                                                   cv::Size(detectorCellSize, detectorCellSize),
//                                                                   detectorBins));
  
  const int kTrackerWindowSize = 16;
  const int kTrackerCellSize = 8;
  const int kTrackerBins = 9;
  trackerDescriptor = DescriptorPtr(new descriptor::HOGDescriptor(cv::Size(kTrackerWindowSize, kTrackerWindowSize),
                                                                  cv::Size(kTrackerCellSize, kTrackerCellSize),
                                                                  cv::Size(kTrackerCellSize, kTrackerCellSize),
                                                                  kTrackerBins));
}

void Detector::parseSIFTFilePath(const std::string& filePath, int* par1)
{
  const char* pattern = "\\d+";
  
  boost::regex re(pattern);
  
  boost::sregex_iterator it(filePath.begin(), filePath.end(), re);
  boost::sregex_iterator end;
  
  *par1 = std::stoi(it->str());
}

void Detector::parseFilePath(const std::string& filePath, int* par1, int* par2, int* par3)
{
  const char* pattern = "\\d+";
  
  boost::regex re(pattern);
  
  boost::sregex_iterator it(filePath.begin(), filePath.end(), re);
  boost::sregex_iterator end;
  
//  BOOST_ASSERT(it->size() == 3);
  
  *par1 = std::stoi(it->str());
  it++;
  *par2 = std::stoi(it->str());
  it++;
  *par3 = std::stoi(it->str());
}