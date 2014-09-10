//
//  ImagePainter.h
//  SDMOpenCVClient
//
//  Created by Stanislav Orlov on 21/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __SDMOpenCVClient__ImagePainter__
#define __SDMOpenCVClient__ImagePainter__

#include <opencv2/opencv.hpp>


class ImagePainter
{
public:
  
  const cv::Mat DrawLandmarks(const cv::Mat* image, const cv::Mat* landmarks);
  
private:
  const cv::Mat addLandmarksOnImage(const cv::Mat* image,
                                    const cv::Mat* position,
                                    const int thickness);

};

#endif /* defined(__SDMOpenCVClient__ImagePainter__) */
