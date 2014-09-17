//
//  ImagePicker.cpp
//  OpenCVTest2
//
//  Created by Stanislav Orlov on 19/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ImagePicker.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <iterator>
#include <boost/filesystem.hpp>

#include "LandmarkCollectionData.h"
#include "FileFinder.h"
#include "DataContainer.h"
#include "FileLinesParser.h"
#include "ImageLandmarkData.h"


ImagePicker::ImagePicker(const std::string& imageDirectory)
: m_directory(imageDirectory)
{
}

ImagePicker::~ImagePicker()
{
}

const LandmarkCollectionDataPtr ImagePicker::FetchImagesAndLandmarksWithProcess()
{
    m_fileNames = fetchFileNames();
    
    TDataContainer dataContainer;
    CFileLinesParser::ParseFilesToContainerData(*m_fileNames, dataContainer);
    
    LandmarkCollectionDataPtr collection = createLandmarkCollectionFormDataContainer(dataContainer);
    
    return collection;
}

const LandmarkCollectionDataPtr ImagePicker::FetchLandmarkCollectionDataFromFile(std::string filePath)
{
    TDataContainer dataContainer;
    CFileLinesParser::ParseFilesToContainerData({filePath}, dataContainer);
    if (dataContainer.size() == 0)
    {
        return LandmarkCollectionDataPtr();
    }
    
    LandmarkCollectionDataPtr collection = createLandmarkCollectionFormDataContainer(dataContainer);
    collection->Sort();
    
    return collection;
}

const LandmarkCollectionDataPtr ImagePicker::createLandmarkCollectionFormDataContainer(const TDataContainer& dataContainer)
{
    LandmarkCollectionDataPtr collection = LandmarkCollectionDataPtr(new LandmarkCollectionData);
    
    typedef LandmarkCollectionData::LandmarkType LandmarkType;
    
    for (TDataContainer::const_iterator dataIter = dataContainer.begin(), dataEnd = dataContainer.end(); dataIter != dataEnd; ++dataIter)
    {
        
        std::string path = fullPathForFileName(dataIter->first);
        ImageLandmarkDataPtr landmarkData = ImageLandmarkDataPtr(new ImageLandmarkData(path));
        for (TCoordinatesContainer::const_iterator coordIter = dataIter->second.begin(), coordEnd = dataIter->second.end(); coordIter != coordEnd; ++coordIter)
        {
            landmarkData->AddLandmark(LandmarkType(coordIter->first, coordIter->second));
        }
        
        collection->AddImageLandmarkData(landmarkData);
    }
    
    return collection;
}

const std::string ImagePicker::fullPathForFileName(const std::string& filename)
{
    typedef boost::filesystem::path path;
    
    if (path(filename).has_root_directory())
    {
        return filename;
    }
    
    path fullPath = path(m_directory);
    fullPath /= path(filename);
    
    if (!boost::filesystem::exists(fullPath))
    {
        std::cout << "File path: " << fullPath << std::endl;
        BOOST_ASSERT(boost::filesystem::exists(fullPath));
    }
    
    return fullPath.string();
}

const ImagePicker::FileNameContainerPtr ImagePicker::fetchFileNames()
{
    CFileFinder finder;
    finder.SearchFiles(m_directory, false);
    
    return finder.GetFileNames();
}

const ImagePicker::FileNameContainerPtr ImagePicker::ImagePaths()
{
    CFileFinder finder;
    finder.SearchFiles(m_directory, false, ".jpg");
    
    return finder.GetFileNames();
}