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

int Controller::longerSide = 1000;

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
    bool debug = false;
    
    if (params.count("draw") && params.at("draw") == "true") {
        draw = true;
    }
    
    if (params.count("debug") && params.at("debug") == "true") {
        debug = true;
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
    
    resize(georeferencedImage);
    resize(similarImage);
    
    Mat georeferencedDrawImage = georeferencedImage.clone();
    Mat similarDrawImage = similarImage.clone();
    
    applyCutline(georeferencedImage, metaProviderGeoreferenced);
    applyCutline(similarImage, metaProviderSimilar);
    
    vector<Point2f> debugGeoreferencedPoints;
    vector<Point2f> debugSimilarPoints;
    
    Mat matrix = AutoGeoreference::findAffineMatrix(georeferencedImage, similarImage, debugGeoreferencedPoints, debugSimilarPoints);
    
    double resizeGeoreferencedRatio = georeferencedImage.cols * 1.0 / metaProviderGeoreferenced.getPyramid().getWidth();
    double resizeSimilarRatio = similarImage.cols * 1.0 / metaProviderSimilar.getPyramid().getWidth();
    
    if (draw) {
        vector<Point2f> similarPoints;
        Mat resizeMatrix = Mat::eye(2, 2, CV_32F) * resizeGeoreferencedRatio;
        // Resize original pixel points to fit the scaled image
        transform(metaProviderGeoreferenced.getPixelPoints(), similarPoints, resizeMatrix);
        // Draw resized points to the scaled image
        drawPoints(georeferencedDrawImage, similarPoints);
        // Apply matrix to the resized points
        transform(similarPoints, similarPoints, matrix);
        // Draw points to the similar image
        drawPoints(similarDrawImage, similarPoints);
    }
    
    vector<Point2f> outPoints;
    AutoGeoreference::applyMatrix(matrix, resizeGeoreferencedRatio, resizeSimilarRatio, metaProviderGeoreferenced.getPixelPoints(), outPoints);
    
    if (debug) {
        int width = georeferencedImage.cols + similarImage.cols;
        int height = max(georeferencedImage.rows, similarImage.rows);
        Mat debugImage = Mat::ones(height, width, CV_8UC3);
        georeferencedImage.copyTo(debugImage(
                Range(0, georeferencedImage.rows),
                Range(0, georeferencedImage.cols)));
        similarImage.copyTo(debugImage(
                Range(0, similarImage.rows),
                Range(georeferencedImage.cols, georeferencedImage.cols + similarImage.cols)));
        drawDebugPoints(debugImage, georeferencedImage, similarImage, debugGeoreferencedPoints, debugSimilarPoints);
        appio::print_ok(outPoints, metaProviderGeoreferenced.getGpsPoints(), georeferencedDrawImage, similarDrawImage, draw, debugImage);
    } else {
        appio::print_ok(outPoints, metaProviderGeoreferenced.getGpsPoints(), georeferencedDrawImage, similarDrawImage, draw);
    }
}

void Controller::drawPoints(cv::Mat& image, const std::vector<cv::Point2f>& points) {
    const Scalar redColor(0, 0, 255);
    for (auto point : points) {
        circle(image, point, 3, redColor, -1);
    }
}

void Controller::drawDebugPoints(
        cv::Mat& debugImage,
        const cv::Mat& georeferencedImage,
        const cv::Mat& similarImage,
        const std::vector<cv::Point2f>& georeferencedPoints,
        const std::vector<cv::Point2f>& similarPoints) {
    
    const Scalar redColor(0, 0, 255);
    
    for (int i = 0; i < georeferencedPoints.size(); i++) {
        Point2f georeferencedPoint = georeferencedPoints[i];
        Point2f similarPoint = similarPoints[i];
        similarPoint.x += georeferencedImage.cols;
        
        line(debugImage, georeferencedPoint, similarPoint, redColor, 1);
    }
}

void Controller::applyCutline(cv::Mat& image, const MetadataProvider& metadata) {
    if (!metadata.getCutLine().size()) {
        return;
    }
    Mat mask = Mat::zeros(image.rows, image.cols, CV_8UC1);
    Mat out = Mat::ones(image.rows, image.cols, CV_8UC3);
    Mat scale = Mat::eye(2, 2, CV_32F) * (1.0 * image.cols / metadata.getPyramid().getWidth());
    vector<vector<Point2i> > fillContAll;
    vector<Point2i> cutline = metadata.getCutLine();
    transform(cutline, cutline, scale);
    fillContAll.push_back(cutline);
    fillPoly(mask, fillContAll, Scalar(1));
    
    image.copyTo(out, mask);
    image = out;
}

void Controller::resize(cv::Mat& image) {
    int side = max(image.rows, image.cols);
    double ratio = 1.0 * longerSide / side;
    cv::resize(image, image, Size(image.cols * ratio, image.rows * ratio));
}