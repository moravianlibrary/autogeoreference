/* 
 * File:   appio.h
 * Author: dudae
 *
 * Created on Nedeľa, 2015, júna 28, 13:50
 */

#ifndef APPIO_H
#define	APPIO_H

#include <string>
#include <vector>
#include <map>
#include <fcgio.h>

#include <opencv2/opencv.hpp>

namespace appio {
    
    void print_ok(const std::string& message);
    
    void print_ok(const std::vector<cv::Point2f>& pixelPoints, const std::vector<cv::Point2f>& gpsPoints, const cv::Mat& georeferencedImg, const cv::Mat& similarImg, bool draw);
    
    void print_error(const std::string& message);
    
    void print_error(const char* message);
    
    std::string get_request_action(FCGX_Request request);
    
    std::map<std::string, std::string> get_request_params(FCGX_Request request);
}

#endif	/* APPIO_H */

