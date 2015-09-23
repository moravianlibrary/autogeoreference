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

const int Controller::extractedImgSize = 2000;

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
    
    Mat georeferencedDrawImage = georeferencedImage.clone();
    Mat similarDrawImage = similarImage.clone();
    
    Mat georeferencedImageMid = extractMidArea(georeferencedImage);
    Mat similarImageMid = extractMidArea(similarImage);
    
    vector<Point2f> debugGeoreferencedPoints;
    vector<Point2f> debugSimilarPoints;
    
    Mat matrix = AutoGeoreference::findAffineMatrix(georeferencedImageMid, similarImageMid, debugGeoreferencedPoints, debugSimilarPoints);
    matrix = create3x3Mat(matrix);
    matrix = getTranslationMat(similarImage, true) * matrix * getTranslationMat(georeferencedImage, false);
    matrix = create3x2Mat(matrix);
    
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
        Mat debugImage = Mat::zeros(height, width, CV_8UC3);
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
        circle(image, point, 10, redColor, -1);
    }
}

void Controller::drawDebugPoints(
        cv::Mat& debugImage,
        const cv::Mat& georeferencedImage,
        const cv::Mat& similarImage,
        const std::vector<cv::Point2f>& georeferencedPoints,
        const std::vector<cv::Point2f>& similarPoints) {
    
    const Scalar redColor(0, 0, 255);
    int georeferencedTranslateX = (georeferencedImage.cols - extractedImgSize) / 2;
    int georeferencedTranslateY = (georeferencedImage.rows - extractedImgSize) / 2;
    int similarTranslateX = (similarImage.cols - extractedImgSize) / 2;
    int similarTranslateY = (similarImage.rows - extractedImgSize) / 2;
    
    for (int i = 0; i < georeferencedPoints.size(); i++) {
        Point2f georeferencedPoint = georeferencedPoints[i];
        Point2f similarPoint = similarPoints[i];
        georeferencedPoint.x += georeferencedTranslateX;
        georeferencedPoint.y += georeferencedTranslateY;
        similarPoint.x += georeferencedImage.cols + similarTranslateX;
        similarPoint.y += similarTranslateY;
        
        line(debugImage, georeferencedPoint, similarPoint, redColor, 4);
    }
}

Mat Controller::extractMidArea(const cv::Mat& image) {
    int midx = image.cols / 2;
    int midy = image.rows / 2;
    
    return image(Range(midy - extractedImgSize / 2, midy + extractedImgSize / 2),
                 Range(midx - extractedImgSize / 2, midx + extractedImgSize / 2));
}

Mat Controller::getTranslationMat(const cv::Mat& image, bool sign) {
    Mat matrix = Mat::eye(3, 3, CV_64F);
    int translateX = (image.cols - extractedImgSize) / 2;
    int translateY = (image.rows - extractedImgSize) / 2;
    if (sign) {
        matrix.at<double>(0, 2) = translateX;
        matrix.at<double>(1, 2) = translateY;
    } else {
        matrix.at<double>(0, 2) = -translateX;
        matrix.at<double>(1, 2) = -translateY;
    }
    return matrix;
}

Mat Controller::create3x3Mat(const cv::Mat& matrix) {
    Mat out = Mat::eye(3, 3, CV_64F);
    matrix.copyTo(out(Range(0, 2), Range(0, 3)));
    return out;
}

Mat Controller::create3x2Mat(const cv::Mat& matrix) {
    Mat out;
    out.push_back(matrix.row(0));
    out.push_back(matrix.row(1));
    return out;
}