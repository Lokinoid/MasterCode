//
//  StatisticTester.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 22/04/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "StatisticTester.h"
#include <opencv2/opencv.hpp>

#include "StatisticProduser.h"


void StatisticTester::Test()
{
  StatisticTester tester;
  tester.startTest();
}


StatisticTester::StatisticTester()
{
}

void StatisticTester::startTest()
{
  cv::Mat mat1 = cv::Mat::ones(98, 1, CV_64FC1);
  cv::Mat mat2 = 2 * cv::Mat::ones(98, 1, CV_64FC1);
  cv::Mat mistake = statistics::StatisticProduser::AbsMistake(mat1, mat2);
  
  cv::Mat hist = statistics::StatisticProduser::Hist(mistake, 0, 10, 9);
  std::cout<<hist<<std::endl;
  
//  std::string res = int(result) == 69 ? "Passed" : "Not Passed";
//  std::cout<<res<<std::endl;
}
