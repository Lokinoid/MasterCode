//
//  main.cpp
//  OpenCVTest2
//
//  Created by Stanislav Orlov on 18/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//


#include "MainController.h"
#include "FileDataPersister.h"
#include "TrainedDataTest.h"
#include "StatisticTester.h"
#include "TrainingTest.h"
#include "TestManager.h"
#include "ConfigurationManager.h"
#include "ConfigurationTester.h"


void TrainFace()
{
    const std::string kConfigurationFilePath = "../config.yml";
    MainController controller;
    controller.StartWithConfigurationFilePath(kConfigurationFilePath);
    
//    
//    controller.Start(MainController::ProcessTypeTrain,
//                     FeatureTypeFace,
//                     PathKeys::kFaceDetectorDataFilePathXml,
//                     //                   PathKeys::kFaceTrackerDataFilePathXml,
//                     //                   PathKeys::kFaceCollectionDataFilePath,
//                     //                   PathKeys::kTrainFacesImageDirectory);
//                     PathKeys::kTrainAlignedFaceCollectionDataFilePath,
//                     PathKeys::kAlignedFacesImageDirectory);
//    //                   PathKeys::kTestLandmarkDataFilePath4,
//    //                   PathKeys::kTestImageDirectory4);
}

void TestFace()
{
    MainController controller;
//    controller.Start(MainController::ProcessTypeTest,
//                     FeatureTypeFace,
//                     PathKeys::kFaceDetectorDataFilePathXml,
//                     //                   PathKeys::kFaceTrackerDataFilePathXml,
//                     //                   PathKeys::kFaceCollectionDataFilePath,
//                     //                   PathKeys::kTrainFacesImageDirectory);
//                     PathKeys::kTestAlignedFaceCollectionDataFilePath,
//                     PathKeys::kAlignedFacesImageDirectory);
//    //                   PathKeys::kTestLandmarkDataFilePath3,
//    //                   PathKeys::kTestImageDirectory3);
}

void GlobalTest()
{
    //  TestManager::StartTests("/Users/stanislav/Dropbox/Master_Thesis/Code/FaceDetectorData_HOGnew_16_8_9.xml");
    //  TestManager::StartTests("/Users/stanislav/Dropbox/Master_Thesis/Code/FaceDetectorData_HOGnew_24_8_9.xml");
    //  TestManager::StartTests("/Users/stanislav/Dropbox/Master_Thesis/Code/FaceDetectorData_HOGnew_32_16_9.xml");
    //  TestManager::StartTests("/Users/stanislav/Dropbox/Master_Thesis/Code/FaceDetectorData_HOGnew_48_16_9.xml");
    TestManager::StartTests("/Users/stanislav/Dropbox/Master_Thesis/Code/FaceDetectorData_SIFT_20.xml");
    TestManager::StartTests("/Users/stanislav/Dropbox/Master_Thesis/Code/FaceDetectorData_SIFT_28.xml");
}

int main()
{
    
    try
    {
        TrainFace();
        //    TestFace();
  
//        ConfigurationTester::StartTest();
        
//        GlobalTest();
        
        //    TrainedDataTest::Test();
        //    StatisticTester::Test();
        //    TrainingTest::StartTest();
    }
    catch (std::exception e)
    {
        std::cout<< "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}
