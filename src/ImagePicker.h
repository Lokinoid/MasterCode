//
//  ImagePicker.h
//  OpenCVTest2
//
//  Created by Stanislav Orlov on 19/02/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __OpenCVTest2__ImagePicker__
#define __OpenCVTest2__ImagePicker__


#include <string>
#include <functional>
#include "DataContainer.h"
#include "CommonTypes.h"


class LandmarkCollectionData;

class ImagePicker
{
public:

  typedef std::vector<std::string> FileNameContainer;
  typedef std::shared_ptr<FileNameContainer> FileNameContainerPtr;

  ImagePicker(const std::string& imageDirectory);
  ~ImagePicker();
  
  const LandmarkCollectionDataPtr FetchImagesAndLandmarksWithProcess();
  const LandmarkCollectionDataPtr FetchLandmarkCollectionDataFromFile(std::string filePath);

  const FileNameContainerPtr ImagePaths();
  
private:
  
  const LandmarkCollectionDataPtr createLandmarkCollectionFormDataContainer(const TDataContainer& dataContainer);
  const std::string fullPathForFileName(const std::string& filename);

  const FileNameContainerPtr fetchFileNames();
  
  FileNameContainerPtr m_fileNames;
  std::string m_directory;
  
};

#endif /* defined(__OpenCVTest2__ImagePicker__) */
