////
////  ProcrustesTransformer.cpp
////  ClassifierTrainer
////
////  Created by Stanislav Orlov on 15/06/14.
////  Copyright (c) 2014 Stanislav. All rights reserved.
////
//
//#include "ProcrustesTransformer.h"
//#include "LandmarkCollectionData.h"
//
//
//void ProcrustesTransformer::Transform(const LandmarkCollectionDataPtr& src,
//                                      LandmarkCollectionDataPtr& dst,
//                                      const std::string& imageDirectory,
//                                      const std::string& landmarkDataPath)
//{
//    ProcrustesTransformer transformer(imageDirectory,
//                                      landmarkDataPath);
//    transformer.transform(src, dst);
//}
//
//ProcrustesTransformer::ProcrustesTransformer(const std::string& imageDirectory,
//                                             const std::string& landmarkDataPath)
//: m_imageDirectory(imageDirectory),
//  m_landmarkDataPath(landmarkDataPath)
//{
//
//}
//
//void ProcrustesTransformer::transform(const LandmarkCollectionDataPtr& src,
//                                      LandmarkCollectionDataPtr& dst)
//{
//    ImageLandmarkDataPtr idealImageData = src->at(0);
//    
//    for (int i = 1; i < src->CollectionSize(); i++)
//    {
//        const ImageLandmarkDataPtr alignedData = alignToImageData(src->at(i), idealImageData);
//    }
//}
//
//const ImageLandmarkDataPtr ProcrustesTransformer::alignToImageData(const ImageLandmarkDataPtr& imageData,
//                                                                   const ImageLandmarkDataPtr& idealData)
//{
//    
//}
//
//float ProcrustesTransformer::procrustes(const cv::Mat& X, const cv::Mat& Y)
//{
//    /* Recenter the points based on their mean ... */
//    cv::Scalar mu_x = cv::mean(X);
//    Mat X0      = X - Mat(X.size(), X.type(), mu_x);
//    
//    Scalar mu_y = cv::mean(Y);
//    Mat Y0      = Y - Mat(Y.size(), Y.type(), mu_y);
//    
//    /* ... and normalize them */
//    float ss_X      = sumSquared( X0 );
//    float norm_X    = sqrt( ss_X );
//    X0              /= norm_X;
//    
//    float ss_Y      = sumSquared( Y0 );
//    float norm_Y    = sqrt( ss_Y );
//    Y0              /= norm_Y;
//    
//    /* Pad with zeros is Y has less points than X */
//    if( Y.rows < X.rows )
//        vconcat( Y0, Mat::zeros( X.rows - Y.rows, 1, Y.type()), Y0 );
//    
//    /* Perform SVD */
//    Mat A = X0.reshape(1).t() * Y0.reshape(1);
//    Mat U, s, Vt;
//    SVDecomp( A, s, U, Vt );
//    
//    Mat V           = Vt.t();
//    this->rotation  = V * U.t();
//    
//    if( !bestReflection ) {
//        bool have_reflection = determinant( this->rotation ) < 0;
//        if( bestReflection != have_reflection ) {
//            V.colRange( V.cols-1, V.cols) *= -1;
//            s.rowRange( s.rows-1, s.rows) *= -1;
//            this->rotation = V * U.t();
//        }
//    }
//    
//    /* Rotate Y0 first */
//    Mat rotated_Y0;
//    cv::transform( Y0, rotated_Y0, rotation.t() );
//    
//    /* Trace of eigenvalues is basically the scale */
//    float trace_TA = sum( s )[0];
//    
//    if( scaling ) {
//        scale   = trace_TA * norm_X / norm_Y;
//        error   = 1 - trace_TA * trace_TA;
//        Yprime  = norm_X * trace_TA * rotated_Y0 + mu_x;
//    }
//    else {
//        error   = 1 + ss_Y / ss_X - 2 * trace_TA * norm_X / norm_Y;
//        Yprime  = norm_Y * rotated_Y0 + mu_x;
//    }
//    
//    if( Y.rows < X.rows )
//        rotation = rotation.rowRange(0, Y.rows );
//    
//    translation = Mat(1, 1, CV_32FC2, mu_x).reshape(1) - scale * Mat(1, 1, CV_32FC2, mu_y).reshape(1) * rotation;
//    
//    return error;
//}
