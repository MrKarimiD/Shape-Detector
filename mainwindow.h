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
#include <QXmlStreamReader>
#include <QFile>
#include "filtersettings.h"

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
    void on_xml_checkBox_stateChanged(int arg1);
    void on_xml_button_clicked();
    void on_cont_checkBox_stateChanged(int arg1);
    void on_geom_checkBox_stateChanged(int arg1);
    void on_bound_checkBox_stateChanged(int arg1);
    void on_rotate_checkBox_stateChanged(int arg1);
    void on_medianBlur_checkBox_stateChanged(int arg1);
    void on_adaptiveThreshold_checkBox_stateChanged(int arg1);
    void on_thresh_checkBox_stateChanged(int arg1);
    void on_dilate_checkBox_stateChanged(int arg1);
    void on_canny_checkBox_stateChanged(int arg1);
    void on_kernelSize_lineEdit_textChanged(const QString &arg1);
    void on_blockSize_slider_sliderMoved(int position);
    void on_C_slider_sliderMoved(int position);
    void on_thresh_slider_sliderMoved(int position);
    void on_firstThresh_slider_sliderMoved(int position);
    void on_secondThresh_slider_sliderMoved(int position);
    void on_dilateSize_lineEdit_textChanged(const QString &arg1);
    void on_apertureSize_lineEdit_textChanged(const QString &arg1);
    void on_blue_slider_sliderMoved(int position);
    void on_red_slider_sliderMoved(int position);
    void on_exposure_slider_sliderMoved(int position);
    void on_brightness_slider_sliderMoved(int position);
    void updateCameraSetting();
    void setCameraSetting();

private:
    Ui::MainWindow *ui;
    ImageProcessing *imageProcessor;
    QTimer *cam_timer;
    VideoCapture cap;
    bool cameraIsOpened;
    filterSettings *filterSetting;

    void enableCameraSetting();
    void disableCameraSetting();
    void enableOpenCamera();
    void disableOpenCamera();
    void enableOpenImage();
    void disableOpenImage();
    void enableXML();
    void disableXML();
    void updateOutputOptions();
    void enableMedianBlur();
    void disableMedianBlur();
    void enableAdaptiveThresholdSetting();
    void disableAdaptiveThresholdSetting();
    void enableThresholdSetting();
    void disableThresholdSetting();
    void enableDilateSetting();
    void disableDilateSetting();
    void enableCannySetting();
    void disableCannySetting();
    void updateFilterSetting();

signals:
    void imageReady(Mat image);
    void cameraSettingChanged();

};

#endif // MAINWINDOW_H
