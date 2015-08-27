/* 
 * File:   ImageDownloaderManager.h
 * Author: dudae
 *
 * Created on Štvrtok, 2015, augusta 13, 12:00
 */

#ifndef IMAGEDOWNLOADERMANAGER_H
#define	IMAGEDOWNLOADERMANAGER_H

#include "ImageDownloader.h"

#include <map>
#include <string>

class ImageDownloaderManager {
public:
    static ImageDownloaderManager& getInstance();
    
    ImageDownloaderManager();
    virtual ~ImageDownloaderManager();
    
    ImageDownloader& getImageDownloader(const Pyramid& pyramid) const;
private:
    std::map<std::string, ImageDownloader*> downloaders;
};

#endif	/* IMAGEDOWNLOADERMANAGER_H */

