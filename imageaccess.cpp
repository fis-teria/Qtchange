#include "imageaccess.h"

cv::Mat imageAccess::strage = cv::Mat(1,1,CV_8UC3);

void imageAccess::strageImage(cv::Mat img)
{
    strage = img.clone();
}

cv::Mat imageAccess::returnImage(cv::Mat img)
{
    img = strage.clone();
    return img;
}
