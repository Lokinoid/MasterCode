//
//  ProcrustesTransformer.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 15/06/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__ProcrustesTransformer__
#define __ClassifierTrainer__ProcrustesTransformer__

#include <iostream>
#include "CommonTypes.h"


class ProcrustesTransformer
{
public:
    static void Transform(const LandmarkCollectionDataPtr& src,
                          LandmarkCollectionDataPtr& dst,
                          const std::string& imageDirectory,
                          const std::string& landmarkDataPath);
    
private:
    
    ProcrustesTransformer(const std::string& imageDirectory,
                          const std::string& landmarkDataPath);
    void transform(const LandmarkCollectionDataPtr& src,
                   LandmarkCollectionDataPtr& dst);
    
    const std::string m_imageDirectory;
    const std::string m_landmarkDataPath;
    const ImageLandmarkDataPtr alignToImageData(const ImageLandmarkDataPtr& imageData,
                                                const ImageLandmarkDataPtr& idealData);

};

#endif /* defined(__ClassifierTrainer__ProcrustesTransformer__) */
