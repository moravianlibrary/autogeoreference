/* 
 * File:   AwareImageDownloader.h
 * Author: dudae
 *
 * Created on Utorok, 2015, augusta 11, 15:10
 */

#ifndef AWAREIMAGEDOWNLOADER_H
#define	AWAREIMAGEDOWNLOADER_H

#include "ImageDownloader.h"


class AwareImageDownloader : public ImageDownloader {
public:
    AwareImageDownloader();
    virtual ~AwareImageDownloader();
    
    virtual cv::Mat download(const Pyramid& pyramid) const;
private:

};

#endif	/* AWAREIMAGEDOWNLOADER_H */

