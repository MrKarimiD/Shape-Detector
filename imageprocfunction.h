#ifndef IMAGEPROCFUNCTION_H
#define IMAGEPROCFUNCTION_H

#include <QObject>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "QSemaphore"
#include "camerasetting.h"
#include "filtersettings.h"
#include "Constants.h"
#include "QDebug"
#include "QTimer"
#include "matarray.h"

using namespace cv;
using namespace std;

//typedef Mat MatArray;

class ImageProcFunction : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcFunction(QObject *parent = 0);
    void setImage(MatArray input);

signals:
    void done(MatArray out);

public slots:
    virtual void process();

protected slots:
    void timer_interval();

protected:
    MatArray input;
    QSemaphore *semaphore;
    QTimer *timer;
    bool newUnreadData;
};

#endif // IMAGEPROCFUNCTION_H
