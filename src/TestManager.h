//
//  TestManager.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 01/05/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__TestManager__
#define __ClassifierTrainer__TestManager__

#include <iostream>
#include <vector>
#include <string>


class TestManager
{
public:
  
  static void StartTests(const std::string& trainedDataFilePath);
  
private:

  TestManager(const std::string& trainedDataFilePath);
  void Start(std::vector<std::pair<std::string, std::string>>& directories);

  std::string m_detectorDataFilePath;
};

#endif /* defined(__ClassifierTrainer__TestManager__) */
