//
//  ImageCollectionMerger.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 29/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ImageCollectionMerger.h"
#include <boost/assert.hpp>
#include <algorithm>
#include <iterator>


namespace collection
{
  
  void ImageCollectionMerger::MergeCollections(const LandmarkCollectionDataPtr& i_data1,
                                               const LandmarkCollectionDataPtr& i_data2,
                                               CollectionData& o_collectionData)
  {
    BOOST_ASSERT(i_data1->CollectionSize() != 0);
    BOOST_ASSERT(i_data2->CollectionSize() != 0);
    BOOST_ASSERT(o_collectionData.collection->CollectionSize() == 0);
    BOOST_ASSERT(o_collectionData.collectionDataFilePath.size() == 0);
    BOOST_ASSERT(o_collectionData.imageDirectory.size() == 0);
    
    LandmarkCollectionDataPtr collection = LandmarkCollectionDataPtr(new LandmarkCollectionData);
    
    i_data1->EnumerateConstColectionWithCallback([&] (const ImageLandmarkDataPtr& landmarkData, const int, bool*) {
      
      collection->AddImageLandmarkData(landmarkData);
    });
    
    
  }
  
  
}