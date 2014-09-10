//
//  ImageCollectionMerger.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 29/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__ImageCollectionMerger__
#define __ClassifierTrainer__ImageCollectionMerger__

#include <string>
#include "CommonTypes.h"
#include "LandmarkCollectionData.h"


namespace collection
{
  
  struct CollectionData
  {
    LandmarkCollectionDataPtr collection;
    std::string collectionDataFilePath;
    std::string imageDirectory;
  };
  
  class ImageCollectionMerger
  {
  public:
    
    static void MergeCollections(const LandmarkCollectionDataPtr& i_data1,
                                 const LandmarkCollectionDataPtr& i_data2,
                                 CollectionData& o_collectionData);
    
  private:
    
    ImageCollectionMerger();
    
    LandmarkCollectionDataPtr m_collection1;
    LandmarkCollectionDataPtr m_colleciton2;
    
  };
  
}


#endif /* defined(__ClassifierTrainer__ImageCollectionMerger__) */
