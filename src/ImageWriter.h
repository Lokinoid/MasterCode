//
//  ImageWriter.h
//  PreTrainImageProcessor
//
//  Created by Stanislav Orlov on 19/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __PreTrainImageProcessor__ImageWriter__
#define __PreTrainImageProcessor__ImageWriter__


#include <string>
#include <opencv2/opencv.hpp>


class ImageWriter
{
public:
  
  static ImageWriter* sharedInstance();
  
  ~ImageWriter();
  void SaveImageWithPath(const std::string& path, const cv::Mat& image);
  
  const std::string GeneratePath(const std::string& directory);
  const std::string Directory() const;
  
private:
  
  ImageWriter();
  
  std::string m_directory;
  unsigned long m_counter;
};

#endif /* defined(__PreTrainImageProcessor__ImageWriter__) */
