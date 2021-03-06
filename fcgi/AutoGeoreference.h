/* 
 * File:   AutoGeoreference.h
 * Author: dudae
 *
 * Created on Štvrtok, 2015, augusta 13, 12:17
 */

#ifndef AUTOGEOREFERENCE_H
#define	AUTOGEOREFERENCE_H

#include <opencv2/opencv.hpp>

#include <vector>

class AutoGeoreference {
public:
    static cv::Mat findAffineMatrix(const cv::Mat& georeferencedImage, const cv::Mat& similarImage, std::vector<cv::Point2f>& georeferencedPointsOut, std::vector<cv::Point2f>& similarPointsOut);
    static void applyMatrix(cv::Mat matrix, double georeferencedRatio, double similarRatio, const std::vector<cv::Point2f>& in, std::vector<cv::Point2f>& out);
};

#endif	/* AUTOGEOREFERENCE_H */

