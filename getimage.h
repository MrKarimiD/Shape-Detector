#ifndef GETIMAGE_H
#define GETIMAGE_H

#include <QObject>
#include "imageprocfunction.h"
#include "QDebug"

class getImage : public ImageProcFunction
{
    Q_OBJECT
public:
    explicit getImage(QObject *parent = 0);
    void setInputType(QString input);
    void process();
    void updateFilterSetting(filterSettings *fs);
    void updateCameraSetting(cameraSetting *cs);
    void setTimerInterval(int fps);

private:
    Mat undistortImage(Mat input);
    Mat cropPhoto(Mat input,Rect goal);
    Mat openCamera();
    void setCameraSetting();
    Mat readAnotherFrame();

    cameraSetting *camSetting;
    filterSettings *fs;
    QSemaphore *semaphoreForUpdateFilterSetting;
    //QSemaphore *semaphoreForUpdateCameraSetting;

    QString type;
    VideoCapture cap;
    bool cameraIsOpened;

//signals:
//    void done1(Mat out);
};

#endif // GETIMAGE_H
