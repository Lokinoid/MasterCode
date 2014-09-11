//
//  main.cpp
//  OpenCVTest2
//
//  Created by Stanislav Orlov on 18/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//


#include "MainController.h"


int main()
{
    try
    {
		MainController controller;
		controller.StartWithConfigurationFilePath("../config.yml");
    }
    catch (std::exception e)
    {
        std::cout<< "Exception: " << e.what() << std::endl;
    }
    
    return 0;
}
