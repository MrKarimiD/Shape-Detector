#ifndef COLORDETECTOR_H
#define COLORDETECTOR_H

#include <QObject>
#include "showoutputsetting.h"
#include <Prototype_Messages/GameGround.pb.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "QSemaphore"
#include "Constants.h"
#include "QDebug"

using namespace cv;
using namespace std;

class ColorDetector : public QObject
{
    Q_OBJECT
public:
    explicit ColorDetector(QObject *parent = 0);
    QString findColor(vector<Mat> hsvPlanes);
    Vec2f returnColor(Mat input, Mat src);

    QList<Vec2f> red_samples;
    QList<Vec2f> blue_samples;
    QList<Vec2f> green_samples;
    QList<Vec2f> yellow_samples;
    QList<Vec2f> cyan_samples;
    QList<Vec2f> violet_samples;
    QList<Vec2f> black_samples;

signals:

public slots:

};

#endif // COLORDETECTOR_H
