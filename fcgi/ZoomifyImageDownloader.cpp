/* 
 * File:   ZoomifyImageDownloader.cpp
 * Author: dudae
 * 
 * Created on Streda, 2015, augusta 12, 11:58
 */

#include "ZoomifyImageDownloader.h"
#include "ZoomifyPyramid.h"
#include "RequestException.h"

#include <Poco/URI.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

#include <opencv2/opencv.hpp>

#include <sstream>
#include <memory>
#include <cmath>

using namespace std;
using namespace cv;

using Poco::URI;
using Poco::Net::Context;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;

ZoomifyImageDownloader::ZoomifyImageDownloader() {
}

ZoomifyImageDownloader::~ZoomifyImageDownloader() {
}

cv::Mat ZoomifyImageDownloader::download(const Pyramid& pyramid) const {
    ZoomifyPyramid zoomifyPyramid(pyramid.getWidth(), pyramid.getHeight(), pyramid.getTileWidth(), pyramid.getTileHeight());
    int level = min(4, zoomifyPyramid.getNTiers() - 1);
    int xCount = zoomifyPyramid.getZoomifyLevel(level).getXTiles();
    int yCount = zoomifyPyramid.getZoomifyLevel(level).getYTiles();
    
    double div = pow(2, zoomifyPyramid.getNTiers() - 1 - level);
    int width = (int) ceil(pyramid.getWidth() * 1.0 / div);
    int height = (int) ceil(pyramid.getHeight() * 1.0 / div);
    
    Mat resultImg = Mat::zeros(height, width, CV_8UC3);
    
    for (int x = 0; x < xCount; x++) {
        for (int y = 0; y < yCount; y++) {
            int cy = -1 - y;
            int tileIndex = zoomifyPyramid.getTileIndex(level, x, cy);
            Mat img = downloadTile(pyramid.getUrl(), tileIndex, level, x, cy);
            img.copyTo(resultImg(
                Range(y * pyramid.getTileHeight(), y * pyramid.getTileHeight() + img.rows),
                Range(x * pyramid.getTileWidth(), x * pyramid.getTileWidth() + img.cols)));
        }
    }
    return resultImg;
}

cv::Mat ZoomifyImageDownloader::downloadTile(const std::string& url, int tileIndex, int level, int x, int y) const {
    stringstream urlBuilder;
    urlBuilder << url << "/TileGroup" << tileIndex << "/" << level << "-" << x << "-" << -y - 1 << ".jpg";
    URI uri(urlBuilder.str());
    
    string path = uri.getPathAndQuery();
    
    unique_ptr<HTTPClientSession> session;
    
    if (uri.getScheme() == "http") {
        session = unique_ptr<HTTPClientSession>(new HTTPClientSession(uri.getHost(), uri.getPort()));
    } else if (uri.getScheme() == "https") {
        const Context::Ptr context = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_STRICT, 9, true);
        session = unique_ptr<HTTPSClientSession>(new HTTPSClientSession(uri.getHost(), uri.getPort(), context));
    } else {
        ostringstream os;
        os << "Unable to get an image: invalid protocol: " << uri.getScheme();
        throw RequestException(os.str());
    }
    
    HTTPRequest request(HTTPRequest::HTTP_GET, path);
    HTTPResponse response;
    
    session->sendRequest(request);
    istream& rs = session->receiveResponse(response);
    
    if (response.getStatus() == HTTPResponse::HTTP_OK) {
        vector<uint8_t> buffer;
        buffer.insert(buffer.begin(), istreambuf_iterator<char>(rs), istreambuf_iterator<char>());
            
        Mat img = imdecode(buffer, CV_LOAD_IMAGE_COLOR);

        return img;
    } else {
        ostringstream os;
        os << "Unable to retrieve image from url: " << uri.toString() << ". Returned status code: " << (int) response.getStatus();
        throw RequestException(os.str());
    }
}