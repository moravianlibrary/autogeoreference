/* 
 * File:   Controller.h
 * Author: dudae
 *
 * Created on Utorok, 2015, augusta 11, 13:31
 */

#ifndef CONTROLLER_H
#define	CONTROLLER_H

#include <string>
#include <map>
#include <vector>

#include <opencv2/opencv.hpp>

#include "MetadataProvider.h"

class Controller {
public:
    Controller();
    virtual ~Controller();
    
    static void autoGeoreference(const std::map<std::string, std::string>& params);
private:
    static void drawPoints(cv::Mat& image, const std::vector<cv::Point2f>& points);
    static void drawDebugPoints(
        cv::Mat& debugImage,
        const cv::Mat& georeferencedImage,
        const cv::Mat& similarImage,
        const std::vector<cv::Point2f>& georeferencedPoints,
        const std::vector<cv::Point2f>& similarPoints);
    static void applyCutline(cv::Mat& image, const MetadataProvider& metadata);
    static void resize(cv::Mat& image);
    
    static int longerSide;
};

#endif	/* CONTROLLER_H */

