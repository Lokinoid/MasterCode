#pragma once

#include <vector>
#include <opencv2/opencv.hpp>
#include "DataContainer.h"


class string;

class CFileLinesParser
{
public:
   static void ParseFilesToContainerData( const std::vector<std::string>& i_fileNames,
                                          TDataContainer& o_dataContainer);
  
   static void ParseFilesToMatrixData( const std::vector<std::string>& i_fileNames,
                                       cv::Mat& o_mat);
};