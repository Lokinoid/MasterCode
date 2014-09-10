//
//  ConfigurationKeyPaths.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/11/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__ConfigurationKeyPaths__
#define __ClassifierTrainer__ConfigurationKeyPaths__

#include <string>

namespace config
{   
    // configuration key paths
    
    extern const std::string kImageDirectoryKeyPath;
    extern const std::string kLandmarkDataFileNameKeyPath;
    extern const std::string kNumberOfTrainIterationsKeyPath;
    
    extern const std::string kEnabledKeyPath;
    extern const std::string kManualKeyPath;
    
    // check quality parameters
    extern const std::string kCheckQualityParameterKeyPath;
    extern const std::string kCheckQualityNewLandmarkDataFileNameKeyPath;
    
    extern const std::string kTrueStringKeyPath;
    extern const std::string kFalseStringKeyPath;
    
    // detector
    extern const std::string kDetectorParametersKeyPath;
    extern const std::string kDetectorDataFileNameKeyPath;

    // tracker
    extern const std::string kTrackerParametersKeyPath;
    extern const std::string kTrackerDataFileNameKeyPath;
    extern const std::string kTrackerDetectedLandmarksDataFilePath;
    
    // descriptor
    extern const std::string kDescriptorParametersKeyPath;
    extern const std::string kDescriptorNameKeyPath;
    
    //HOG
    extern const std::string kDescriptorHOGName;
    extern const std::string kDescriptorHOGWindowSizeKeyPath;
    extern const std::string kDescriptorHOGStrideSizeKeyPath;
    extern const std::string kDescriptorHOGCellSizeKeyPath;
    extern const std::string kDescriptorHOGBinSizeKeyPath;
    
    //SIFT
    extern const std::string kDescriptorSIFTName;
    extern const std::string kDescriptorSIFTDiameterSizeKeyPath;
    
    //external solver
    extern const std::string kExternalSolverParameterKeyPath;
    extern const std::string kExternalSolverTmpInputMatrixDirectoryKeyPath;
    extern const std::string kExternalSolverTmpOutputMatrixDirectoryKeyPath;
    
}

#endif /* defined(__ClassifierTrainer__ConfigurationKeyPaths__) */
