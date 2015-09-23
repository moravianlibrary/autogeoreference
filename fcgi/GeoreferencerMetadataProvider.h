/* 
 * File:   GeoreferencerMetadataProvider.h
 * Author: dudae
 *
 * Created on Utorok, 2015, augusta 11, 14:05
 */

#ifndef GEOREFERENCERMETADATAPROVIDER_H
#define	GEOREFERENCERMETADATAPROVIDER_H

#include <string>

#include <Poco/Dynamic/Var.h>

#include "MetadataProvider.h"


class GeoreferencerMetadataProvider : public MetadataProvider {
public:
    GeoreferencerMetadataProvider(const std::string& identifier, bool georeferenced);
    virtual ~GeoreferencerMetadataProvider();
    
    virtual const Pyramid& getPyramid() const { return pyramid; }
    virtual const std::vector<cv::Point2f>& getPixelPoints() const { return pixelPoints; }
    virtual const std::vector<cv::Point2f>& getGpsPoints() const { return gpsPoints; }
    virtual const std::vector<cv::Point2i>& getCutLine() const { return cutlinePoints; }
protected:
    void parsePyramid(const Poco::Dynamic::Var& json);
    void parsePoints(const Poco::Dynamic::Var& json, bool georeferenced);
private:
    
    Pyramid pyramid;
    std::vector<cv::Point2f> pixelPoints;
    std::vector<cv::Point2f> gpsPoints;
    std::vector<cv::Point2i> cutlinePoints;
};

#endif	/* GEOREFERENCERMETADATAPROVIDER_H */

