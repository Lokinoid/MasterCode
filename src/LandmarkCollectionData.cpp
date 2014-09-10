//
//  LandmarkCollectionData.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 20/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "LandmarkCollectionData.h"
#include <string>
#include <iostream>
#include <iterator>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <algorithm>

#include "ImageLandmarkData.h"


namespace fs = boost::filesystem;

LandmarkCollectionData::LandmarkCollectionData(const size_t reservedSize)
: m_collection(new CollectionType)
{
    m_collection->reserve(reservedSize);
}

//void LandmarkCollectionData::EnumerateConstColectionWithCallback(std::function<void(const ImageLandmarkDataPtr&)> callback) const
//{
//    std::for_each(m_collection->begin(), m_collection->end(), callback);
//}

void LandmarkCollectionData::EnumerateColectionWithCallback(std::function<void(ImageLandmarkDataPtr&)> callback)
{
    std::for_each(m_collection->begin(), m_collection->end(), callback);
}

void LandmarkCollectionData::EnumerateConstColectionWithCallback(std::function<void(const ImageLandmarkDataPtr& landmarkData,
                                                                                    const int index,
                                                                                    bool* stop)> callback) const
{
    int i = 0;
    bool shouldStop = false;
    for (const ImageLandmarkDataPtr& landmarkData : *m_collection)
    {
        if (shouldStop)
        {
            break;
        }
        callback(landmarkData, i++, &shouldStop);
    }
}

void LandmarkCollectionData::AddImageLandmarkData(const ImageLandmarkDataPtr& landmarkData)
{
    m_collection->push_back(landmarkData);
}

void LandmarkCollectionData::DeleteLandmarkData(const ImageLandmarkDataPtr& landmarkData)
{
    CollectionType::const_iterator iter = std::find(m_collection->begin(), m_collection->end(), landmarkData);
    if (iter != m_collection->end())
    {
        m_collection->erase(iter);
    }
}

const ImageLandmarkDataPtr& LandmarkCollectionData::at(const int i) const
{
    return m_collection->at(i);
}

const size_t LandmarkCollectionData::CollectionSize() const
{
    return m_collection->size();
}

const size_t LandmarkCollectionData::LandmarkArraySizeForImage(const std::string& imagePath) const
{
    ImageLandmarkDataPtr landmarkData = landmarkDataWithImagePath(imagePath);
    if (!landmarkData)
    {
        return 0;
    }
    
    return landmarkData->LandmarksSize();
}

ImageLandmarkDataPtr LandmarkCollectionData::landmarkDataWithImagePath(const PathType& imagePath) const
{
    CollectionType::iterator iter = std::find_if(m_collection->begin(), m_collection->end(), [&](const ImageLandmarkDataPtr& item){
        
        return item->ImagePath() == imagePath;
    });
    
    if (iter != m_collection->end())
    {
        return *iter;
    }
    return ImageLandmarkDataPtr();
}

const cv::Size LandmarkCollectionData::ImageSize() const
{
    CV_Assert(m_collection);
    
    ImageLandmarkDataPtr data =  *(m_collection->begin());
    return data->ImageSource().size();
}

void LandmarkCollectionData::PrintCollection() const
{
    std::for_each(m_collection->begin(), m_collection->end(), [&](const ImageLandmarkDataPtr& landmarkData){
        
        landmarkData->PrintLandmarkData();
    });
    std::cout<<std::endl;
}

void LandmarkCollectionData::AddCollection(const LandmarkCollectionDataPtr& collection)
{
    int lastUsedNameCounter;
    EnumerateConstColectionWithCallback([&] (const ImageLandmarkDataPtr& landmarkData, const int, bool*) {
        
        int counter = getCounterFromName((std::string)landmarkData->ImagePath());
        if (lastUsedNameCounter < counter)
        {
            lastUsedNameCounter = counter;
        }
    });
    
    collection->EnumerateColectionWithCallback([&] (ImageLandmarkDataPtr& landmarkData) {
        
        lastUsedNameCounter++;
        const cv::Mat image = landmarkData->ImageSource();
        
        const std::string nextImageName = createFileNameWithCounter(lastUsedNameCounter);
        landmarkData->SetImagePath(nextImageName);
        landmarkData->SetImageSource(image, true);
        
        this->AddImageLandmarkData(landmarkData);
        
    });
    
}

void LandmarkCollectionData::Sort()
{
    const std::string imageName = "test_image";
    const int imageNameLength = static_cast<int>(imageName.size());
    
    std::function<int (const std::string& filePath)> number = [&] (const std::string& filePath) {
        
        fs::path path(filePath);
        std::string imageName = path.filename().string();
        int number = std::stoi(imageName.substr(imageNameLength));
        
        return number;
    };
    
    std::sort(m_collection->begin(), m_collection->end(), [&](const ImageLandmarkDataPtr& landmarkData1, const ImageLandmarkDataPtr& landmarkData2){
        
        return number(landmarkData1->ImagePath()) < number(landmarkData2->ImagePath());
    });
}

const int LandmarkCollectionData::getCounterFromName(const std::string& imagePath) const
{
    boost::filesystem::path path(imagePath);
    std::string fileName = path.filename().string();
    int number = std::stoi(fileName.substr(10)); // testImage_
    
    return number;
}

const std::string LandmarkCollectionData::createFileNameWithCounter(const int counter)
{
    fs::path path(m_collection->at(0)->ImagePath());
    std::string filename = path.filename().string();
    std::string filenameWithoutNumber = filename.substr(0, 10); // testImage_
    
    const std::string format = filenameWithoutNumber + "%07d";
    boost::format formatFileName = boost::format(format) % std::to_string(counter);
    
    std::string newFilename = formatFileName.str() + path.extension().string();
    
    fs::path dir = path.parent_path();
    fs::path newImagePath = dir / newFilename;
    
    return newImagePath.string();
}
