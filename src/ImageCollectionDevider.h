//
//  ImageCollectionDevider.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 24/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__ImageCollectionDevider__
#define __ClassifierTrainer__ImageCollectionDevider__


#include <string>
#include "CommonTypes.h"


struct CollectionData
{
  LandmarkCollectionDataPtr collection;
  std::string collectionDataFilePath;
  
  CollectionData(const LandmarkCollectionDataPtr& i_collection,
                 const std::string i_collectionDataFilePath) :
  collection(i_collection),
  collectionDataFilePath(i_collectionDataFilePath) {}
};

class ImageCollectionDevider
{
public:
  
  static void DevideCollection(const CollectionData& input,
                               CollectionData& output1,
                               CollectionData& output2);
  
};

#endif /* defined(__ClassifierTrainer__ImageCollectionDevider__) */
