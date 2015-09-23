/* 
 * File:   MetadataProvider.h
 * Author: dudae
 *
 * Created on Utorok, 2015, augusta 11, 14:02
 */

#ifndef METADATAPROVIDER_H
#define	METADATAPROVIDER_H

#include <vector>

#include "Pyramid.h"

#include <opencv2/opencv.hpp>


class MetadataProvider {
    
public:
    virtual const Pyramid& getPyramid() const = 0;
    virtual const std::vector<cv::Point2f>& getPixelPoints() const = 0;
    virtual const std::vector<cv::Point2f>& getGpsPoints() const = 0;
    virtual const std::vector<cv::Point2i>& getCutLine() const = 0;
};

#endif	/* METADATAPROVIDER_H */

