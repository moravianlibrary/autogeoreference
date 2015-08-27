/* 
 * File:   GeoreferencerMetadataProvider.cpp
 * Author: dudae
 * 
 * Created on Utorok, 2015, augusta 11, 14:05
 */

#include "GeoreferencerMetadataProvider.h"

#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/URI.h>

#include "RequestException.h"
#include "GeoreferencerException.h"

using namespace std;

using Poco::Dynamic::Var;
using Poco::JSON::Array;
using Poco::JSON::Object;
using Poco::JSON::Parser;
using Poco::Net::Context;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;

GeoreferencerMetadataProvider::GeoreferencerMetadataProvider(const std::string& identifier, bool georeferenced) {
    Poco::URI uri;
    uri.setScheme("http");
    uri.setHost("staremapy.georeferencer.cz");
    uri.setPath("/map/" + identifier + "/json");
    
    string path = uri.getPathAndQuery();
    if (path.empty()) path = "/";
    
    HTTPClientSession session(uri.getHost(), uri.getPort());
    
    HTTPRequest request(HTTPRequest::HTTP_GET, path);
    HTTPResponse response;
    
    session.sendRequest(request);
    istream& rs = session.receiveResponse(response);
    
    if (response.getStatus() == HTTPResponse::HTTP_OK) {
        
        Parser parser;
        Var json = parser.parse(rs);
        parsePyramid(json);
        parsePoints(json, georeferenced);
        
    } else {
        ostringstream os;
        os << "Unable to retrieve json data from url: " << uri.toString() << ". Returned status code: " << (int) response.getStatus();
        throw RequestException(os.str());
    }
    
}

GeoreferencerMetadataProvider::~GeoreferencerMetadataProvider() {
}

void GeoreferencerMetadataProvider::parsePyramid(const Poco::Dynamic::Var& json) {
    Object::Ptr jsonPtr = json.extract<Object::Ptr>();
    
    if (!jsonPtr->has("pyramid")) {
        throw GeoreferencerException("Missing pyramid attribute in Georeferencer JSON.");
    }
    
    Object::Ptr pyramidJSON = jsonPtr->get("pyramid").extract<Object::Ptr>();
    
    if (!pyramidJSON->has("format")) {
        throw GeoreferencerException("Missing pyramid.format attribute in Georeferencer JSON.");
    }
    if (!pyramidJSON->has("url")) {
        throw GeoreferencerException("Missing pyramid.url attribute in Georeferencer JSON.");
    }
    if (!pyramidJSON->has("width")) {
        throw GeoreferencerException("Missing pyramid.width attribute in Georeferencer JSON.");
    }
    if (!pyramidJSON->has("height")) {
        throw GeoreferencerException("Missing pyramid.height attribute in Georeferencer JSON.");
    }
    if (!pyramidJSON->has("tile_width")) {
        throw GeoreferencerException("Missing pyramid.tile_width attribute in Georeferencer JSON.");
    }
    if (!pyramidJSON->has("tile_height")) {
        throw GeoreferencerException("Missing pyramid.tile_height attribute in Georeferencer JSON.");
    }
    
    pyramid.setFormat(pyramidJSON->getValue<string>("format"));
    pyramid.setUrl(pyramidJSON->getValue<string>("url"));
    pyramid.setWidth(pyramidJSON->getValue<int>("width"));
    pyramid.setHeight(pyramidJSON->getValue<int>("height"));
    pyramid.setTileWidth(pyramidJSON->getValue<int>("tile_width"));
    pyramid.setTileHeight(pyramidJSON->getValue<int>("tile_height"));
}

void GeoreferencerMetadataProvider::parsePoints(const Poco::Dynamic::Var& json, bool georeferenced) {
    Object::Ptr jsonPtr = json.extract<Object::Ptr>();
    
    if (!jsonPtr->has("control_points")) {
        if (georeferenced) {
            throw GeoreferencerException("Missing control_points attribute in Georeferencer JSON.");
        } else {
            return;
        }
    }
    
    if (jsonPtr->get("control_points").type() != typeid(Array::Ptr)) {
        throw GeoreferencerException("Param control_points must be type of Array.");
    }
    
    Array::Ptr controlPoints = jsonPtr->get("control_points").extract<Array::Ptr>();
    
    for (Array::ConstIterator it = controlPoints->begin(); it != controlPoints->end(); it++) {
        Poco::JSON::Object::Ptr item = it->extract<Poco::JSON::Object::Ptr>();
        float x = item->getValue<float>("pixel_x");
        float y = item->getValue<float>("pixel_y");
        float lon = item->getValue<float>("longitude");
        float lat = item->getValue<float>("latitude");
        
        pixelPoints.push_back(cv::Point2f(x, y));
        gpsPoints.push_back(cv::Point2f(lon, lat));
    }
}