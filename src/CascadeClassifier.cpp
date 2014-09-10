//
//  CascadeClassifier.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 31/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "CascadeClassifier.h"


CascadeClassifier::CascadeClassifier()
{
    m_cascadeClassifier = createCascadeClassifier();
}

const cv::Rect CascadeClassifier::DetectFace(const cv::Mat& image)
{
    std::vector<cv::Rect> faces;
	m_cascadeClassifier->detectMultiScale(image, faces, 1.2, 2, 0, cv::Size(50, 50));
    std::vector<cv::Rect>::iterator iter = std::max_element(faces.begin(), faces.end(), [&] (const cv::Rect& rect1, const cv::Rect rect2)
                                                          {
                                                            return rect1.width < rect2.width;
                                                          });
	if (iter == faces.end())
	{
		return cv::Rect(0,0,0,0);
	}
  
  return *iter;
}

const CascadeClassifier::CascadeClassifierPtr CascadeClassifier::createCascadeClassifier()
{
	std::string faceDetectionModel = "../haarcascade_frontalface_alt.xml";
	CV_Assert(faceDetectionModel.size() != 0);
  
	CascadeClassifierPtr faceCascade = CascadeClassifierPtr(new cv::CascadeClassifier);
	faceCascade->load(faceDetectionModel);
	CV_Assert(!faceCascade->empty());
  
	return faceCascade;
}
