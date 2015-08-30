#include "getimage.h"

getImage::getImage(QObject *parent) :
    ImageProcFunction(parent)
{
    camSetting = new cameraSetting();
    fs = new filterSettings();
    semaphoreForUpdateFilterSetting = new QSemaphore(1);
    //semaphoreForUpdateCameraSetting = new QSemaphore(1);
    cameraIsOpened = false;
    this->newUnreadData = true;
}

void getImage::setInputType(QString input)
{
    this->type = input;
}

void getImage::process()
{
    disconnect(timer,SIGNAL(timeout()),this,SLOT(timer_interval()));

    filterSettings *filterSet = new filterSettings();

    semaphoreForUpdateFilterSetting->tryAcquire(1,30);
    filterSet = fs;
    semaphoreForUpdateFilterSetting->release(1);

    Mat readFromCamera;

    MatArray out;

    if(!cameraIsOpened)
    {
        readFromCamera = openCamera();
    }
    else
    {
        readFromCamera = readAnotherFrame();
    }

    Mat inputFrame;

    if(filterSet->getUseUndisort())
    {
        undistortImage(readFromCamera).copyTo(inputFrame);
    }
    else
    {
        readFromCamera.copyTo(inputFrame);
    }
        out.final = inputFrame;

    Mat frame;

    if(filterSet->getUseCrop())
    {
        Rect goal;
        goal.x = filterSet->getCropFirstX();
        goal.y = filterSet->getCropFirstY();
        goal.width = filterSet->getCropSecondX() - filterSet->getCropFirstX();
        goal.height = filterSet->getCropSecondY() - filterSet->getCropFirstY();

        Mat crop = cropPhoto(inputFrame,goal);
        crop.copyTo(frame);
        //crop.release();
    }
    else
    {
        inputFrame.copyTo(frame);
    }

    out.crop = frame;

    connect(timer,SIGNAL(timeout()),this,SLOT(timer_interval()));

    emit done(out);
}

Mat getImage::undistortImage(Mat input)
{
    Mat cameraMatrix = Mat::eye(3, 3, CV_64F);

    cameraMatrix.at<double>(0,0) = 6.7055726712006776e+02;
    cameraMatrix.at<double>(0,1) = 0;
    cameraMatrix.at<double>(0,2) = 3.8950000000000000e+02;

    cameraMatrix.at<double>(1,0) = 0;
    cameraMatrix.at<double>(1,1) = 6.7055726712006776e+02;
    cameraMatrix.at<double>(1,2) = 2.8950000000000000e+02;

    cameraMatrix.at<double>(2,0) = 0;
    cameraMatrix.at<double>(2,1) = 0;
    cameraMatrix.at<double>(2,2) = 1;

    Mat distCoeffs = Mat::zeros(8, 1, CV_64F);

    distCoeffs.at<double>(0,0) = -3.7087470577837894e-01;
    distCoeffs.at<double>(1,0) = 1.8508542088322785e-01;
    distCoeffs.at<double>(2,0) = 0;
    distCoeffs.at<double>(3,0) = 0;
    distCoeffs.at<double>(4,0) = -3.4799226907590207e-02;

    Mat inputFrame;
    input.copyTo(inputFrame);

    Size imageSize = input.size();
    Mat view, rview, map1, map2;

    initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
                            getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 0, imageSize, 0),
                            imageSize, CV_16SC2, map1, map2);
    view = inputFrame;
    remap(view, rview, map1, map2, INTER_LINEAR);

    return rview;
}

Mat getImage::cropPhoto(Mat input, Rect goal)
{
    Mat cropedRect(input,goal);
    return cropedRect;
}

Mat getImage::openCamera()
{
    Mat frame;

    if(type == "0")
    {
        cameraIsOpened=cap.open(CAP_FIREWIRE+0);
        cap.read(frame);
    }
    else if(type == "1")
    {
        cameraIsOpened=cap.open(CAP_FIREWIRE+1);
        cap.read(frame);

    }
    else if(type == "USB0")
    {
        cameraIsOpened=cap.open(0);
        cap.read(frame);

    }
    else if(type == "USB1")
    {
        cameraIsOpened=cap.open(1);
        cap.read(frame);

    }
    else if(type == "Network")
    {

    }
    return frame;
}

void getImage::setCameraSetting()
{
    if(cameraIsOpened)
    {
        cap.set(CAP_PROP_FPS, camSetting->get_fps());
        cap.set(CAP_PROP_WHITE_BALANCE_BLUE_U,camSetting->get_WHITE_BALANCE_BLUE_U());
        cap.set(CAP_PROP_WHITE_BALANCE_RED_V,camSetting->get_WHITE_BALANCE_RED_V());
        cap.set(CAP_PROP_BRIGHTNESS,camSetting->get_BRIGHTNESS());
        cap.set(CAP_PROP_EXPOSURE,camSetting->get_EXPOSURE());
        cap.set(CAP_PROP_SHARPNESS,camSetting->get_SHARPNESS());
        cap.set(CAP_PROP_GAIN,camSetting->get_GAIN());
        cap.set(CAP_PROP_HUE,camSetting->get_HUE());
        cap.set(CAP_PROP_SATURATION,camSetting->get_SATURATION());
        cap.set(CAP_PROP_CONTRAST,camSetting->get_CONTRAST());
    }
}

Mat getImage::readAnotherFrame()
{
    Mat frame;
    cameraIsOpened = cap.read(frame);
    return frame;
}

void getImage::updateFilterSetting(filterSettings *fs)
{
    semaphoreForUpdateFilterSetting->acquire(1);
    this->fs = fs;
    semaphoreForUpdateFilterSetting->release(1);
}

void getImage::updateCameraSetting(cameraSetting *cs)
{
    //semaphoreForUpdateCameraSetting->acquire(1);
    this->camSetting = cs;
    setCameraSetting();
    //semaphoreForUpdateCameraSetting->release(1);
}

void getImage::setTimerInterval(int fps)
{
    this->timer->stop();
    this->timer->start(1000*(1/fps));
}
