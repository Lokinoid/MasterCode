//
//  DescriptorTester.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 05/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "DescriptorTester.h"

#include <time.h>
#include <stdlib.h>

#include "StarFeature.h"
#include "FileFinder.h"
#include "MatHelper.h"
#include "HOGDescriptor.h"


void DescriptorTester::StartTest()
{
  const cv::Mat image = cv::imread("/Users/stanislav/Documents/Images/Faces/Aligned/test_image0.jpg");
  const cv::Mat points = MatHelper::pointToMat(cv::Point(5,5));
  
  DescriptorTester tester;
  tester.ShowDescriptors(image, points);
}

const cv::Mat DescriptorTester::loadImage(const CFileFinder& fileFinder)
{
  int nextRand = rand () % fileFinder.GetFileNames()->size();
  cv::Mat image = cv::imread(fileFinder.GetFileNames()->at(nextRand));
  if (image.empty())
  {
    CV_Assert(0);
  }
  return image;
}


void DescriptorTester::ShowDescriptors(const cv::Mat& image, const cv::Mat& points)
{
  const PointArrayPtr pointArray = MatHelper::matToPoints(points);
  
  const int kWindowSize = 24;
  const int kCellSize = 12;
  const int bins = 9;
  descriptor::HOGDescriptor descriptorExtractor(cv::Size(kWindowSize,kWindowSize),
                                                cv::Size(kCellSize,kCellSize),
                                                cv::Size(kCellSize,kCellSize),
                                                bins);
  
  
  for (const PointType& point : *pointArray)
  {
    cv::Mat descriptors = descriptorExtractor.ComputeDescriptor(image, MatHelper::pointToMat(point));
    const cv::Rect rect = MatHelper::rectAtPoint(point, image.size(), descriptorExtractor.WindowSize());
    
    cv::Mat colorImagePart = image(rect);
    cv::Mat newPartImage = createVisualImage(colorImagePart ,
                                             std::vector<double>(descriptors),
                                             descriptorExtractor.WindowSize(),
                                             descriptorExtractor.CellSize(),
                                             10);
    cv::imshow("imagePart", newPartImage);
    
    cv::Mat newImage = addPointArea(image, rect);
    cv::imshow("image", newImage);
    cv::waitKey();
  }
  
}

const cv::Mat DescriptorTester::addPointArea(const cv::Mat& image,
                                             const cv::Rect& rect)
{
  cv::Mat newImage(image);
  cv::rectangle(newImage, rect, cv::Scalar(255, 0, 0));
  
  return newImage;
}

const cv::Mat DescriptorTester::createVisualImage(const cv::Mat& origImg,
                                                  const std::vector<double>& descriptorValues,
                                                  const cv::Size winSize,
                                                  const cv::Size cellSize,
                                                  const int scaleFactor)
{
  double vizFactor = 1;
  
  cv::Mat visual_image;
  cv::resize(origImg, visual_image, cv::Size(origImg.cols*scaleFactor, origImg.rows*scaleFactor));
  
  int gradientBinSize = 9;
  // dividing 180° into 9 bins, how large (in rad) is one bin?
  double radRangeForOneBin = 3.14/(double)gradientBinSize;
  
  // prepare data structure: 9 orientation / gradient strenghts for each cell
	int cells_in_x_dir = winSize.width / cellSize.width;
  int cells_in_y_dir = winSize.height / cellSize.height;
  double*** gradientStrengths = new double**[cells_in_y_dir];
  int** cellUpdateCounter   = new int*[cells_in_y_dir];
  for (int y=0; y<cells_in_y_dir; y++)
  {
    gradientStrengths[y] = new double*[cells_in_x_dir];
    cellUpdateCounter[y] = new int[cells_in_x_dir];
    for (int x=0; x<cells_in_x_dir; x++)
    {
      gradientStrengths[y][x] = new double[gradientBinSize];
      cellUpdateCounter[y][x] = 0;
      
      for (int bin=0; bin<gradientBinSize; bin++)
        gradientStrengths[y][x][bin] = 0.0;
    }
  }
  
  // nr of blocks = nr of cells - 1
  // since there is a new block on each cell (overlapping blocks!) but the last one
  int blocks_in_x_dir = cells_in_x_dir - 1;
  int blocks_in_y_dir = cells_in_y_dir - 1;
  
  // compute gradient strengths per cell
  int descriptorDataIdx = 0;
  
  for (int blockx=0; blockx<blocks_in_x_dir; blockx++)
  {
    for (int blocky=0; blocky<blocks_in_y_dir; blocky++)
    {
      // 4 cells per block ...
      for (int cellNr=0; cellNr<4; cellNr++)
      {
        // compute corresponding cell nr
        int cellx = blockx;
        int celly = blocky;
        if (cellNr==1) celly++;
        if (cellNr==2) cellx++;
        if (cellNr==3)
        {
          cellx++;
          celly++;
        }
        
        for (int bin=0; bin<gradientBinSize; bin++)
        {
          double gradientStrength = descriptorValues[ descriptorDataIdx ];
          descriptorDataIdx++;
          
          gradientStrengths[celly][cellx][bin] += gradientStrength;
          
        } // for (all bins)
        
        
        // note: overlapping blocks lead to multiple updates of this sum!
        // we therefore keep track how often a cell was updated,
        // to compute average gradient strengths
        cellUpdateCounter[celly][cellx]++;
        
      } // for (all cells)
      
      
    } // for (all block x pos)
  } // for (all block y pos)
  
  
  // compute average gradient strengths
  for (int celly=0; celly<cells_in_y_dir; celly++)
  {
    for (int cellx=0; cellx<cells_in_x_dir; cellx++)
    {
      
      double NrUpdatesForThisCell = (double)cellUpdateCounter[celly][cellx];
      
      // compute average gradient strenghts for each gradient bin direction
      for (int bin=0; bin<gradientBinSize; bin++)
      {
        gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
      }
    }
  }
  
  
  //  std::cout << "descriptorDataIdx = " << descriptorDataIdx << std::endl;
  
  // draw cells
  for (int celly=0; celly<cells_in_y_dir; celly++)
  {
    for (int cellx=0; cellx<cells_in_x_dir; cellx++)
    {
      int drawX = cellx * cellSize.width;
      int drawY = celly * cellSize.height;
      
      int mx = drawX + cellSize.width/2;
      int my = drawY + cellSize.height/2;
      
      cv::rectangle(visual_image,
                    cv::Point(drawX*scaleFactor,drawY*scaleFactor),
                    cv::Point((drawX+cellSize.width)*scaleFactor,
                              (drawY+cellSize.height)*scaleFactor),
                    cv::Scalar(100,100,100),
                    1);
      
      // draw in each cell all 9 gradient strengths
      for (int bin=0; bin<gradientBinSize; bin++)
      {
        double currentGradStrength = gradientStrengths[celly][cellx][bin];
        
        // no line to draw?
        if (currentGradStrength==0)
          continue;
        
        double currRad = bin * radRangeForOneBin + radRangeForOneBin/2;
        
        double dirVecX = cos( currRad );
        double dirVecY = sin( currRad );
        double maxVecLen = cellSize.width/2;
        double scale = vizFactor; // just a visual_imagealization scale,
        // to see the lines better
        
        // compute line coordinates
        double x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
        double y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
        double x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
        double y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;
        
        // draw gradient visual_imagealization
        cv::line(visual_image,
                 cv::Point(x1*scaleFactor,y1*scaleFactor),
                 cv::Point(x2*scaleFactor,y2*scaleFactor),
                 cv::Scalar(0,0,255),
                 1);
        
      } // for (all bins)
      
    } // for (cellx)
  } // for (celly)
  
  
  // don't forget to free memory allocated by helper data structures!
  for (int y=0; y<cells_in_y_dir; y++)
  {
    for (int x=0; x<cells_in_x_dir; x++)
    {
      delete[] gradientStrengths[y][x];
    }
    delete[] gradientStrengths[y];
    delete[] cellUpdateCounter[y];
  }
  delete[] gradientStrengths;
  delete[] cellUpdateCounter;
  
  return visual_image;
}
