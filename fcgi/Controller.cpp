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

const int Controller::extractedImgSize = 1000;

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
    
    Mat georeferencedImageMid = extractMidArea(georeferencedImage);
    Mat similarImageMid = extractMidArea(similarImage);
    
    Mat matrix = AutoGeoreference::findAffineMatrix(georeferencedImageMid, similarImageMid);
    matrix = create3x3Mat(matrix);
    matrix = getTranslationMat(similarImage, true) * matrix * getTranslationMat(georeferencedImage, false);
    matrix = create3x2Mat(matrix);
    
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