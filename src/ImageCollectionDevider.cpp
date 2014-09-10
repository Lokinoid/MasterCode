//
//  ImageCollectionDevider.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 24/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ImageCollectionDevider.h"
#include <boost/assert.hpp>
#include <vector>

#include "LandmarkCollectionData.h"
#include "ImageLandmarkData.h"


void ImageCollectionDevider::DevideCollection(const CollectionData& input,
                                              CollectionData& output1,
                                              CollectionData& output2)
{
  BOOST_ASSERT(input.collection->CollectionSize() != 0);
  BOOST_ASSERT(output1.collectionDataFilePath.size() != 0);
  BOOST_ASSERT(output2.collectionDataFilePath.size() != 0);
  
  
  LandmarkCollectionDataPtr data = input.collection;
  
  const int kNumberOfItemsToRemove = 1000;
  std::vector<int> indicesToRemove;
  indicesToRemove.reserve(kNumberOfItemsToRemove);
  
  for (int i = 0; i < kNumberOfItemsToRemove; i++)
  {
    int randomValue = (i * 37) % data->CollectionSize();
    indicesToRemove.push_back(randomValue);
  }
    
  LandmarkCollectionDataPtr collection1(new LandmarkCollectionData);
  LandmarkCollectionDataPtr collection2(new LandmarkCollectionData);
  
  
  data->EnumerateConstColectionWithCallback([&] (const ImageLandmarkDataPtr& landmarkData, const int index, bool* stop) {
    
    bool isFound = false;
    for (int i : indicesToRemove)
    {
      if (index == i)
      {
        isFound = true;
        break;
      }
    }
    isFound ? collection1->AddImageLandmarkData(landmarkData)
            : collection2->AddImageLandmarkData(landmarkData);
  });
  
  output1.collection = collection1;
  output2.collection = collection2;
}