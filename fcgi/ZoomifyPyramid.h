/* 
 * File:   ZoomifyPyramid.h
 * Author: dudae
 *
 * Created on Streda, 2015, augusta 12, 10:37
 */

#ifndef ZOOMIFYPYRAMID_H
#define	ZOOMIFYPYRAMID_H

#include <vector>

class ZoomifyLevel {
public:
    ZoomifyLevel(int width, int height, int tileWidth, int tileHeight);
    virtual ~ZoomifyLevel();
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getTileWidth() const { return tileWidth; }
    int getTileHeight() const { return tileHeight; }
    int getXTiles() const { return xtiles; }
    int getYTiles() const { return ytiles; }
    int getSize() const { return xtiles * ytiles; }
    
private:
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    int xtiles;
    int ytiles;
};

class ZoomifyPyramid {
public:
    ZoomifyPyramid(int width, int height, int tileWidth, int tileHeight);
    virtual ~ZoomifyPyramid();
    
    int getTileIndex(int level, int x, int y) const;
    int getNTiers() const { return pyramid.size(); }
    const ZoomifyLevel& getZoomifyLevel(int level) const { return *pyramid[pyramid.size() - level - 1]; }
    
private:
    int tilesUptoLevel(int level) const;
    
    int width;
    int height;
    int tileWidth;
    int tileHeight;
    std::vector<ZoomifyLevel*> pyramid;
};

#endif	/* ZOOMIFYPYRAMID_H */

