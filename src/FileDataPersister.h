//
//  FileDataPersister.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 12/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__FileDataPersister__
#define __ClassifierTrainer__FileDataPersister__

#include <string>
#include <fstream>
#include "CommonTypes.h"
#include "LLSSolver.h"


//@brief Class for data persistence.
//
class FileDataPersister
{
public:
    
    //@brief Save landmarks into the file.
    //@param filePath
    //       Path of file to save.
    //@param collection
    //       Set of landmarks for each image.
    void PersistData(const std::string& filePath,
                     const LandmarkCollectionDataPtr& collection);
    
    //@brief Save equations to the set of files. File names are generated automatically.
    //@param directoryPath
    //       Path of directory to save.
    //@param equationArray
    //       Set of equations to save.
    void PersistData(const std::string& directoryPath,
                     const LLSSolver::LLSEquationArrayPtr& equationArray);
    
    //@brief Loading data and fills the result matrix.
    //       Used to unserialize matrix from set of matrixes that are made by external linear lest squares solver (Matlab).
    //@param directoryPath
    //       Directory where the matrixes are.
    //@param matrix
    //       Output matrix to fill.
    void LoadData(const std::string& directoryPath,
                  cv::Mat& matrix);
    
private:
    
    const std::string nextString();
    
    std::ofstream createFile(const std::string& filePath);
    const std::string createLineFormLandmarkData(const ImageLandmarkDataPtr& landmarkData);
    
    void persistEquation(const std::string& directoryPath, const LLSSolver::LLSEquationPtr& equation, const int counter);
    void writeMatToFile(const cv::Mat& mat, const std::string& filePath);
    const std::string generatePath(const std::string& directoryPath,
                                   const std::string& prefix,
                                   int counter);
    
};

#endif /* defined(__ClassifierTrainer__FileDataPersister__) */
