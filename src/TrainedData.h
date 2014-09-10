//
//  TrainedData.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 28/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__TrainedData__
#define __ClassifierTrainer__TrainedData__

#include <opencv2/opencv.hpp>
#include <memory>


class TrainedData
{
public:
  
  TrainedData();
  ~TrainedData();
  
  void Save(const std::string& savedTrainedDataFilePath);
  bool Load(const std::string& savedTrainedDataFilePath);
  
  void AddData(const cv::Mat& mat,
               bool shoudSaveTmpData,
               const std::string& savedTrainedDataFilePath);
  const cv::Mat at(const int i) const;
  const int NumberOfIterations() const;
  
private:
  
  typedef std::vector<cv::Mat> DataStorageType;
  typedef std::shared_ptr<DataStorageType> DataStoragePtr;
  
  DataStoragePtr m_storage;
  std::vector<std::string> m_dataKeys;
  
  const std::string generateDataKey(const int number);
};

#endif /* defined(__ClassifierTrainer__TrainedData__) */
