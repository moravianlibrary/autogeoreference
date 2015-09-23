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
    static cv::Mat extractMidArea(const cv::Mat& image);
    static cv::Mat getTranslationMat(const cv::Mat& image, bool sign);
    static cv::Mat create3x3Mat(const cv::Mat& matrix);
    static cv::Mat create3x2Mat(const cv::Mat& matrix);
    
    static const int extractedImgSize;
};

#endif	/* CONTROLLER_H */

