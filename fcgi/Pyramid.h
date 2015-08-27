/* 
 * File:   Pyramid.h
 * Author: dudae
 *
 * Created on Utorok, 2015, augusta 11, 13:35
 */

#ifndef PYRAMID_H
#define	PYRAMID_H

#include <string>

class Pyramid {
    
public:
    Pyramid() {}
    ~Pyramid() {}
    
    const std::string& getFormat() const { return format; }
    void setFormat(const std::string& format) { this->format = format; }
    
    const std::string& getUrl() const { return url; }
    void setUrl(const std::string& url) { this->url = url; }
    
    int getWidth() const { return width; }
    void setWidth(int width) { this->width = width; }
    
    int getHeight() const { return height; }
    void setHeight(int height) { this->height = height; }
    
    int getTileWidth() const { return tileWidth; }
    void setTileWidth(int tileWidth) { this->tileWidth = tileWidth; }
    
    int getTileHeight() const { return tileHeight; }
    void setTileHeight(int tileHeight) { this->tileHeight = tileHeight; }
    
private:
    std::string format;
    std::string url;
    int width;
    int height;
    int tileWidth;
    int tileHeight;
};

#endif	/* PYRAMID_H */

