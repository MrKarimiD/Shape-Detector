#include "imageprocfunction.h"

ImageProcFunction::ImageProcFunction(QObject *parent) :
    QObject(parent)
{
    semaphore = new QSemaphore(1);
    timer = new QTimer();
    timer->start(20);
    connect(timer,SIGNAL(timeout()),this,SLOT(timer_interval()));
    newUnreadData = false;
    qRegisterMetaType<MatArray>("MatArray");
}

void ImageProcFunction::setImage(MatArray input)
{
    semaphore->tryAcquire(1,30);
    //input.copyTo(this->input);
    this->input = input;
    newUnreadData = true;
    semaphore->release(1);
}

void ImageProcFunction::process()
{

}

void ImageProcFunction::timer_interval()
{
    if(newUnreadData)
    {
        process();
    }
}
