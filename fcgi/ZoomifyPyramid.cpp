/* 
 * File:   ZoomifyPyramid.cpp
 * Author: dudae
 * 
 * Created on Streda, 2015, augusta 12, 10:37
 */

#include "ZoomifyPyramid.h"

#include <cmath>
#include <iostream>

using namespace std;

ZoomifyLevel::ZoomifyLevel(int width, int height, int tileWidth, int tileHeight) {
    this->width = width;
    this->height = height;
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    this->xtiles = (int) ceil(width * 1.0 / tileWidth);
    this->ytiles = (int) ceil(height * 1.0 / tileHeight);
}

ZoomifyLevel::~ZoomifyLevel() {
}

ZoomifyPyramid::ZoomifyPyramid(int width, int height, int tileWidth, int tileHeight) {
    this->width = width;
    this->height = height;
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    
    ZoomifyLevel* level = new ZoomifyLevel(width, height, tileWidth, tileHeight);
    while (width > level->getTileWidth() || height > level->getTileHeight()) {
        pyramid.push_back(level);
        level = new ZoomifyLevel(width, height, level->getTileWidth() * 2, level->getTileHeight() * 2);
    }
    delete level;
    pyramid.push_back(new ZoomifyLevel(1, 1, 1, 1));
}

ZoomifyPyramid::~ZoomifyPyramid() {
    for (vector<ZoomifyLevel*>::iterator it = pyramid.begin(); it != pyramid.end(); it++) {
        delete *it;
    }
}

int ZoomifyPyramid::tilesUptoLevel(int level) const {
    int sum = 0;
    int i = 0;
    vector<ZoomifyLevel*>::const_reverse_iterator it = pyramid.rbegin();
    for (; i < level && it != pyramid.rend(); i++, it++) {
        sum += (*it)->getSize();
    }
    return sum;
}

int ZoomifyPyramid::getTileIndex(int level, int x, int y) const {
    return (x + (-y - 1) * pyramid[pyramid.size() - level - 1]->getXTiles() + tilesUptoLevel(level)) / tileWidth;
}