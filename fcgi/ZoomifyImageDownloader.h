/* 
 * File:   ZoomifyImageDownloader.h
 * Author: dudae
 *
 * Created on Streda, 2015, augusta 12, 11:58
 */

#ifndef ZOOMIFYIMAGEDOWNLOADER_H
#define	ZOOMIFYIMAGEDOWNLOADER_H

#include "ImageDownloader.h"

#include <string>

class ZoomifyImageDownloader : public ImageDownloader {
public:
    ZoomifyImageDownloader();
    virtual ~ZoomifyImageDownloader();
    
    virtual cv::Mat download(const Pyramid& pyramid) const;
private:
    virtual cv::Mat downloadTile(const std::string& url, int tileIndex, int level, int x, int y) const;
};

#endif	/* ZOOMIFYIMAGEDOWNLOADER_H */

