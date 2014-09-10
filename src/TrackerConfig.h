//
//  TrackerConfig.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/25/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__TrackerConfig__
#define __ClassifierTrainer__TrackerConfig__

#include "DetectorConfig.h"


class TrackerConfig : public DetectorConfig
{
public:

    const std::string TrackerDataFilePath() const;
    const std::string DetectedLandmarkDataFilePath() const;
    
    void ParseConfiguration(const cv::FileNode& node);
    
    void Print();
};

#endif /* defined(__ClassifierTrainer__TrackerConfig__) */
