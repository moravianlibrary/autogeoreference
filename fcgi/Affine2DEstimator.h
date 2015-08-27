/* 
 * File:   Affine2DEstimator.h
 * Author: dudae
 *
 * Created on Štvrtok, 2015, júla 23, 12:58
 */

#ifndef AFFINE2DESTIMATOR_H
#define	AFFINE2DESTIMATOR_H

#include <opencv2/opencv.hpp>

class Affine2DEstimator  
{  
public:  
    Affine2DEstimator();  
    int runKernel( const CvMat* m1, const CvMat* m2, CvMat* model );   
    bool runRANSAC( const CvMat* m1, const CvMat* m2, CvMat* model,  
        CvMat* mask, double threshold,  
        double confidence=0.99, int maxIters=2000 );  
    bool getSubset( const CvMat* m1, const CvMat* m2,  
        CvMat* ms1, CvMat* ms2, int maxAttempts=1000 );  
    bool checkSubset( const CvMat* ms1, int count );  
    int findInliers( const CvMat* m1, const CvMat* m2,  
        const CvMat* model, CvMat* error,  
        CvMat* mask, double threshold );  
    void computeReprojError( const CvMat* m1, const CvMat* m2, const CvMat* model, CvMat* error );   
protected:  
    CvRNG rng;  
    int modelPoints;  
    CvSize modelSize;  
    int maxBasicSolutions;  
    bool checkPartialSubsets;  
};  
  
  
  
int estimateAffine2D(cv::InputArray _from, cv::InputArray _to,  
    cv::OutputArray _out, cv::OutputArray _inliers,  
    double param1=3, double param2=0.99);

#endif	/* AFFINE2DESTIMATOR_H */

