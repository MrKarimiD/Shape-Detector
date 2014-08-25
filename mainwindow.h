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
#include "QMouseEvent"
#include "QString"
#include "QSemaphore"
#include "QMessageBox"
#include "networksender.h"
#include "Prototype_Messages/GameGround.pb.h"


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

    void on_camSet_checkBox_stateChanged();

    void callImageProcessingFunctions(Mat input_mat);

    void on_xml_checkBox_stateChanged();

    void on_xml_button_clicked();

    void on_cont_checkBox_stateChanged();

    void on_geom_checkBox_stateChanged();

    void on_bound_checkBox_stateChanged();

    void on_rotate_checkBox_stateChanged();

    void on_medianBlur_checkBox_stateChanged();

    void on_adaptiveThreshold_checkBox_stateChanged();

    void on_thresh_checkBox_stateChanged();

    void on_dilate_checkBox_stateChanged();

    void on_canny_checkBox_stateChanged();

    void on_blockSize_slider_sliderMoved(int position);

    void on_C_slider_sliderMoved(int position);

    void on_thresh_slider_sliderMoved(int position);

    void on_firstThresh_slider_sliderMoved(int position);

    void on_secondThresh_slider_sliderMoved(int position);

    void on_blue_slider_sliderMoved(int position);

    void on_red_slider_sliderMoved(int position);

    void on_exposure_slider_sliderMoved(int position);

    void on_brightness_slider_sliderMoved(int position);

    void updateCameraSetting();

    void setCameraSetting();

    void on_sharpness_slider_sliderMoved(int position);

    void on_gain_slider_sliderMoved(int position);

    void on_mouse_button_clicked();

    void mouseDoubleClickEvent(QMouseEvent *event);

    void on_drawCrop_checkBox_stateChanged();

    void on_firstM_rButton_toggled(bool checked);

    void on_secondM_rButton_toggled(bool checked);

    void on_go_button_clicked();

    void on_thirsM_rButton_toggled(bool checked);

    void sendDataPacket();

private:
    Ui::MainWindow *ui;
    ImageProcessing *imageProcessor;
    QTimer *cam_timer,*send_timer;
    VideoCapture cap;
    bool cameraIsOpened,mouseButtonClicked,firstPointSelected,imProcDataAvailable;
    bool permissionForSending;
    filterSettings *filterSetting;
    QSemaphore *semaphore;
    NetworkSender *sendingSocket;
    int mission;

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

    void enableCropSetting();

    void disableCropSetting();

    void enableFirstMission();

    void disableFirstMission();

    void enableSecondMission();

    void disableSecondMission();

    void setInitializeMessage(int mission);

    void preapreDataForSending();

signals:
    void imageReady(Mat image);
    void cameraSettingChanged();

};

#endif // MAINWINDOW_H
