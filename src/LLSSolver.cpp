//
//  LLSSolver.cpp
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 03/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#include "LLSSolver.h"
#include <stdlib.h>

#include "EquationData.h"
#include "FileDataPersister.h"

#include "ExternalSolverConfig.h"
#include "ConfigurationManager.h"


//void showCurrentTime()
//{
//    time_t t = time(0);   // get time now
//    struct tm * now = localtime( & t );
//    std::cout << now->tm_hour << " : "
//    << now->tm_min << " : "
//    << now->tm_sec
//    << std::endl;
//}

const cv::Mat LLSSolver::Solve(const EquationArrayPtr& equationArray, bool useExternalSolver)
{
    cv::Mat_<double> matB;
    
    LLSEquationArrayPtr llsEquationArray = generateLLSEquationArray(equationArray);
    FileDataPersister persister;
    
    //**************************************************************************************************
    // START USING MATLAB ONLY
    // The main idea is to let Matlab solve all the equations. Computation time decreases greatly.
    //**************************************************************************************************
    
    if (useExternalSolver)
    {
        // flag rules the state of Matlab computation.
        // If Matlab's already finished its computations
        static bool shouldPersistEquation = true; // false if matlab has calculated B_i matrices
        if (shouldPersistEquation)
        {
            std::cout<<"Persisting data"<<std::endl;
            
            persister.PersistData(ConfigurationManager::ExternalSolverConfiguration()->InputMatrixDirectory(), llsEquationArray);
        }
        shouldPersistEquation = true;
        
        //**************************************************************************************************
        // WAIT UNTIL MATLAB FINISHES ITS WORK !!!
        //**************************************************************************************************
        
        std::cout<<"Loading data"<<std::endl;
        
        persister.LoadData(ConfigurationManager::ExternalSolverConfiguration()->OutputMatrixDirectory(), matB);
        std::cout<<matB.rows<< " " <<matB.cols<<std::endl;
        return matB;
    }
    
    //**************************************************************************************************
    // FINISH USING MATLAB ONLY
    //**************************************************************************************************
    
    //  PrintLLSEquationArray(llsEquationArray);
    
    std::cout<<"LLS equations generated"<<std::endl;
    
    for (LLSEquationPtr llsEquation : *llsEquationArray)
    {
        cv::Mat nextRow = solveLLSEquation(llsEquation).t();
        
        static double i = 0;
        std::cout<<"LLS equations solved, %: "<<  ++i / llsEquationArray->size() << std::endl;
        
        matB.push_back(nextRow);
    }
    
    return matB;
}

const LLSSolver::LLSEquationArrayPtr LLSSolver::generateLLSEquationArray(const EquationArrayPtr& equationArray)
{
    const int kNumberOfEquations = equationArray->at(0)->CurrentPosition().rows;
    
    LLSEquationArrayPtr llsEquationArray = LLSEquationArrayPtr(new LLSEquationArray);
    llsEquationArray->reserve(kNumberOfEquations);
    
    const cv::Mat matX = createMatX(equationArray);
    int lineNumber = 0;
    for (int i = 0; i < kNumberOfEquations; i++)
    {
        cv::Mat matY = createMatY(equationArray, lineNumber++);
        LLSEquationPtr equation = LLSEquationPtr(new LLSEquation(matX, matY));
        llsEquationArray->push_back(equation);
    }
    
    return llsEquationArray;
}

const cv::Mat LLSSolver::createMatX(const EquationArrayPtr& equationArray)
{
    cv::Mat_<double> descriptors;
    descriptors.reserve(equationArray->size());
    
    for (EquationDataPtr equation : *equationArray)
    {
        cv::Mat transposedDescriptor = equation->Descriptor().t();
        descriptors.push_back(transposedDescriptor);
    }
    
    return descriptors;
}

const cv::Mat LLSSolver::createMatY(const EquationArrayPtr& equationArray, const int lineNumber)
{
    cv::Mat result;
    result.reserve(equationArray->size());
    
    for (EquationDataPtr equation : *equationArray)
    {
        double value = equation->DeltaPosition().at<double>(lineNumber, 0);
        result.push_back(value);
    }
    
    return result;
}

const cv::Mat LLSSolver::solveLLSEquation(const LLSEquationPtr& llsEquation)
{
    // Y = Xb ->
    // X'Xb = X'Y ->
    // b = inv(X'X)X'Y
    
    cv::Mat X = llsEquation->X;
    cv::Mat Y = llsEquation->Y;
    
    cv::Mat b;
    
    //showCurrentTime();
    bool isValid = cv::solve(X, Y, b, cv::DECOMP_SVD);
    
    if (!isValid)
    {
        //    CV_Assert(0);
        std::cout<<"Not valid answer"<<std::endl;
    }
    
    return b;
}

void LLSSolver::PrintLLSEquationArray(const LLSEquationArrayPtr& equationArray)
{
    for (LLSEquationPtr equation : *equationArray)
    {
        std::cout << "X: \n" << equation->X << std::endl;
        std::cout << "Y: \n" << equation->Y << std::endl;
        
    }
}
