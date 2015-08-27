/* 
 * File:   Controller.cpp
 * Author: dudae
 * 
 * Created on Utorok, 2015, augusta 11, 13:31
 */

#include "Controller.h"
#include "GeoreferencerMetadataProvider.h"
#include "ImageDownloaderManager.h"
#include "AutoGeoreference.h"
#include "RequestException.h"
#include "appio.h"

#include <vector>

using namespace std;
using namespace cv;

Controller::Controller() {
}

Controller::~Controller() {
}

void Controller::autoGeoreference(const std::map<std::string, std::string>& params) {
    if (!params.count("georeferenced")) {
        throw RequestException("Param georeferenced must be provided.");
    }
    if (!params.count("similar")) {
        throw RequestException("Param similar must be provided.");
    }
    string georeferencedMap = params.at("georeferenced");
    string similarMap = params.at("similar");
    bool draw = false;
    
    if (params.count("draw") && params.at("draw") == "true") {
        draw = true;
    }
    
    cerr << georeferencedMap;
    cerr << similarMap;
    
    GeoreferencerMetadataProvider metaProviderGeoreferenced(georeferencedMap, true);
    GeoreferencerMetadataProvider metaProviderSimilar(similarMap, false);
    
    const ImageDownloader& imageDownloaderGeoreferenced = ImageDownloaderManager::getInstance()
                                                            .getImageDownloader(metaProviderGeoreferenced.getPyramid());
    const ImageDownloader& imageDownloaderSimilar = ImageDownloaderManager::getInstance()
                                                            .getImageDownloader(metaProviderSimilar.getPyramid());
    
    Mat georeferencedImage = imageDownloaderGeoreferenced.download(metaProviderGeoreferenced.getPyramid());
    Mat similarImage = imageDownloaderSimilar.download(metaProviderSimilar.getPyramid());
    
    Mat matrix = AutoGeoreference::findAffineMatrix(georeferencedImage, similarImage);
    double resizeRatio = georeferencedImage.cols * 1.0 / metaProviderGeoreferenced.getPyramid().getWidth();
    
    if (draw) {
        vector<Point2f> similarPoints;
        Mat resizeMatrix = Mat::eye(2, 2, CV_32F) * resizeRatio;
        // Resize original pixel points to fit the scaled image
        transform(metaProviderGeoreferenced.getPixelPoints(), similarPoints, resizeMatrix);
        // Draw resized points to the scaled image
        drawPoints(georeferencedImage, similarPoints);
        // Apply matrix to the resized points
        transform(similarPoints, similarPoints, matrix);
        // Draw points to the similar image
        drawPoints(similarImage, similarPoints);
    }
    
    vector<Point2f> outPoints;
    AutoGeoreference::applyMatrix(matrix, resizeRatio, metaProviderGeoreferenced.getPixelPoints(), outPoints);
    
    appio::print_ok(outPoints, metaProviderGeoreferenced.getGpsPoints(), georeferencedImage, similarImage, draw);
}

void Controller::drawPoints(cv::Mat& image, std::vector<cv::Point2f> points) {
    const Scalar redColor(0, 0, 255);
    for (auto point : points) {
        circle(image, point, 10, redColor, -1);
    }
}