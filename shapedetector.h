#ifndef SHAPEDETECTOR_H
#define SHAPEDETECTOR_H

#include <QObject>
#include "imageprocfunction.h"
#include "showoutputsetting.h"
#include <Prototype_Messages/GameGround.pb.h>
#include "colordetector.h"

class shapeDetector : public ImageProcFunction
{
    Q_OBJECT
public:
    explicit shapeDetector(QObject *parent = 0);
    void process();
    void updateShowSetting(showOutputSetting *os);
    void updateFilterSetting(filterSettings *fs);

    outputPacket result;
    ColorDetector *colorDetector;

private:
    Mat shapeDetection(Mat filter,Mat destination,Rect cropedRect);
    bool checkAspectRatio(vector<Point> contours_poly);
    bool checkAspectRatioForRotatedRect(RotatedRect input);
    double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
    void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour,Rect cropedRect);
    void prepareDataForOutput(std::vector<cv::Point>& contour,QString type,Mat frame,Rect crop);
    void addShape(float x,float y,double radius,string type,string color);


    cv::Size imSize;
    showOutputSetting *os;
    filterSettings *fs;

    QSemaphore *semaphoreForUpdateShowSettings;
    QSemaphore *semaphoreForUpdateFilterSetting;
    //QSemaphore *semaphoreForAddingSource;

    Mat Source;
signals:

public slots:

};

#endif // SHAPEDETECTOR_H
