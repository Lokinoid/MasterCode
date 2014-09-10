//
//  ConfigurationKeyPaths.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 8/11/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ConfigurationKeyPaths.h"

namespace config
{   
    // configuration key paths
    
    const std::string kImageDirectoryKeyPath = "imageDirectory";
    const std::string kLandmarkDataFileNameKeyPath = "landmarkDataFilePath";
    const std::string kTrainedDetectorDataKeyPath = "trainedDetectorData";
    const std::string kNumberOfTrainIterationsKeyPath = "numberOfTrainIterations";
    
    const std::string kEnabledKeyPath = "enabled";
    const std::string kManualKeyPath = "manual";
    
    // check quality parameters
    const std::string kCheckQualityParameterKeyPath = "checkDataQualityParameters";
    const std::string kCheckQualityNewLandmarkDataFileNameKeyPath = "newLandmarkDataFilePath";
    
    const std::string kTrueStringKeyPath = "true";
    const std::string kFalseStringKeyPath = "false";

    // detector
    const std::string kDetectorParametersKeyPath = "detectorParameters";
    const std::string kDetectorDataFileNameKeyPath = "detectorDataFilePath";
    
    // tracker
    const std::string kTrackerParametersKeyPath = "trackerParameters";
    const std::string kTrackerDataFileNameKeyPath = "trackerDataFilePath";
    const std::string kTrackerDetectedLandmarksDataFilePath = "detectedLandmarkDataFilePath";

    // descriptor
    const std::string kDescriptorParametersKeyPath = "descriptorParameters";
    const std::string kDescriptorNameKeyPath = "name";
    
    //HOG
    const std::string kDescriptorHOGName = "HOG";
    const std::string kDescriptorHOGWindowSizeKeyPath = "windowSize";
    const std::string kDescriptorHOGStrideSizeKeyPath = "strideSize";
    const std::string kDescriptorHOGCellSizeKeyPath = "cellSize";
    const std::string kDescriptorHOGBinSizeKeyPath = "binSize";
    
    //SIFT
    const std::string kDescriptorSIFTName = "SIFT";
    const std::string kDescriptorSIFTDiameterSizeKeyPath = "diameterSize";
    
    //external solver
    const std::string kExternalSolverParameterKeyPath = "externalSolverParameters";
    const std::string kExternalSolverTmpInputMatrixDirectoryKeyPath = "inputMatrixDirectory";
    const std::string kExternalSolverTmpOutputMatrixDirectoryKeyPath = "outputMatrixDirectory";
}
