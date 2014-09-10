//
//  ImageWriter.cpp
//  PreTrainImageProcessor
//
//  Created by Stanislav Orlov on 19/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ImageWriter.h"
#include <boost/filesystem.hpp>


static ImageWriter* writer;

ImageWriter::ImageWriter()
: m_counter(0)
{
}

ImageWriter::~ImageWriter()
{
  delete writer;
}

void ImageWriter::SaveImageWithPath(const std::string& path, const cv::Mat& image)
{
  cv::imwrite(path, image);
}

const std::string ImageWriter::GeneratePath(const std::string& directory)
{
  BOOST_ASSERT(directory.size() != 0);

  const std::string kImageKeyPath = "test_image";
  const std::string kImageExtentionKeyPath = ".jpg";
  
  std::string fileName = kImageKeyPath + std::to_string(m_counter++) + kImageExtentionKeyPath;
  boost::filesystem::path fullPath(directory);
  fullPath /= fileName;

  //! change
  // return fullPath.native();
  return std::string();
}

ImageWriter* ImageWriter::sharedInstance()
{
  static ImageWriter* writer;

  if (writer == NULL)
  {
    writer = new ImageWriter();
  }
  
  return writer;
}

