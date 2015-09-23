/* 
 * File:   AutoGeoreference.cpp
 * Author: dudae
 * 
 * Created on Štvrtok, 2015, augusta 13, 12:17
 */

#include "AutoGeoreference.h"

#include <cstdio>

#include <GDALCorrelator.h>

#include "Affine2DEstimator.h"
#include "RequestException.h"

using namespace std;
using namespace cv;

Mat AutoGeoreference::findAffineMatrix(const cv::Mat& georeferencedImage, const cv::Mat& similarImage) {
    
    char georeferencedPath[] = "/tmp/autogeoreference-georeferencedXXXXXX.jpg";
    char similarPath[] = "/tmp/autogeoreference-similarXXXXXX.jpg";
    
    mkstemps(georeferencedPath, 4);
    mkstemps(similarPath, 4);
    
    imwrite(georeferencedPath, georeferencedImage);
    imwrite(similarPath, similarImage);
    
    GDALDataset* georeferencedDS = (GDALDataset *) GDALOpen(georeferencedPath, GA_ReadOnly);
    GDALDataset* similarDS = (GDALDataset *) GDALOpen(similarPath, GA_ReadOnly);
    
    remove(georeferencedPath);
    remove(similarPath);

    GDALFeaturePointsCollection* georeferencedPC = new GDALFeaturePointsCollection(georeferencedDS);
    GDALFeaturePointsCollection* similarPC = new GDALFeaturePointsCollection(similarDS);
    GDALMatchedPointsCollection* matched = new GDALMatchedPointsCollection();
    
    int nOctStart = 1;
    int nOctEnd = 4;
    double dfSURFTreshold = 0.001;
    double dfMatchingThreshold = 0.015;
    
    int* panBands = new int[3];
    for (int i = 0; i < 3; i++)
            panBands[i] = i + 1;
    
    // Find feature points on both images
    GatherFeaturePoints(georeferencedDS, panBands,
                    georeferencedPC, nOctStart, nOctEnd, dfSURFTreshold);
    GatherFeaturePoints(similarDS, panBands,
                    similarPC, nOctStart, nOctEnd, dfSURFTreshold);
    
    cerr << "Georeferenced feature points: " << georeferencedPC->GetSize();
    cerr << "Similar feature points: " << similarPC->GetSize();
    
    // Use gathered points to find correspondences
    MatchFeaturePoints(matched, georeferencedPC, similarPC, dfMatchingThreshold);
    
    cerr << "Matched points: " << matched->GetSize();
    
    GDALFeaturePoint *point_1 = new GDALFeaturePoint();
    GDALFeaturePoint *point_2 = new GDALFeaturePoint();

    Mat georeferencedPoints;
    Mat similarPoints;
    
    for (int i = 0; i < matched->GetSize(); i++)
    {
        matched->GetPoints(i, point_1, point_2);

        georeferencedPoints.push_back(Point2f(point_1->GetX(), point_1->GetY()));
        similarPoints.push_back(Point2f(point_2->GetX(), point_2->GetY()));
    }
    
    // clean up
    delete georeferencedDS;
    delete similarDS;
    delete georeferencedPC;
    delete similarPC;
    delete matched;
    delete point_1;
    delete point_2;
    delete[] panBands;
    
    if (georeferencedPoints.rows == 0) {
        throw RequestException("Images are not similar enough.");
    }
    
    Mat output;
    Mat inliers;
    
    estimateAffine2D(georeferencedPoints, similarPoints, output, inliers, 12, 0);
    return output;
}

void AutoGeoreference::applyMatrix(cv::Mat matrix, double georeferencedRatio, double similarRatio, const std::vector<cv::Point2f>& in, std::vector<cv::Point2f>& out) {
    Mat georeferencedResizeMat = Mat::eye(2, 2, CV_32F) * georeferencedRatio;
    Mat similarResizeMat = Mat::eye(2, 2, CV_32F) * (1.0 / similarRatio);
    transform(in, out, georeferencedResizeMat);
    transform(out, out, matrix);
    transform(out, out, similarResizeMat);
}