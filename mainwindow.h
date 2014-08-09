#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include "QString"
#include "QFileDialog"
#include "imageprocessing.h"
#include "QTimer"

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_image_rButton_toggled(bool checked);
    void on_address_button_clicked();
    void on_camera_rButton_toggled(bool checked);
    void on_open_button_clicked();
    void cam_timeout();
    void on_camSet_checkBox_stateChanged(int arg1);
    void callImageProcessingFunctions(Mat input_mat);

private:
    Ui::MainWindow *ui;
    ImageProcessing *imageProcessor;
    QTimer *cam_timer;
    VideoCapture cap;

    void enableCameraSetting();
    void disableCameraSetting();
    void enableOpenCamera();
    void disableOpenCamera();
    void enableOpenImage();
    void disableOpenImage();

signals:
    void imageReady(Mat image);

};

#endif // MAINWINDOW_H
