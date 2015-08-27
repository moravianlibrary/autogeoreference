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
    static void drawPoints(cv::Mat& image, std::vector<cv::Point2f> points);
};

#endif	/* CONTROLLER_H */

