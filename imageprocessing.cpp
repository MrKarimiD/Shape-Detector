#include "imageprocessing.h"

ImageProcessing::ImageProcessing(QObject *parent) :
    QObject(parent)
{
}

Mat ImageProcessing::shapeDetection(Mat input)
{
    vector<Vec3f> circles;
    Mat outputFrame=input.clone();
    cvtColor(input,input,COLOR_RGB2GRAY);
    HoughCircles(input,circles,HOUGH_GRADIENT,2,10,150,100,0,100);
    for(int i=0;i<circles.size();i++)
    {
        Point center(cvRound(circles[i][0]),cvRound(circles[i][1]));
        double radios=circles[i][2];
        circle(outputFrame,center,3,Scalar(0,0,255),-1);
        circle(outputFrame,center,radios,Scalar(0,0,255),3,8,0);
    }
    return outputFrame;
}

void ImageProcessing::circleDetection()
{

}
