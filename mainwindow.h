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
#include "System_Protobuf/SystemSettings.pb.h"
#include <iostream>
#include <fstream>
#include "QRubberBand"
#include "camerasetting.h"

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

    //-----filter Setting Changed-----
    void on_undisort_checkBox_stateChanged();

    void on_crop_checkBox_stateChanged();

    void on_fX_lineEdit_textChanged();

    void on_fY_lineEdit_textChanged();

    void on_sX_lineEdit_textChanged();

    void on_sY_lineEdit_textChanged();

    void on_medianBlur_checkBox_stateChanged();

    void on_kernelSize_lineEdit_textChanged();

    void on_adaptiveThreshold_checkBox_stateChanged();

    void on_thresh_checkBox_stateChanged();

    void on_dilate_checkBox_stateChanged();

    void on_dilateSize_lineEdit_textChanged();

    void on_canny_checkBox_stateChanged();

    void on_blockSize_slider_sliderMoved(int position);

    void on_C_slider_sliderMoved(int position);

    void on_thresh_slider_sliderMoved(int position);

    void on_firstThresh_slider_sliderMoved(int position);

    void on_secondThresh_slider_sliderMoved(int position);

    void on_apertureSize_lineEdit_textChanged();

    //-----camera setting changed----------------
    void on_blue_slider_sliderMoved(int position);

    void on_red_slider_sliderMoved(int position);

    void on_exposure_slider_sliderMoved(int position);

    void on_brightness_slider_sliderMoved(int position);

    void updateCameraSetting();

    void setCameraSetting();

    void on_sharpness_slider_sliderMoved(int position);

    void on_gain_slider_sliderMoved(int position);
    //---------------------------------------------

    void on_mouse_button_clicked();

    //-------Mouse Functions---------
    bool eventFilter(QObject *obj, QEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    //-------------------------------

    void on_drawCrop_checkBox_stateChanged();

    void on_firstM_rButton_toggled(bool checked);

    void on_secondM_rButton_toggled(bool checked);

    void on_go_button_clicked();

    void on_thirsM_rButton_toggled(bool checked);

    void sendDataPacket();

    void on_use_blue_checkBox_toggled(bool checked);

    void on_use_red_checkBox_toggled(bool checked);

    void on_use_green_checkBox_toggled(bool checked);

    void on_use_yellow_checkBox_toggled(bool checked);

    void on_use_violet_checkBox_toggled(bool checked);

    void on_use_cyan_checkBox_toggled(bool checked);

    void on_use_black_checkBox_toggled(bool checked);

    void on_redSelect_button_clicked();

    void on_blueSelect_button_clicked();

    void on_greenSelect_button_clicked();

    void on_yellowSelect_button_clicked();

    void on_violetSelect_button_clicked();

    void on_cyanSelect_button_clicked();

    void on_blackSelect_button_clicked();

    void on_save_set_button_clicked();

    void on_open_set_button_clicked();

    void on_add_red_button_clicked();

    void on_add_blue_button_clicked();

    void on_add_green_button_clicked();

    void on_add_yellow_button_clicked();

    void on_add_violet_button_clicked();

    void on_add_cyan_button_clicked();

    void on_add_black_button_clicked();

    void on_cancel_button_clicked();

    void on_stall_button_clicked();

    void on_clear_red_button_clicked();

    void on_clear_blue_button_clicked();

    void on_clear_green_button_clicked();

    void on_clear_yellow_button_clicked();

    void on_clear_violet_button_clicked();

    void on_clear_cyan_button_clicked();

    void on_clear_black_button_clicked();

    void responseForFilterSettingsChanged();



private:
    Ui::MainWindow *ui;
    ImageProcessing *imageProcessor;
    QTimer *cam_timer,*send_timer;
    VideoCapture cap;
    bool cameraIsOpened,mouseButtonClicked,firstPointSelectedIsValid,imProcDataAvailable;
    bool permissionForSending;
    filterSettings *filterSetting;
    cameraSetting *camSetting;
    QSemaphore *semaphore;
    NetworkSender *sendingSocket;
    int mission;
    bool colorMode;
    bool stallMode;
    QString whichColor;
    QPoint origin;
    QRubberBand *rubberBand;
    Mat frameForColorDetection;
    QSemaphore *semaphoreForColorImage;
    QSemaphore *access2StallMode;

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

    void enableSelectRed();

    void disableSelectRed();

    void enableSelectBlue();

    void disableSelectBlue();

    void enableSelectGreen();

    void disableSelectGreen();

    void enableSelectYellow();

    void disableSelectYellow();

    void enableSelectViolet();

    void disableSelectViolet();

    void enableSelectCyan();

    void disableSelectCyan();

    void enableSelectBlack();

    void disableSelectBlack();

    bool isValidPlaceForSelect(int x,int y);

    void setInitializeMessage(int mission);

    void preapreDataForSending();

    void openSetting(QString fileAddress);

signals:
    void imageReady(Mat image);
    void cameraSettingChanged();
    void dataReadyForSend();
    void filterSettingChanged();

};

#endif // MAINWINDOW_H
