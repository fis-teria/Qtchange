#ifndef IMAGEACCESS_H
#define IMAGEACCESS_H

#include <opencv2/opencv.hpp>

class imageAccess
{
private:
    static cv::Mat strage;
public:
    static void strageImage(cv::Mat img);
    static cv::Mat returnImage(cv::Mat img);

};

#endif // IMAGEACCESS_H
