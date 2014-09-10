//
//  ClassifierTrainer.cpp
//  ClassifierTrainer
//
//  Created by Stanislav on 23.02.14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "ClassifierTrainer.h"

#include <algorithm>
#include <ctime>

#include "ImageLandmarkData.h"
#include "StarFeature.h"
#include "FaceFeature.h"
#include "EquationData.h"
#include "TrainedData.h"
#include "LLSSolver.h"
#include "MatHelper.h"
#include "TestFunctionFeature.h"
#include "FileDataPersister.h"


void ClassifierTrainer::TrainDetector(const LandmarkCollectionDataPtr& collectionData,
                                      const FeaturePtr& feature,
                                      const DescriptorPtr& descriptor,
                                      const int numberOfTrainIterations,
                                      const std::string& detectorDataFilePath,
                                      bool shouldLoadMatrixNotSolve)
{
    trainClassifier(collectionData,
                    NULL,
                    feature,
                    descriptor,
                    numberOfTrainIterations,
                    detectorDataFilePath,
                    shouldLoadMatrixNotSolve);
    
}

void ClassifierTrainer::TrainTracker(const LandmarkCollectionDataPtr& collectionData,
                                     const LandmarkCollectionDataPtr& detectedData,
                                     const FeaturePtr& feature,
                                     const DescriptorPtr& descriptor,
                                     const int numberOfTrainIterations,
                                     const std::string& trackerDataFilePath,
                                     bool shouldLoadMatrixNotSolve)
{
    trainClassifier(collectionData,
                    detectedData,
                    feature,
                    descriptor,
                    numberOfTrainIterations,
                    trackerDataFilePath,
                    shouldLoadMatrixNotSolve);
}

void ClassifierTrainer::trainClassifier(const LandmarkCollectionDataPtr& collectionData,
                                        const LandmarkCollectionDataPtr& detectedData,
                                        const FeaturePtr& feature,
                                        const DescriptorPtr& descriptor,
                                        const int numberOfTrainIterations,
                                        const std::string& savedTrainedDataFilePath,
                                        bool shouldLoadMatrixNotSolve)
{
    m_collectionData = collectionData;
    m_feature = feature;
    m_descriptor = descriptor;
    m_shouldLoadMatrixNotSolve = shouldLoadMatrixNotSolve;
    
    EquationArrayPtr equationArray = createEquationArray(collectionData);
    std::cout<<"Equation array created"<<std::endl;
    
    PositionArrayPtr currentPosition;
    bool isTracker = detectedData == NULL;
    if (!isTracker)
    {
        currentPosition = startPositionArray(equationArray->size());
        std::cout<<"Start position array created"<<std::endl;
    }
    else
    {
        currentPosition = detectedPositionArray(detectedData);
        std::cout<<"Detected position array created"<<std::endl;
    }
    
    TrainedData trainedData;
    trainedData.Load(savedTrainedDataFilePath); // try to load already calculated data
    
    for (int i = 0; i < numberOfTrainIterations; i++)
    {
        std::cout<<"Iteration "<<i<<std::endl;
        updateEquationWithCurrentPosition(equationArray, currentPosition);
        
        std::cout<<"Equation updated"<<std::endl;
        
        cv::Mat resMat = trainedData.at(i);
        if (resMat.empty())
        {
            std::cout<<"Empty mat. Solving"<<std::endl;
            
            resMat = solveEquationArray(equationArray);
            trainedData.AddData(resMat, true, savedTrainedDataFilePath);
        }
        
        currentPosition = nextPosition(equationArray, resMat);
    }
}

const EquationArrayPtr ClassifierTrainer::createEquationArray(const LandmarkCollectionDataPtr& collectionData)
{
    const int kNumberOfLandmarks = 49 * 2;
    EquationArrayPtr equationArray = EquationArrayPtr(new EquationArray);
    equationArray->reserve(collectionData->CollectionSize());
    
    collectionData->EnumerateColectionWithCallback([&] (ImageLandmarkDataPtr& landmarkData) {
        
        const cv::Mat bestPosition = landmarkData->LandmarksMat();
        
        if (bestPosition.rows != kNumberOfLandmarks || bestPosition.cols != 1)
        {
            std::cout<<landmarkData->ImagePath()<<std::endl;
            CV_Assert(bestPosition.rows != 98 || bestPosition.cols != 1);
        }
        
        EquationDataPtr equation = EquationDataPtr(new EquationData);
        equation->SetBestPosition(bestPosition);
        equationArray->push_back(equation);
    });
    
    return equationArray;
}

const cv::Mat ClassifierTrainer::solveEquationArray(const EquationArrayPtr& equationArray)
{
    LLSSolver solver;
    const cv::Mat result = solver.Solve(equationArray, m_shouldLoadMatrixNotSolve);
    
    return result;
}

const ClassifierTrainer::PositionArrayPtr ClassifierTrainer::startPositionArray(const size_t size)
{
    PositionArrayPtr positions = PositionArrayPtr(new PositionArrayType);
    positions->reserve(size);
    
    int failCounter = 0;
    m_collectionData->EnumerateConstColectionWithCallback([&] (const ImageLandmarkDataPtr& landmarkData, const int, bool*) {
        
        const cv::Mat startPosition = m_feature->StartPosition(landmarkData->ImageSource());
        if (startPosition.empty())
        {
            failCounter++;
            std::cout<<"Fail image: "<<landmarkData->ImagePath()<<std::endl;
        }
        positions->push_back(startPosition);
    });
    
    CV_Assert(failCounter == 0);
    
    return positions;
}

const ClassifierTrainer::PositionArrayPtr ClassifierTrainer::detectedPositionArray(const LandmarkCollectionDataPtr& detectedData)
{
    CV_Assert(detectedData);
    
    PositionArrayPtr positions = PositionArrayPtr(new PositionArrayType);
    positions->reserve(detectedData->CollectionSize());
    
    m_collectionData->EnumerateConstColectionWithCallback([&] (const ImageLandmarkDataPtr& landmarkData, const int, bool*) {
        
        detectedData->EnumerateConstColectionWithCallback([&] (const ImageLandmarkDataPtr& detectedLandmarkData, const int index, bool* stop) {
            
            if (detectedLandmarkData->ImagePath() == landmarkData->ImagePath())
            {
                const cv::Mat startPosition = detectedLandmarkData->LandmarksMat();
                positions->push_back(startPosition);
                *stop = true;
            }
        });
    });
    
    CV_Assert(positions->size() > 0);
    
    return positions;
}

const ClassifierTrainer::PositionArrayPtr ClassifierTrainer::nextPosition(EquationArrayPtr& equationArray,
                                                                          const cv::Mat& mat)
{
    PositionArrayPtr positions = PositionArrayPtr(new PositionArrayType);
    positions->reserve(equationArray->size());
    
    for (int i = 0; i < equationArray->size(); i++)
    {
        EquationDataPtr equation = equationArray->at(i);
        ImageLandmarkDataPtr landmarkData = m_collectionData->at(i);
        
        CV_Assert(mat.depth() == CV_64FC1);
        CV_Assert(mat.cols == equation->Descriptor().rows);
        CV_Assert(mat.rows == equation->DeltaPosition().rows);
        
        // check if current image data suits equation
        const cv::Mat landmarksMat = landmarkData->LandmarksMat();
        const cv::Mat equationLandmarksMat = equation->BestPosition();
        const cv::Mat diff = landmarksMat != equationLandmarksMat;
        bool equiv = cv::countNonZero(diff) == 0;
        CV_Assert(equiv);
        
        const cv::Mat_<int> position = equation->CurrentPosition() + mat * equation->Descriptor();
        
        cv::Mat correctPosition = checkAndConvertPosition(position, landmarkData->ImageSize());
        //    std::cout<<"Position: "<< position<<std::endl;
        
        positions->push_back(correctPosition);
    }
    
    return positions;
}

const cv::Mat ClassifierTrainer::checkAndConvertPosition(const cv::Mat& position,
                                                         const cv::Size& imageSize)
{
    const int maxSizeX = imageSize.width - 1;
    const int maxSizeY = imageSize.height - 1;
    
    cv::Mat_<double> resultPosition(position.size(), CV_64FC1);
    double currentX;
    double currentY;
    for (int i = 0; i < position.rows; i+=2)
    {
        currentX = static_cast<double>(MIN(MAX(0, position.at<int>(i,0)), maxSizeX));
        currentY = static_cast<double>(MIN(MAX(0, position.at<int>(i + 1,0)), maxSizeY));
        
        resultPosition(i,0) = currentX;
        resultPosition(i + 1,0) = currentY;
    }
    
    return resultPosition;
}

void ClassifierTrainer::updateEquationWithCurrentPosition(EquationArrayPtr& equationArray,
                                                          const PositionArrayPtr& currentPositions)
{
    int i = 0;
    
    m_collectionData->EnumerateColectionWithCallback([&] (ImageLandmarkDataPtr& landmarkData) {
        
        equationArray->at(i)->SetCurrentPosition(currentPositions->at(i));
        const cv::Mat descriptor = m_descriptor->ComputeDescriptor(landmarkData->ImageSource(), currentPositions->at(i));
        equationArray->at(i)->SetDescriptor(descriptor);
        i++;
    });
}

void ClassifierTrainer::printEquationArray(const EquationArrayPtr& equationArray)
{
    if (!equationArray || equationArray->size() == 0)
    {
        return;
    }
    
    std::cout<<"\nEquation array:\n";
    
    for (EquationDataPtr equation : *equationArray)
    {
        std::cout<<"Best position: "<<equation->BestPosition().rows <<" "<<equation->BestPosition().cols <<std::endl;
        std::cout<< equation->BestPosition() <<std::endl;
        
        std::cout<<"Current position: "<<equation->CurrentPosition().rows <<" "<<equation->CurrentPosition().cols <<std::endl;
        std::cout<< equation->CurrentPosition() <<std::endl;
        
        std::cout<<"Descriptor: "<<equation->Descriptor().rows <<" "<<equation->Descriptor().cols <<std::endl;
        std::cout<< equation->Descriptor() <<std::endl;
    }
}
