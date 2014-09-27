#ifndef MATARRAY_H
#define MATARRAY_H

#include <QObject>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

class MatArray
{
public:
    explicit MatArray();

    Mat crop;
    Mat addaptiveThreshold;
    Mat threshold;
    Mat canny;
    Mat final;

signals:

public slots:

};

#endif // MATARRAY_H
