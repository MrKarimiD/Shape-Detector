#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include "QString"
#include "QFileDialog"
#include "filtersettings.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/cuda.hpp>
#include <Prototype_Messages/GameGround.pb.h>
#include "Constants.h"
#include "shape.h"

using namespace cv;
using namespace std;

#include <QObject>

class ImageProcessing : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessing(QObject *parent = 0);

    Mat undistortImage(Mat input);

    Mat applyFilters(Mat input);

    Mat shapeDetection(Mat input,Mat src,Rect cropedRect);

    QString returnHsv(Mat input);

    Vec2f returnColor(Mat input, Mat src);

    void changeOutputSetting(bool con,bool geom,bool bound,bool rotate,bool boundries);

    void updateFilterSettings(filterSettings *fs);

    Mat returnAdaptiveThreshlodImage();

    Mat returnThreshlodImage();

    Mat returnCannyImage();

    Mat returnCropedImage();

    void addShape(float x,float y,double radius,string type,string color);

    Mat Outputs[5];//0->Crop    1->Adaptive    2->threshold   3->canny     4->final

    outputPacket result;

//    QList<Vec3b> red_samples;
//    QList<Vec3b> blue_samples;
//    QList<Vec3b> green_samples;
//    QList<Vec3b> yellow_samples;
//    QList<Vec3b> cyan_samples;
//    QList<Vec3b> violet_samples;
//    QList<Vec3b> black_samples;

    QList<Vec2f> red_samples;
    QList<Vec2f> blue_samples;
    QList<Vec2f> green_samples;
    QList<Vec2f> yellow_samples;
    QList<Vec2f> cyan_samples;
    QList<Vec2f> violet_samples;
    QList<Vec2f> black_samples;

private:
    vector<Vec3f> finding_circles;
    filterSettings *filterSetting;
    cv::Size imSize;
    //Mat cameraMatrix, distCoeffs;
    bool drawContoursBool,drawGeometricLabels,drawBoundedRect,drawRotatedRect,drawBoundries;

    bool checkAspectRatio(vector<Point> contours_poly);

    bool checkAspectRatioForRotatedRect(RotatedRect input);

    void prepareDataForOutput(std::vector<cv::Point>& contour,QString type,Mat frame,Rect crop);

    double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);

    void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour,Rect cropedRect);

    bool colorIsInRange(Vec3b inputColor,Vec3b sourceColor);

    QString findColor(vector<Mat> hsvPlanes);

signals:

public slots:

};

#endif // IMAGEPROCESSING_H
