//
//  LLSSolver.h
//  ClassifierTrainer
//
//  Created by Stanislav Orlov on 03/03/14.
//  Copyright (c) 2014 Stanislav. All rights reserved.
//

#ifndef __ClassifierTrainer__LLSSolver__
#define __ClassifierTrainer__LLSSolver__


#include <opencv2/opencv.hpp>
#include "CommonTypes.h"


class EquationData;

//@brief Data for equation Y = Xb
//
struct LLSEquation
{
    cv::Mat X;
    cv::Mat Y;
    LLSEquation(const cv::Mat& x, const cv::Mat& y): X(x), Y(y) {}
};

//@brief Class to solve matrix equations.
//       
//       Now 2 possible kinds of work supported: solve overloaded linear lest squares equation via OpenCV or
//       with external solver (Matlab recommended).
//       If external solver is chosen then LLSSolver persists matrixes into a set of files and
//       waits for the external solvation (for example, by Matlab).
//       External solver should persist the answers b_i to another set of files and after that
//       current class loads those answers and buils the final solvation matrix b.
//
class LLSSolver
{
public:
    
    typedef std::shared_ptr<LLSEquation> LLSEquationPtr;
    typedef std::vector<LLSEquationPtr> LLSEquationArray;
    typedef std::shared_ptr<LLSEquationArray> LLSEquationArrayPtr;

    //@brief Solves equations in equation array.
    //@param equationArray
    //       Array of equation to solve.
    //       shouldLoadMatrixNotSolve
    //       Indicates if equations should be solved or not.
    const cv::Mat Solve(const EquationArrayPtr& equationArray, bool useExternalSolver);

private:
    
    const LLSEquationArrayPtr generateLLSEquationArray(const EquationArrayPtr& equationArray);
    const cv::Mat solveLLSEquation(const LLSEquationPtr& llsEquation);
    const cv::Mat createMatX(const EquationArrayPtr& equationArray);
    const cv::Mat createMatY(const EquationArrayPtr& equationArray, const int lineNumber);
    
    void PrintLLSEquationArray(const LLSEquationArrayPtr& equationArray);
    
};

#endif /* defined(__ClassifierTrainer__LLSSolver__) */
