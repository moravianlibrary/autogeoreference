/* 
 * File:   ImageDownloader.h
 * Author: dudae
 *
 * Created on Utorok, 2015, augusta 11, 15:09
 */

#ifndef IMAGEDOWNLOADER_H
#define	IMAGEDOWNLOADER_H

#include "Pyramid.h"

#include <opencv2/opencv.hpp>

class ImageDownloader {
    
public:
    virtual cv::Mat download(const Pyramid& pyramid) const = 0;
    
};

#endif	/* IMAGEDOWNLOADER_H */

