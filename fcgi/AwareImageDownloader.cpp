/* 
 * File:   AwareImageDownloader.cpp
 * Author: dudae
 * 
 * Created on Utorok, 2015, augusta 11, 15:10
 */

#include "AwareImageDownloader.h"

#include "RequestException.h"

#include <Poco/URI.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>

#include <sstream>
#include <memory>

using namespace std;
using namespace cv;

using Poco::URI;
using Poco::Net::Context;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPSClientSession;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;

AwareImageDownloader::AwareImageDownloader() {
}

AwareImageDownloader::~AwareImageDownloader() {
}

cv::Mat AwareImageDownloader::download(const Pyramid& pyramid) const {
    URI uri(pyramid.getUrl());
    uri.addQueryParameter("viewwidth", std::to_string(pyramid.getWidth()));
    uri.addQueryParameter("viewheight", std::to_string(pyramid.getHeight()));
    uri.addQueryParameter("x", "0");
    uri.addQueryParameter("y", "0");
    
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