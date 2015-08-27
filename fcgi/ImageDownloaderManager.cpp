/* 
 * File:   ImageDownloaderManager.cpp
 * Author: dudae
 * 
 * Created on Štvrtok, 2015, augusta 13, 12:00
 */

#include "ImageDownloaderManager.h"
#include "AwareImageDownloader.h"
#include "ZoomifyImageDownloader.h"
#include "RequestException.h"

#include <sstream>

using namespace std;

ImageDownloaderManager& ImageDownloaderManager::getInstance() {
    static ImageDownloaderManager instance;
    return instance;
}

ImageDownloaderManager::ImageDownloaderManager() {
    downloaders["AWARE"] = new AwareImageDownloader();
    downloaders["ZOOMIFY"] = new ZoomifyImageDownloader();
}

ImageDownloaderManager::~ImageDownloaderManager() {
    for (auto& it: downloaders) {
        delete it.second;
    }
}

ImageDownloader& ImageDownloaderManager::getImageDownloader(const Pyramid& pyramid) const {
    string format = pyramid.getFormat();
    transform(format.begin(), format.end(), format.begin(), ::toupper);
    if (downloaders.count(format)) {
        return *downloaders.at(format);
    } else {
        stringstream ss;
        ss << "Unknown format: " << pyramid.getFormat();
        throw RequestException(ss.str());
    }
}
