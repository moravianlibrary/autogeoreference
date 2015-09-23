#include "appio.h"

#include <sstream>

#include <Poco/URI.h>
#include <Poco/RegularExpression.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/JSON/Object.h>
#include <Poco/Base64Encoder.h>

#include "RequestException.h"
#include "LogicalException.h"

using namespace std;
using namespace cv;
using Poco::JSON::Array;
using Poco::JSON::Stringifier;
using Poco::JSON::Object;
using Poco::Base64Encoder;

Poco::RegularExpression action_re("\\/v\\d+\\/([^\\/\\?]+)\\/?\\??.*$");

const unsigned long STDIN_MAX = 2147483648;

string get_request_content(const FCGX_Request & request) {
    char * content_length_str = FCGX_GetParam("CONTENT_LENGTH", request.envp);
    unsigned long content_length = STDIN_MAX;

    if (content_length_str) {
        content_length = strtol(content_length_str, &content_length_str, 10);
        if (*content_length_str) {
            cerr << "Can't Parse 'CONTENT_LENGTH='"
                 << FCGX_GetParam("CONTENT_LENGTH", request.envp)
                 << "'. Consuming stdin up to " << STDIN_MAX << endl;
        }

        if (content_length > STDIN_MAX) {
            content_length = STDIN_MAX;
        }
    } else {
        // Do not read from stdin if CONTENT_LENGTH is missing
        content_length = 0;
    }

    char * content_buffer = new char[content_length];
    cin.read(content_buffer, content_length);
    content_length = cin.gcount();

    // Chew up any remaining stdin - this shouldn't be necessary
    // but is because mod_fastcgi doesn't handle it correctly.

    // ignore() doesn't set the eof bit in some versions of glibc++
    // so use gcount() instead of eof()...
    do cin.ignore(1024); while (cin.gcount() == 1024);

    string content(content_buffer, content_length);
    delete [] content_buffer;
    return content;
}

void print_header() {
    cout << "Content-type: application/json\r\n";
    cout << "Access-Control-Allow-Origin: *\r\n";
    cout << "\r\n";
}

string image_to_base64(const Mat& image) {
    vector<uchar> buffer;
    imencode(".jpg", image, buffer);
            
    stringstream ss;
    Base64Encoder b64enc(ss);
    
    ostream_iterator<char> out_it(b64enc);
    copy(buffer.begin(), buffer.end(), out_it);
    
    return ss.str();
}

void appio::print_ok(const string& message) {
    print_header();
    Object root;
    root.set("status", "ok");
    root.set("message", message);
    
    Stringifier::stringify(root, cout);
}

void appio::print_ok(const std::vector<cv::Point2f>& pixelPoints, const std::vector<cv::Point2f>& gpsPoints, const cv::Mat& georeferencedImg, const cv::Mat& similarImg, bool draw) {
    print_header();
    
    Array controlPoints;
    
    if (pixelPoints.size() != gpsPoints.size()) {
        throw LogicalException("pixelPoints.size() != gpsPoints.size()");
    }
    
    for (int i = 0; i < pixelPoints.size(); i++) {
        Object controlPoint;
        controlPoint.set("pixel_x", (int) round(pixelPoints[i].x));
        controlPoint.set("pixel_y", (int) round(pixelPoints[i].y));
        controlPoint.set("longitude", gpsPoints[i].x);
        controlPoint.set("latitude", gpsPoints[i].y);
        controlPoints.add(controlPoint);
    }
    
    Object root;
    root.set("status", "ok");
    root.set("control_points", controlPoints);
    if (draw) {
        root.set("georeferenced_image", image_to_base64(georeferencedImg));
        root.set("similar_image", image_to_base64(similarImg));
    }
    
    Stringifier::stringify(root, cout);
}

void appio::print_ok(const std::vector<cv::Point2f>& pixelPoints, const std::vector<cv::Point2f>& gpsPoints, const cv::Mat& georeferencedImg, const cv::Mat& similarImg, bool draw, const cv::Mat& debugImage) {
    print_header();
    
    Array controlPoints;
    
    if (pixelPoints.size() != gpsPoints.size()) {
        throw LogicalException("pixelPoints.size() != gpsPoints.size()");
    }
    
    for (int i = 0; i < pixelPoints.size(); i++) {
        Object controlPoint;
        controlPoint.set("pixel_x", (int) round(pixelPoints[i].x));
        controlPoint.set("pixel_y", (int) round(pixelPoints[i].y));
        controlPoint.set("longitude", gpsPoints[i].x);
        controlPoint.set("latitude", gpsPoints[i].y);
        controlPoints.add(controlPoint);
    }
    
    Object root;
    root.set("status", "ok");
    root.set("control_points", controlPoints);
    if (draw) {
        root.set("georeferenced_image", image_to_base64(georeferencedImg));
        root.set("similar_image", image_to_base64(similarImg));
    }
    
    root.set("debug_image", image_to_base64(debugImage));
    
    Stringifier::stringify(root, cout);
}

void appio::print_error(const string& message) {
    print_header();
    Object root;
    root.set("status", "error");
    root.set("message", message);
    
    Stringifier::stringify(root, cout);
}

void appio::print_error(const char* message) {
    print_header();
    Object root;
    root.set("status", "error");
    root.set("message", message);
    
    Stringifier::stringify(root, cout);
}

std::string appio::get_request_action(FCGX_Request request) {
    string request_uri = FCGX_GetParam("REQUEST_URI", request.envp);
    Poco::RegularExpression::MatchVec matches;
    action_re.match(request_uri, 0, matches);
    
    if (matches.size() > 1) {
        Poco::RegularExpression::Match match = matches[1];
        return request_uri.substr(match.offset, match.length);
    } else {
        throw RequestException("Invalid uri.");
    }
}

map<string, string> appio::get_request_params(FCGX_Request request) {
    string request_uri = FCGX_GetParam("REQUEST_URI", request.envp);
    map<string, string> result;
    
    Poco::URI uri(request_uri);
    Poco::URI::QueryParameters params = uri.getQueryParameters();
    
    for (Poco::URI::QueryParameters::const_iterator it = params.begin(); it != params.end(); it++) {
        result[it->first] = it->second;
    }
    
    return result;
}