//
//  LandmarkCollectionData.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 20/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__LandmarkCollectionData__
#define __ClassifierTrainer__LandmarkCollectionData__


#include <vector>
#include <functional>
#include "ImageLandmarkData.h"
#include "CommonTypes.h"


//@brief Model class.
//       Represents landmark and image data collection.
//
class LandmarkCollectionData
{
public:
    
    typedef ImageLandmarkData::LandmarkType LandmarkType;
    
    typedef ImageLandmarkData::LandmarkArray LandmarkArray;
    typedef ImageLandmarkData::LandmarkArrayPtr LandmarkArrayPtr;
    
    LandmarkCollectionData(const size_t reservedSize = 0);
    
    //@brief Enumerates collection
    //@param callback
    //       Function that invokes for each element of collection.
    void EnumerateColectionWithCallback(std::function<void(ImageLandmarkDataPtr&)> callback);
    //void EnumerateConstColectionWithCallback(std::function<void(const ImageLandmarkDataPtr&)> callback) const;
    void EnumerateConstColectionWithCallback(std::function<void(const ImageLandmarkDataPtr&, const int, bool*)> callback) const;
    
    //@brief Adds new collection to existing one.
    //@param collection
    //       Collection to add.
    void AddCollection(const LandmarkCollectionDataPtr& collection);
    
    //@brief Adds new model data to collection.
    //@param landmarkData
    //       Model object that represents image with corresponding set of landmarks.
    void AddImageLandmarkData(const ImageLandmarkDataPtr& landmarkData);
    
    //@brief Removes model data from collection if exists.
    //@param landmarkData
    //       Model object that represents image with corresponding set of landmarks.
    void DeleteLandmarkData(const ImageLandmarkDataPtr& landmarkData);
    
    //@brief Gives access to element of collection by index.
    //@param index
    //       Index of element to access.
    const ImageLandmarkDataPtr& at(const int index) const;
    
    //@breif Returns size of image.
    //       Assume all images are the same size.
    const cv::Size ImageSize() const;
    
    //@breif Returns amount of elements for current collection.
    const size_t CollectionSize() const;
    
    //@breif Returns amount of landmarks for image with chosen path.
    //@param imagePath
    //       Path of image to choose landmarks for.
    const size_t LandmarkArraySizeForImage(const std::string& imagePath) const;
    
    //@brief Sorts models data by numbers in image names.
    void Sort();
    
    //@brief Prints collection data.
    void PrintCollection() const;
    
private:
    
    typedef std::vector<ImageLandmarkDataPtr> CollectionType;
    typedef std::shared_ptr<CollectionType> CollectionTypePtr;
    CollectionTypePtr m_collection;
    
    ImageLandmarkDataPtr landmarkDataWithImagePath(const std::string& imagePath) const;
    const int getCounterFromName(const std::string& imagePath) const;
    const std::string createFileNameWithCounter(const int counter);
    
};

#endif /* defined(__ClassifierTrainer__LandmarkCollectionData__) */
