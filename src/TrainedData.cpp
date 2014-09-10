//
//  TrainedData.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 28/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "TrainedData.h"
#include <vector>


static const std::string kPartKeyPath = "step";

TrainedData::TrainedData()
: m_storage(new DataStorageType()),
m_dataKeys(std::vector<std::string>())
{
}

TrainedData::~TrainedData()
{
}

void TrainedData::AddData(const cv::Mat& mat,
                          bool shoudSaveTmpData,
                          const std::string& savedTrainedDataFilePath)
{
    const std::string nextDataKey = generateDataKey(static_cast<int>(m_storage->size()));
    m_dataKeys.push_back(nextDataKey);
    m_storage->push_back(mat);
    
    if (shoudSaveTmpData)
    {
        Save(savedTrainedDataFilePath);
    }
}

void TrainedData::Save(const std::string& savedTrainedDataFilePath)
{
    cv::FileStorage fs(savedTrainedDataFilePath, cv::FileStorage::WRITE);
    
    if (!fs.isOpened())
    {
        std::cout<<"[TrainedData] !!! Error !!!: Cannot open file "<<savedTrainedDataFilePath<<std::endl;
    }
    for (int i = 0; i < m_storage->size(); i++)
    {
        fs << m_dataKeys[i] << m_storage->at(i);
    }
    
    fs.release();
}

bool TrainedData::Load(const std::string& savedTrainedDataFilePath)
{
    cv::FileStorage fs(savedTrainedDataFilePath, cv::FileStorage::READ);
    if (!fs.isOpened())
    {
        std::cout<<"File " << savedTrainedDataFilePath <<" does not exist"<<std::endl;
        CV_Assert(0);
        return false;
    }
    
    m_storage->clear();
    
    for (int i = 0; i < INT_MAX; i++)
    {
        cv::Mat tmpMat;
        const std::string dataKey = generateDataKey(i);
        fs[dataKey] >> tmpMat;
        if (tmpMat.empty())
        {
            std::cout<<"Empty matrix "<< i <<std::endl;
            break;
        }
        
        m_dataKeys.push_back(dataKey);
        m_storage->push_back(tmpMat);
    }
    
    fs.release();
    
    return true;
}

const int TrainedData::NumberOfIterations() const
{
    return static_cast<int>(m_storage->size());
}

const cv::Mat TrainedData::at(const int i) const
{
    if (i < m_storage->size())
    {
        return m_storage->at(i);
    }
    else
    {
        return cv::Mat();
    }
}

const std::string TrainedData::generateDataKey(const int number)
{
    const std::string dataKey = kPartKeyPath + std::to_string(number);
    return dataKey;
}
