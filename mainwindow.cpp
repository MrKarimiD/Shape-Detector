#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qRegisterMetaType<MatArray>("MatArray");

    installEventFilter(this);

    shapeWorker = new shapeDetector();
    getWorker = new getImage();
    filterWorker = new filterProcces();

    getThread = new QThread();
    filterThread = new QThread();
    shapeThread = new QThread();

    getWorker->moveToThread(getThread);
    filterWorker->moveToThread(filterThread);
    shapeWorker->moveToThread(shapeThread);

    cameraIsOpened=false;
    mouseButtonClicked=false;
    colorMode = false;
    firstPointSelectedIsValid=false;

    stallMode = false;

    permissionForSending = false;

    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);

    sendingSocket = new NetworkSender();
    semaphoreForColorImage = new QSemaphore(1);
    access2StallMode = new QSemaphore(1);

    QStringList items;
    items<<"0"<<"1"<<"Network";
    ui->cam_comboBox->addItems(items);

    QStringList fps_items;
    fps_items<<"60"<<"15"<<"30";
    ui->fps_comboBox->addItems(fps_items);

    QPalette red_pal(Qt::red);
    ui->redSelect_button->setAutoFillBackground(true);
    ui->redSelect_button->setPalette(red_pal);

    QPalette blue_pal(Qt::blue);
    ui->blueSelect_button->setAutoFillBackground(true);
    ui->blueSelect_button->setPalette(blue_pal);

    QPalette green_pal(Qt::green);
    ui->greenSelect_button->setAutoFillBackground(true);
    ui->greenSelect_button->setPalette(green_pal);

    QPalette yellow_pal(Qt::yellow);
    ui->yellowSelect_button->setAutoFillBackground(true);
    ui->yellowSelect_button->setPalette(yellow_pal);

    QPalette purple_pal(QColor(148,0,211));
    ui->violetSelect_button->setAutoFillBackground(true);
    ui->violetSelect_button->setPalette(purple_pal);

    QPalette cyan_pal(Qt::cyan);
    ui->cyanSelect_button->setAutoFillBackground(true);
    ui->cyanSelect_button->setPalette(cyan_pal);

    QPalette black_pal(Qt::black);
    ui->blackSelect_button->setAutoFillBackground(true);
    ui->blackSelect_button->setPalette(black_pal);

    QStringList output_items;
    output_items<<"Croped"<<"Adaptive Threshold"<<"Threshold"<<"Canny"<<"Final";
    ui->out_comboBox->addItems(output_items);
    ui->out_comboBox->setCurrentIndex(4);

    filterSetting=new filterSettings();
    camSetting = new cameraSetting();

    connect(this,SIGNAL(cameraSettingChanged()),this,SLOT(updateCameraSetting()));
    connect(this,SIGNAL(dataReadyForSend()),this,SLOT(sendDataPacket()));
    connect(this,SIGNAL(filterSettingChanged()),this,SLOT(responseForFilterSettingsChanged()));
    connect(this,SIGNAL(showSettingChanged()),this,SLOT(updateOutputOptions()));

    connect(getWorker,SIGNAL(done(MatArray)),this,SLOT(getOriginalFrame(MatArray)));
    connect(filterWorker,SIGNAL(done(MatArray)),this,SLOT(getFilterdFrame(MatArray)));
    connect(shapeWorker,SIGNAL(done(MatArray)),this,SLOT(getOutputFrame(MatArray)));

    semaphoreForGetWorker = new QSemaphore(1);
    semaphoreForFilterWorker = new QSemaphore(1);
    semaphoreForShapeWorker = new QSemaphore(1);

    showSetting = new showOutputSetting();

    openSetting("/home/kn2c/setting.txt");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_button_clicked()
{
    sendingSocket->configure(ui->ip_lineEdit->text(),ui->port_lineEdit->text().toInt());

//    Mat frame;

//    if(ui->cam_comboBox->currentText() == "Network")
//    {
//    }
//    else
//    {
//        if(ui->cam_comboBox->currentText()=="0")
//        {
//            cameraIsOpened=cap.open(CAP_FIREWIRE+0);
//        }
//        else
//        {
//            cameraIsOpened=cap.open(CAP_FIREWIRE+1);
//        }

//        setCameraSetting();

//        cap.read(frame);

//        cam_timer->start(5);
//        connect(cam_timer,SIGNAL(timeout()),this,SLOT(cam_timeout()));
//    }

    getWorker->setInputType(ui->cam_comboBox->currentText());
    getWorker->setTimerInterval(ui->fps_comboBox->currentText().toInt());
    getWorker->updateCameraSetting(camSetting);
    getWorker->updateFilterSetting(filterSetting);

    filterWorker->updateFilterSetting(filterSetting);

    shapeWorker->updateFilterSetting(filterSetting);
    getThread->start();
    filterThread->start();
    shapeThread->start();
}

void MainWindow::enableCameraSetting()
{
    ui->red_slider->setEnabled(true);
    ui->blue_slider->setEnabled(true);
    ui->exposure_slider->setEnabled(true);
    ui->brightness_slider->setEnabled(true);
    ui->gain_slider->setEnabled(true);
    ui->sharpness_slider->setEnabled(true);

    ui->red_label->setEnabled(true);
    ui->blue_label->setEnabled(true);
    ui->expo_label->setEnabled(true);
    ui->brightness_label->setEnabled(true);
    ui->gain_label->setEnabled(true);
    ui->sharpness_label->setEnabled(true);

    ui->redOut_label->setEnabled(true);
    ui->blueOut_label->setEnabled(true);
    ui->expoOut_label->setEnabled(true);
    ui->brightnessOut_label->setEnabled(true);
    ui->gainOut_label->setEnabled(true);
    ui->sharpnessOut_label->setEnabled(true);
}

void MainWindow::disableCameraSetting()
{
    ui->red_slider->setDisabled(true);
    ui->blue_slider->setDisabled(true);
    ui->exposure_slider->setDisabled(true);
    ui->brightness_slider->setDisabled(true);
    ui->gain_slider->setDisabled(true);
    ui->sharpness_slider->setDisabled(true);

    ui->red_label->setDisabled(true);
    ui->blue_label->setDisabled(true);
    ui->expo_label->setDisabled(true);
    ui->brightness_label->setDisabled(true);
    ui->gain_label->setDisabled(true);
    ui->sharpness_label->setDisabled(true);

    ui->redOut_label->setDisabled(true);
    ui->blueOut_label->setDisabled(true);
    ui->expoOut_label->setDisabled(true);
    ui->brightnessOut_label->setDisabled(true);
    ui->gainOut_label->setDisabled(true);
    ui->sharpnessOut_label->setDisabled(true);
}

void MainWindow::enableXML()
{
    ui->xmlAdd_lineEdit->setEnabled(true);
    ui->xml_button->setEnabled(true);
}

void MainWindow::disableXML()
{
    ui->xmlAdd_lineEdit->setDisabled(true);
    ui->xml_button->setDisabled(true);
}

void MainWindow::updateOutputOptions()
{
    showSetting->set_drawContoursBool(ui->cont_checkBox->isChecked());
    showSetting->set_drawGeometricLabels(ui->geom_checkBox->isChecked());
    showSetting->set_drawBoundedRect(ui->bound_checkBox->isChecked());
    showSetting->set_drawRotatedRect(ui->rotate_checkBox->isChecked());
    showSetting->set_drawBoundries(ui->drawCrop_checkBox->isChecked());

    shapeWorker->updateShowSetting(showSetting);
}

void MainWindow::getOriginalFrame(MatArray input)
{
    semaphoreForColorImage->acquire(1);
    frameForColorDetection = input.final;
    semaphoreForColorImage->release(1);
    filterWorker->setImage(input);
}

void MainWindow::getFilterdFrame(MatArray input)
{
    shapeWorker->setImage(input);
}

void MainWindow::getOutputFrame(MatArray input)
{
    Mat outputFrame;

    if(ui->out_comboBox->currentText() == "Croped")
    {
        input.crop.copyTo(outputFrame);
    }
    else if(ui->out_comboBox->currentText() == "Adaptive Threshold")
    {
        input.addaptiveThreshold.copyTo(outputFrame);
    }
    else if(ui->out_comboBox->currentText() == "Threshold")
    {
        input.threshold.copyTo(outputFrame);
    }
    else if(ui->out_comboBox->currentText() == "Canny")
    {
        input.canny.copyTo(outputFrame);
    }
    else if(ui->out_comboBox->currentText() == "Final")
    {
        input.final.copyTo(outputFrame);
    }

    if(!outputFrame.empty())
    {
        cv::resize(outputFrame,outputFrame,Size(640,480),0,0,INTER_CUBIC);
        QImage imgIn;
        if(ui->out_comboBox->currentText() == "Final" || ui->out_comboBox->currentText() == "Croped")
        {
            cvtColor(outputFrame, outputFrame, COLOR_BGR2RGB);
        }
        else
        {
            cvtColor(outputFrame, outputFrame, COLOR_GRAY2RGB);
        }

        imgIn= QImage((uchar*) outputFrame.data, outputFrame.cols, outputFrame.rows, outputFrame.step, QImage::Format_RGB888);
        ui->outputLabel->setPixmap(QPixmap::fromImage(imgIn));
    }

    if(permissionForSending)
    {
        shapeWorker->result.set_mission(mission);
        shapeWorker->result.set_type(1);
        //sendDataPacket();
        emit dataReadyForSend();
    }
}

void MainWindow::enableMedianBlur()
{
    ui->kernekSize_label->setEnabled(true);
    ui->kernelSize_lineEdit->setEnabled(true);
}

void MainWindow::disableMedianBlur()
{
    ui->kernekSize_label->setDisabled(true);
    ui->kernelSize_lineEdit->setDisabled(true);
}

void MainWindow::enableAdaptiveThresholdSetting()
{
    ui->blockSizeOut_label->setEnabled(true);
    ui->blockSize_label->setEnabled(true);
    ui->blockSize_slider->setEnabled(true);

    ui->c_label->setEnabled(true);
    ui->cOut_label->setEnabled(true);
    ui->C_slider->setEnabled(true);
}

void MainWindow::disableAdaptiveThresholdSetting()
{
    ui->blockSizeOut_label->setDisabled(true);
    ui->blockSize_label->setDisabled(true);
    ui->blockSize_slider->setDisabled(true);

    ui->c_label->setDisabled(true);
    ui->cOut_label->setDisabled(true);
    ui->C_slider->setDisabled(true);
}

void MainWindow::enableThresholdSetting()
{
    ui->threshOut_label->setEnabled(true);
    ui->thresh_slider->setEnabled(true);
}

void MainWindow::disableThresholdSetting()
{
    ui->threshOut_label->setDisabled(true);
    ui->thresh_slider->setDisabled(true);
}

void MainWindow::enableDilateSetting()
{
    ui->dilateSize_lineEdit->setEnabled(true);
    ui->dilateSize_label->setEnabled(true);
}

void MainWindow::disableDilateSetting()
{
    ui->dilateSize_lineEdit->setDisabled(true);
    ui->dilateSize_label->setDisabled(true);
}

void MainWindow::enableCannySetting()
{
    ui->firstThreshOut_label->setEnabled(true);
    ui->firstThresh_label->setEnabled(true);
    ui->firstThresh_slider->setEnabled(true);

    ui->secondThreshOut_label->setEnabled(true);
    ui->secondThresh_label->setEnabled(true);
    ui->secondThresh_slider->setEnabled(true);

    ui->apertureSize_label->setEnabled(true);
    ui->apertureSize_lineEdit->setEnabled(true);
}

void MainWindow::disableCannySetting()
{
    ui->firstThreshOut_label->setDisabled(true);
    ui->firstThresh_label->setDisabled(true);
    ui->firstThresh_slider->setDisabled(true);

    ui->secondThreshOut_label->setDisabled(true);
    ui->secondThresh_label->setDisabled(true);
    ui->secondThresh_slider->setDisabled(true);

    ui->apertureSize_label->setDisabled(true);
    ui->apertureSize_lineEdit->setDisabled(true);
}

void MainWindow::updateFilterSetting()
{
    filterSetting->setUseUndisort(ui->undisort_checkBox->isChecked());

    filterSetting->setUseCrop(ui->crop_checkBox->isChecked());
    filterSetting->setCrop_firstX(ui->fX_lineEdit->text().toInt());
    filterSetting->setCrop_firstY(ui->fY_lineEdit->text().toInt());
    filterSetting->setCrop_secondX(ui->sX_lineEdit->text().toInt());
    filterSetting->setCrop_secondY(ui->sY_lineEdit->text().toInt());

    filterSetting->setUseMedianBlur(ui->medianBlur_checkBox->isChecked());
    filterSetting->setKernelSize(ui->kernelSize_lineEdit->text().toInt());

    filterSetting->setUseAdaptiveThresh(ui->adaptiveThreshold_checkBox->isChecked());
    filterSetting->setBlockSize(ui->blockSize_slider->value());
    filterSetting->setC(ui->C_slider->value());

    filterSetting->setUseThreshold(ui->thresh_checkBox->isChecked());
    filterSetting->setThreshValue(ui->thresh_slider->value());

    filterSetting->setUseDilate(ui->dilate_checkBox->isChecked());
    filterSetting->setDilateSize(ui->dilateSize_lineEdit->text().toInt());

    filterSetting->setUseCanny(ui->canny_checkBox->isChecked());
    filterSetting->setFirstThresh(ui->firstThresh_slider->value());
    filterSetting->setSecondThresh(ui->secondThresh_slider->value());
    filterSetting->setApertureSize(ui->apertureSize_lineEdit->text().toInt());
}

void MainWindow::enableCropSetting()
{
    ui->fPoint_label->setEnabled(true);
    ui->fVirgoul_label->setEnabled(true);
    ui->fX_lineEdit->setEnabled(true);
    ui->fY_lineEdit->setEnabled(true);

    ui->sPoint_label->setEnabled(true);
    ui->sVirgoul_label->setEnabled(true);
    ui->sX_lineEdit->setEnabled(true);
    ui->sY_lineEdit->setEnabled(true);

    //ui->mouse_button->setEnabled(true);
}

void MainWindow::disableCropSetting()
{
    ui->fPoint_label->setDisabled(true);
    ui->fVirgoul_label->setDisabled(true);
    ui->fX_lineEdit->setDisabled(true);
    ui->fY_lineEdit->setDisabled(true);

    ui->sPoint_label->setDisabled(true);
    ui->sVirgoul_label->setDisabled(true);
    ui->sX_lineEdit->setDisabled(true);
    ui->sY_lineEdit->setDisabled(true);

    //ui->mouse_button->setDisabled(true);
}

void MainWindow::enableFirstMission()
{
    ui->region1_groupBox->setEnabled(true);

    ui->region2_groupBox->setEnabled(true);

    ui->fMendX_lineEdit->setEnabled(true);
    ui->fMendY_lineEdit->setEnabled(true);
    ui->fMend_label->setEnabled(true);
    ui->fMVirgoul_label->setEnabled(true);
}

void MainWindow::disableFirstMission()
{
    ui->region1_groupBox->setDisabled(true);

    ui->region2_groupBox->setDisabled(true);

    ui->fMendX_lineEdit->setDisabled(true);
    ui->fMendY_lineEdit->setDisabled(true);
    ui->fMend_label->setDisabled(true);
    ui->fMVirgoul_label->setDisabled(true);
}

void MainWindow::enableSecondMission()
{
    ui->sMendX_lineEdit->setEnabled(true);
    ui->sMendY_lineEdit->setEnabled(true);
    ui->sMend_label->setEnabled(true);
}

void MainWindow::disableSecondMission()
{
    ui->sMendX_lineEdit->setDisabled(true);
    ui->sMendY_lineEdit->setDisabled(true);
    ui->sMend_label->setDisabled(true);
}

void MainWindow::enableSelectRed()
{
    ui->redSelect_button->setEnabled(true);
    ui->red_hue_lineEdit->setEnabled(true);
    ui->red_sat_lineEdit->setEnabled(true);
    ui->red_value_lineEdit->setEnabled(true);
    ui->add_red_button->setEnabled(true);
    ui->red_status_label->setEnabled(true);
}

void MainWindow::disableSelectRed()
{
    ui->redSelect_button->setDisabled(true);
    ui->red_hue_lineEdit->setDisabled(true);
    ui->red_sat_lineEdit->setDisabled(true);
    ui->red_value_lineEdit->setDisabled(true);
    ui->add_red_button->setDisabled(true);
    ui->red_status_label->setDisabled(true);
}

void MainWindow::enableSelectBlue()
{
    ui->blueSelect_button->setEnabled(true);
    ui->blue_hue_lineEdit->setEnabled(true);
    ui->blue_sat_lineEdit->setEnabled(true);
    ui->blue_value_lineEdit->setEnabled(true);
    ui->add_blue_button->setEnabled(true);
    ui->blue_status_label->setEnabled(true);
}

void MainWindow::disableSelectBlue()
{
    ui->blueSelect_button->setDisabled(true);
    ui->blue_hue_lineEdit->setDisabled(true);
    ui->blue_sat_lineEdit->setDisabled(true);
    ui->blue_value_lineEdit->setDisabled(true);
    ui->add_blue_button->setDisabled(true);
    ui->blue_status_label->setDisabled(true);
}

void MainWindow::enableSelectGreen()
{
    ui->greenSelect_button->setEnabled(true);
    ui->green_hue_lineEdit->setEnabled(true);
    ui->green_sat_lineEdit->setEnabled(true);
    ui->green_value_lineEdit->setEnabled(true);
    ui->add_green_button->setEnabled(true);
    ui->green_status_label->setEnabled(true);
}

void MainWindow::disableSelectGreen()
{
    ui->greenSelect_button->setDisabled(true);
    ui->green_hue_lineEdit->setDisabled(true);
    ui->green_sat_lineEdit->setDisabled(true);
    ui->green_value_lineEdit->setDisabled(true);
    ui->add_green_button->setDisabled(true);
    ui->green_status_label->setDisabled(true);
}

void MainWindow::enableSelectYellow()
{
    ui->yellowSelect_button->setEnabled(true);
    ui->yellow_hue_lineEdit->setEnabled(true);
    ui->yellow_sat_lineEdit->setEnabled(true);
    ui->yellow_value_lineEdit->setEnabled(true);
    ui->add_yellow_button->setEnabled(true);
    ui->yellow_status_label->setEnabled(true);
}

void MainWindow::disableSelectYellow()
{
    ui->yellowSelect_button->setDisabled(true);
    ui->yellow_hue_lineEdit->setDisabled(true);
    ui->yellow_sat_lineEdit->setDisabled(true);
    ui->yellow_value_lineEdit->setDisabled(true);
    ui->add_yellow_button->setDisabled(true);
    ui->yellow_status_label->setDisabled(true);
}

void MainWindow::enableSelectViolet()
{
    ui->violetSelect_button->setEnabled(true);
    ui->violet_hue_lineEdit->setEnabled(true);
    ui->violet_sat_lineEdit->setEnabled(true);
    ui->violet_value_lineEdit->setEnabled(true);
    ui->add_violet_button->setEnabled(true);
    ui->violet_status_label->setEnabled(true);
}

void MainWindow::disableSelectViolet()
{
    ui->violetSelect_button->setDisabled(true);
    ui->violet_hue_lineEdit->setDisabled(true);
    ui->violet_sat_lineEdit->setDisabled(true);
    ui->violet_value_lineEdit->setDisabled(true);
    ui->add_violet_button->setDisabled(true);
    ui->violet_status_label->setDisabled(true);
}

void MainWindow::enableSelectCyan()
{
    ui->cyanSelect_button->setEnabled(true);
    ui->cyan_hue_lineEdit->setEnabled(true);
    ui->cyan_sat_lineEdit->setEnabled(true);
    ui->cyan_value_lineEdit->setEnabled(true);
    ui->add_cyan_button->setEnabled(true);
    ui->cyan_status_label->setEnabled(true);
}

void MainWindow::disableSelectCyan()
{
    ui->cyanSelect_button->setDisabled(true);
    ui->cyan_hue_lineEdit->setDisabled(true);
    ui->cyan_sat_lineEdit->setDisabled(true);
    ui->cyan_value_lineEdit->setDisabled(true);
    ui->add_cyan_button->setDisabled(true);
    ui->cyan_status_label->setDisabled(true);
}

void MainWindow::enableSelectBlack()
{
    ui->blackSelect_button->setEnabled(true);
    ui->black_hue_lineEdit->setEnabled(true);
    ui->black_sat_lineEdit->setEnabled(true);
    ui->black_value_lineEdit->setEnabled(true);
    ui->add_black_button->setEnabled(true);
    ui->black_status_label->setEnabled(true);
}

void MainWindow::disableSelectBlack()
{
    ui->blackSelect_button->setDisabled(true);
    ui->black_hue_lineEdit->setDisabled(true);
    ui->black_sat_lineEdit->setDisabled(true);
    ui->black_value_lineEdit->setDisabled(true);
    ui->add_black_button->setDisabled(true);
    ui->black_status_label->setDisabled(true);
}

bool MainWindow::isValidPlaceForSelect(int x, int y)
{
    bool isValid = false ;

    if((x > ui->outputLabel->x() && x<(ui->outputLabel->x()+ui->outputLabel->width())))
    {
        if(y>ui->outputLabel->y() && y<ui->outputLabel->y()+ui->outputLabel->height())
        {
            isValid = true;
        }
    }

    return isValid;
}

void MainWindow::setInitializeMessage(int mission)
{
    switch(mission)
    {
    case 1 :
    {
        this->mission = 1;

        shapeWorker->result.set_mission(1);
        shapeWorker->result.set_type(0);
        shapeWorker->result.set_numberofshape(0);

        shapeWorker->result.set_mission1_isvalid(true);
        shapeWorker->result.set_mission1_region1_tl_x(ui->region1_tlX_lineEdit->text().toFloat());
        shapeWorker->result.set_mission1_region1_tl_y(ui->region1_tlY_lineEdit->text().toFloat());
        shapeWorker->result.set_mission1_region1_br_x(ui->region1_brX_lineEdit->text().toFloat());
        shapeWorker->result.set_mission1_region1_br_y(ui->region1_brY_lineEdit->text().toFloat());

        shapeWorker->result.set_mission1_region2_tl_x(ui->region2_tlX_lineEdit->text().toFloat());
        shapeWorker->result.set_mission1_region2_tl_y(ui->region2_tlY_lineEdit->text().toFloat());
        shapeWorker->result.set_mission1_region2_br_x(ui->region2_brX_lineEdit->text().toFloat());
        shapeWorker->result.set_mission1_region2_br_y(ui->region2_brY_lineEdit->text().toFloat());

        shapeWorker->result.set_mission1_end_x(ui->fMendX_lineEdit->text().toFloat());
        shapeWorker->result.set_mission1_end_y(ui->fMendY_lineEdit->text().toFloat());

        break;
    }
    case 2 :
    {
        this->mission = 2;

        shapeWorker->result.set_mission(2);
        shapeWorker->result.set_type(0);
        shapeWorker->result.set_numberofshape(0);

        shapeWorker->result.set_mission2_isvalid(true);
        shapeWorker->result.set_mission2_end_x(ui->sMendX_lineEdit->text().toFloat());
        shapeWorker->result.set_mission2_end_y(ui->sMendY_lineEdit->text().toFloat());

        outputPacket_line *line1=shapeWorker->result.add_mission2_lines();
        line1->set_start_x(1300);
        line1->set_start_y(-1500);
        line1->set_end_x(1700);
        line1->set_end_y(0);

        outputPacket_line *line2=shapeWorker->result.add_mission2_lines();
        line2->set_start_x(1700);
        line2->set_start_y(0);
        line2->set_end_x(1300);
        line2->set_end_y(1500);
        break;
    }
    case 3:
    {
        this->mission = 3;

        shapeWorker->result.set_mission(3);
        shapeWorker->result.set_type(0);
        shapeWorker->result.set_numberofshape(0);

        shapeWorker->result.set_mission3_isvalid(true);

        if(ui->attacker_rButton->isChecked())
            shapeWorker->result.set_mission3_isattacker(true);
        else if(ui->defender_rButton->isChecked())
            shapeWorker->result.set_mission3_isattacker(false);

        //Border Center
        shapeWorker->result.set_mission3_circularborde_x(ui->border_X_lineEdit->text().toFloat());
        shapeWorker->result.set_mission3_circularborde_y(ui->border_Y_lineEdit->text().toFloat());

        //goal1 center
        shapeWorker->result.set_mission3_goal1_x(ui->goal1_X_lineEdit->text().toFloat());
        shapeWorker->result.set_mission3_goal1_y(ui->goal1_Y_lineEdit->text().toFloat());

        //goal2 center
        shapeWorker->result.set_mission3_goal2_x(ui->goal2_X_lineEdit->text().toFloat());
        shapeWorker->result.set_mission3_goal2_x(ui->goal2_Y_lineEdit->text().toFloat());

        break;
    }
    }
}

void MainWindow::preapreDataForSending()
{

}

void MainWindow::openSetting(QString fileAddress)
{
    SystemSettings setting;

    fstream input;
    input.open(fileAddress.toUtf8(), ios::in | ios::binary);
    if (!input)
    {
        qDebug() << fileAddress << ": File not found.  Creating a new file." << endl;

    }
    else if (!setting.ParseFromIstream(&input))
    {
        qDebug() << "Failed";
    }
    else
    {
        ui->camSet_checkBox->setCheckable(setting.input_edit_camera_setting());

        ui->blue_slider->setValue(setting.input_white_balance_blue_u());
        ui->blueOut_label->setText(QString::number(setting.input_white_balance_blue_u()));

        ui->red_slider->setValue(setting.input_white_balance_red_v());
        ui->redOut_label->setText(QString::number(setting.input_white_balance_red_v()));

        ui->exposure_slider->setValue(setting.input_exposure());
        ui->expoOut_label->setText(QString::number(setting.input_exposure()));

        ui->brightness_slider->setValue(setting.input_brightness());
        ui->brightnessOut_label->setText(QString::number(setting.input_brightness()));

        ui->sharpness_slider->setValue(setting.input_sharpness());
        ui->sharpnessOut_label->setText(QString::number(setting.input_sharpness()));

        ui->gain_slider->setValue(setting.input_gain());
        ui->gainOut_label->setText(QString::number(setting.input_gain()));

        ui->ip_lineEdit->setText(QString::fromStdString(setting.input_network_ip()));
        ui->port_lineEdit->setText(QString::fromStdString(setting.input_network_port()));

        ui->crop_checkBox->setChecked(setting.filters_crop_photo());
        ui->fX_lineEdit->setText(QString::fromStdString(setting.filters_crop_firstpoint_x()));
        ui->fY_lineEdit->setText(QString::fromStdString(setting.filters_crop_firstpoint_y()));
        ui->sX_lineEdit->setText(QString::fromStdString(setting.filters_crop_secondpoint_x()));
        ui->sY_lineEdit->setText(QString::fromStdString(setting.filters_crop_secondpoint_y()));

        ui->medianBlur_checkBox->setChecked(setting.filters_median_blur());
        ui->kernelSize_lineEdit->setText(QString::fromStdString(setting.filters_median_blur_kernelsize()));

        ui->adaptiveThreshold_checkBox->setChecked(setting.filters_adaptive_threshold());
        ui->blockSize_slider->setValue(setting.filters_adaptive_threshold_blocksize());
        ui->blockSizeOut_label->setText(QString::number(setting.filters_adaptive_threshold_blocksize()));
        ui->C_slider->setValue(setting.filters_adaptive_threshold_c());
        ui->cOut_label->setText(QString::number(setting.filters_adaptive_threshold_c()));

        ui->thresh_checkBox->setChecked(setting.filters_threshold());
        ui->thresh_slider->setValue(setting.filters_threshold_value());
        ui->threshOut_label->setText(QString::number(setting.filters_threshold_value()));

        ui->dilate_checkBox->setChecked(setting.filters_dilate());
        ui->dilateSize_lineEdit->setText(QString::fromStdString(setting.filters_dilationsize()));

        ui->canny_checkBox->setChecked(setting.filters_canny());
        ui->firstThresh_slider->setValue(setting.filters_canny_first_threshold());
        ui->firstThreshOut_label->setText(QString::number(setting.filters_canny_first_threshold()));
        ui->secondThresh_slider->setValue(setting.filters_canny_second_threshold());
        ui->secondThreshOut_label->setText(QString::number(setting.filters_canny_second_threshold()));
        ui->apertureSize_lineEdit->setText(QString::fromStdString(setting.filters_canny_aperturesize()));

        ui->use_red_checkBox->setChecked(setting.have_red());
        //Add red Colors
        for(int i=0;i<setting.red_instances_size();i++)
        {
            Vec2f colorSample;
            colorSample.val[0] = setting.red_instances(i).min_hue();
            colorSample.val[1] = setting.red_instances(i).max_hue();
            //colorSample.val[2] = setting.red_instances(i).val();
            shapeWorker->colorDetector->red_samples.push_back(colorSample);
        }
        ui->red_status_label->setText(QString::number(shapeWorker->colorDetector->red_samples.size())+" Color at list.");

        ui->use_blue_checkBox->setChecked(setting.have_blue());
        //Add blue colors
        for(int i=0;i<setting.blue_instances_size();i++)
        {
            Vec2f colorSample;
            colorSample.val[0] = setting.blue_instances(i).min_hue();
            colorSample.val[1] = setting.blue_instances(i).max_hue();
            //colorSample.val[2] = setting.blue_instances(i).val();
            shapeWorker->colorDetector->blue_samples.push_back(colorSample);
        }
        ui->blue_status_label->setText(QString::number(shapeWorker->colorDetector->blue_samples.size())+" Color at list.");

        ui->use_green_checkBox->setChecked(setting.have_green());
        //Add green colors
        for(int i=0;i<setting.green_instances_size();i++)
        {
            Vec2f colorSample;
            colorSample.val[0] = setting.green_instances(i).min_hue();
            colorSample.val[1] = setting.green_instances(i).max_hue();
            //colorSample.val[2] = setting.green_instances(i).val();
            shapeWorker->colorDetector->green_samples.push_back(colorSample);
        }
        ui->green_status_label->setText(QString::number(shapeWorker->colorDetector->green_samples.size())+" Color at list.");

        ui->use_yellow_checkBox->setChecked(setting.have_yellow());
        //Add yellow colors
        for(int i=0;i<setting.yellow_instances_size();i++)
        {
            Vec2f colorSample;
            colorSample.val[0] = setting.yellow_instances(i).min_hue();
            colorSample.val[1] = setting.yellow_instances(i).max_hue();
            //colorSample.val[2] = setting.yellow_instances(i).val();
            shapeWorker->colorDetector->yellow_samples.push_back(colorSample);
        }
        ui->yellow_status_label->setText(QString::number(shapeWorker->colorDetector->yellow_samples.size())+" Color at list.");

        ui->use_violet_checkBox->setChecked(setting.have_violet());
        //Add violet colors
        for(int i=0;i<setting.violet_instances_size();i++)
        {
            Vec2f colorSample;
            colorSample.val[0] = setting.violet_instances(i).min_hue();
            colorSample.val[1] = setting.violet_instances(i).max_hue();
            //colorSample.val[2] = setting.violet_instances(i).val();
            shapeWorker->colorDetector->violet_samples.push_back(colorSample);
        }
        ui->violet_status_label->setText(QString::number(shapeWorker->colorDetector->violet_samples.size())+" Color at list.");

        ui->use_cyan_checkBox->setChecked(setting.have_cyan());
        //Add cyan colors
        for(int i=0;i<setting.cyan_instances_size();i++)
        {
            Vec2f colorSample;
            colorSample.val[0] = setting.cyan_instances(i).min_hue();
            colorSample.val[1] = setting.cyan_instances(i).max_hue();
            //colorSample.val[2] = setting.cyan_instances(i).val();
            shapeWorker->colorDetector->cyan_samples.push_back(colorSample);
        }
        ui->cyan_status_label->setText(QString::number(shapeWorker->colorDetector->cyan_samples.size())+" Color at list.");

        ui->use_black_checkBox->setChecked(setting.have_black());
        //Add black colors
        for(int i=0;i<setting.black_instances_size();i++)
        {
            Vec2f colorSample;
            colorSample.val[0] = setting.black_instances(i).min_hue();
            colorSample.val[1] = setting.black_instances(i).max_hue();
            //colorSample.val[2] = setting.black_instances(i).val();
            shapeWorker->colorDetector->black_samples.push_back(colorSample);
        }
        ui->black_status_label->setText(QString::number(shapeWorker->colorDetector->black_samples.size())+" Color at list.");
    }
}

void MainWindow::on_camSet_checkBox_stateChanged()
{
    if(ui->camSet_checkBox->isChecked())
    {
        enableCameraSetting();
    }
    else
    {
        disableCameraSetting();
    }
}

void MainWindow::on_xml_checkBox_stateChanged()
{
    if(ui->xml_checkBox->isChecked())
    {
        enableXML();
    }
    else
    {
        disableXML();
    }
}

void MainWindow::on_xml_button_clicked()
{
    QString fileAddress = QFileDialog::getOpenFileName(this,tr("Select Your XML File"), "/home", tr("XML Files (*.xml)"));
    ui->xmlAdd_lineEdit->setText(fileAddress);
    QFile inputXMLFile(fileAddress);
    QXmlStreamReader cameraXMLSetting(&inputXMLFile);

    while (!cameraXMLSetting.atEnd() && !cameraXMLSetting.hasError())
    {
        qDebug()<<"reading xml file";
        cameraXMLSetting.readNext();
        if (cameraXMLSetting.isStartElement())
        {

        }
        else if (cameraXMLSetting.hasError())
        {
            qDebug() << "XML error: " << cameraXMLSetting.errorString() << endl;
        }
        else if (cameraXMLSetting.atEnd())
        {
            qDebug() << "Reached end, done" << endl;
        }
    }
}

void MainWindow::on_cont_checkBox_stateChanged()
{
    emit showSettingChanged();
}

void MainWindow::on_geom_checkBox_stateChanged()
{
    emit showSettingChanged();
}

void MainWindow::on_bound_checkBox_stateChanged()
{
    emit showSettingChanged();
}

void MainWindow::on_rotate_checkBox_stateChanged()
{
    emit showSettingChanged();
}

void MainWindow::on_medianBlur_checkBox_stateChanged()
{
    if(ui->medianBlur_checkBox->isChecked())
    {
        enableMedianBlur();
    }
    else
    {
        disableMedianBlur();
    }
    emit filterSettingChanged();
}

void MainWindow::on_adaptiveThreshold_checkBox_stateChanged()
{
    if(ui->adaptiveThreshold_checkBox->isChecked())
    {
        enableAdaptiveThresholdSetting();
    }
    else
    {
        disableAdaptiveThresholdSetting();
    }
    emit filterSettingChanged();
}

void MainWindow::on_thresh_checkBox_stateChanged()
{
    if(ui->thresh_checkBox->isChecked())
    {
        enableThresholdSetting();
    }
    else
    {
        disableThresholdSetting();
    }
    emit filterSettingChanged();
}

void MainWindow::on_dilate_checkBox_stateChanged()
{
    if(ui->dilate_checkBox->isChecked())
    {
        enableDilateSetting();
    }
    else
    {
        disableDilateSetting();
    }
    emit filterSettingChanged();
}

void MainWindow::on_canny_checkBox_stateChanged()
{
    if(ui->canny_checkBox->isChecked())
    {
        enableCannySetting();
    }
    else
    {
        disableCannySetting();
    }
    emit filterSettingChanged();
}

void MainWindow::on_blockSize_slider_sliderMoved(int position)
{
    ui->blockSizeOut_label->setText(QString::number(position));
    emit filterSettingChanged();
}

void MainWindow::on_C_slider_sliderMoved(int position)
{
    ui->cOut_label->setText(QString::number(position));
    emit filterSettingChanged();
}

void MainWindow::on_thresh_slider_sliderMoved(int position)
{
    ui->threshOut_label->setText(QString::number(position));
    emit filterSettingChanged();
}

void MainWindow::on_firstThresh_slider_sliderMoved(int position)
{
    ui->firstThreshOut_label->setText(QString::number(position));
    emit filterSettingChanged();
}

void MainWindow::on_secondThresh_slider_sliderMoved(int position)
{
    ui->secondThreshOut_label->setText(QString::number(position));
    emit filterSettingChanged();
}

void MainWindow::on_blue_slider_sliderMoved(int position)
{
    ui->blueOut_label->setText(QString::number(position));
    emit cameraSettingChanged();
}

void MainWindow::on_red_slider_sliderMoved(int position)
{
    ui->redOut_label->setText(QString::number(position));
    emit cameraSettingChanged();
}

void MainWindow::on_exposure_slider_sliderMoved(int position)
{
    ui->expoOut_label->setText(QString::number(position));
    emit cameraSettingChanged();
}

void MainWindow::on_brightness_slider_sliderMoved(int position)
{
    ui->brightnessOut_label->setText(QString::number(position));
    emit cameraSettingChanged();
}

void MainWindow::updateCameraSetting()
{
    setCameraSetting();
    getWorker->updateCameraSetting(camSetting);
}

void MainWindow::setCameraSetting()
{
    camSetting->set_fps(ui->fps_comboBox->currentText().toInt());
    camSetting->set_WHITE_BALANCE_BLUE_U(ui->blue_slider->value());
    camSetting->set_WHITE_BALANCE_RED_V(ui->red_slider->value());
    camSetting->set_BRIGHTNESS(ui->brightness_slider->value());
    camSetting->set_EXPOSURE(ui->exposure_slider->value());
    camSetting->set_SHARPNESS(ui->sharpness_slider->value());
    camSetting->set_GAIN(ui->gain_slider->value());
    camSetting->set_HUE(ui->hue_slider->value());
    camSetting->set_SATURATION(ui->saturation_slider->value());
    camSetting->set_CONTRAST(ui->contrast_slider->value());
}

void MainWindow::on_sharpness_slider_sliderMoved(int position)
{
    ui->sharpnessOut_label->setText(QString::number(position));
    emit cameraSettingChanged();
}

void MainWindow::on_gain_slider_sliderMoved(int position)
{
    ui->gainOut_label->setText(QString::number(position));
    emit cameraSettingChanged();
}

void MainWindow::on_mouse_button_clicked()
{
    mouseButtonClicked=!mouseButtonClicked;
    setMouseTracking(mouseButtonClicked);
    QString temp=(mouseButtonClicked)?"Turn Off\nMouse\nIntrrupt":"Turn On\nMouse\nIntrrupt";
    ui->mouse_button->setText(temp);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonRelease)
    {
        if(colorMode || mouseButtonClicked)
        {
            return QMainWindow::eventFilter(obj, event);
        }
        else
        {
            qDebug()<<"Select a mode!";
        }
    }
    else
    {
        // pass the event on to the parent class
        return QMainWindow::eventFilter(obj, event);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(isValidPlaceForSelect(event->x(),event->y()))
    {
        origin = event->pos();
        rubberBand->setGeometry(QRect(origin, QSize()));
        rubberBand->show();
        firstPointSelectedIsValid = true;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(isValidPlaceForSelect(event->x(),event->y()))
    {
        rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
    }
    else
    {
        rubberBand->hide();
        firstPointSelectedIsValid = false;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(isValidPlaceForSelect(event->x(),event->y()))
    {
        if(firstPointSelectedIsValid)
        {
            if(mouseButtonClicked)
            {
                ui->fX_lineEdit->setText(QString::number(origin.x()-ui->outputLabel->x()));
                ui->fY_lineEdit->setText(QString::number(origin.y()-ui->outputLabel->y()));

                ui->sX_lineEdit->setText(QString::number(event->x()-ui->outputLabel->x()));
                ui->sY_lineEdit->setText(QString::number(event->y()-ui->outputLabel->y()));
            }

            if(colorMode)
            {
                semaphoreForColorImage->acquire(1);
                Mat reference;
                frameForColorDetection.copyTo(reference);
                semaphoreForColorImage->release(1);

                Rect cropedRect;
                cropedRect.width = event->x()-origin.x();
                cropedRect.height = event->y()-origin.y();
                cropedRect.x = origin.x()-ui->outputLabel->x();
                cropedRect.y = origin.y()-ui->outputLabel->y();

                Mat CropFrame(reference,cropedRect);

                Vec2f color = shapeWorker->colorDetector->returnColor(CropFrame,reference);

                if(whichColor == "red")
                {
                    ui->red_hue_lineEdit->setText(QString::number((int)color.val[0]));
                    ui->red_sat_lineEdit->setText(QString::number((int)color.val[1]));
                    //ui->red_value_lineEdit->setText(QString::number((int)color.val[2]));
                }
                else if(whichColor == "blue")
                {
                    ui->blue_hue_lineEdit->setText(QString::number((int)color.val[0]));
                    ui->blue_sat_lineEdit->setText(QString::number((int)color.val[1]));
                    //ui->blue_value_lineEdit->setText(QString::number((int)color.val[2]));
                }
                else if(whichColor == "green")
                {
                    ui->green_hue_lineEdit->setText(QString::number((int)color.val[0]));
                    ui->green_sat_lineEdit->setText(QString::number((int)color.val[1]));
                    //ui->green_value_lineEdit->setText(QString::number((int)color.val[2]));
                }
                else if(whichColor == "yellow")
                {
                    ui->yellow_hue_lineEdit->setText(QString::number((int)color.val[0]));
                    ui->yellow_sat_lineEdit->setText(QString::number((int)color.val[1]));
                    //ui->yellow_value_lineEdit->setText(QString::number((int)color.val[2]));
                }
                else if(whichColor == "cyan")
                {
                    ui->cyan_hue_lineEdit->setText(QString::number((int)color.val[0]));
                    ui->cyan_sat_lineEdit->setText(QString::number((int)color.val[1]));
                    //ui->cyan_value_lineEdit->setText(QString::number((int)color.val[2]));
                }
                else if(whichColor == "violet")
                {
                    ui->violet_hue_lineEdit->setText(QString::number((int)color.val[0]));
                    ui->violet_sat_lineEdit->setText(QString::number((int)color.val[1]));
                    //ui->violet_value_lineEdit->setText(QString::number((int)color.val[2]));
                }
                else if(whichColor == "black")
                {
                    ui->black_hue_lineEdit->setText(QString::number((int)color.val[0]));
                    ui->black_sat_lineEdit->setText(QString::number((int)color.val[1]));
                    //ui->black_value_lineEdit->setText(QString::number((int)color.val[2]));
                }
            }
        }
    }

    rubberBand->hide();
    firstPointSelectedIsValid = false;
}

void MainWindow::on_drawCrop_checkBox_stateChanged()
{
    emit showSettingChanged();
}

void MainWindow::on_firstM_rButton_toggled(bool checked)
{
    if(checked)
    {
        enableFirstMission();
    }
    else
    {
        disableFirstMission();
    }
}

void MainWindow::on_secondM_rButton_toggled(bool checked)
{
    if(checked)
    {
        enableSecondMission();
    }
    else
    {
        disableSecondMission();
    }
}

void MainWindow::on_go_button_clicked()
{
    shapeWorker->result.Clear();

    if(ui->firstM_rButton->isChecked())
    {
        setInitializeMessage(1);
        emit dataReadyForSend();
        permissionForSending = true;
    }
    else if(ui->secondM_rButton->isChecked())
    {
        setInitializeMessage(2);
        emit dataReadyForSend();
        permissionForSending = true;
    }
    else if(ui->thirsM_rButton->isChecked())
    {
        setInitializeMessage(3);
        emit dataReadyForSend();
        permissionForSending = true;
    }
    else
    {
        qDebug()<<"Error : Select a Mission!";
    }
}

void MainWindow::on_thirsM_rButton_toggled(bool checked)
{
    if(checked)
    {
        ui->role_groupBox->setEnabled(true);
        ui->regions_groupBox->setEnabled(true);
    }
    else
    {
        ui->role_groupBox->setDisabled(true);
        ui->regions_groupBox->setDisabled(true);
    }
}

void MainWindow::sendDataPacket()
{
    access2StallMode->acquire(1);
    if(!stallMode)
    {
        qDebug()<<"--------------";
        qDebug()<<"mission:"<<shapeWorker->result.mission();
        qDebug()<<"number:"<<shapeWorker->result.numberofshape();
        qDebug()<<"type:"<<shapeWorker->result.type();
        for(int i=0;i<shapeWorker->result.shapes_size();i++)
        {
            qDebug()<<"shape "<<i<<" seen at:"<<shapeWorker->result.shapes(i).position_x()<<","<<shapeWorker->result.shapes(i).position_y();
            qDebug()<<"color:"<<QString::fromStdString(shapeWorker->result.shapes(i).color());
        }
        qDebug()<<"--------------";
    }
    else
    {
        shapeWorker->result.set_mission(0);
    }
    access2StallMode->release(1);

    string data;
    shapeWorker->result.SerializeToString(&data);
    QString s = QString::fromStdString(data);
    QByteArray q_data;
    q_data.append(s);
    sendingSocket->sendData(q_data);
}

void MainWindow::on_use_blue_checkBox_toggled(bool checked)
{
    if(checked)
    {
        enableSelectBlue();
    }
    else
    {
        disableSelectBlue();
    }
}

void MainWindow::on_use_red_checkBox_toggled(bool checked)
{
    if(checked)
    {
        enableSelectRed();
    }
    else
    {
        disableSelectRed();
    }
}

void MainWindow::on_use_green_checkBox_toggled(bool checked)
{
    if(checked)
    {
        enableSelectGreen();
    }
    else
    {
        disableSelectGreen();
    }
}

void MainWindow::on_use_yellow_checkBox_toggled(bool checked)
{
    if(checked)
    {
        enableSelectYellow();
    }
    else
    {
        disableSelectYellow();
    }
}

void MainWindow::on_use_violet_checkBox_toggled(bool checked)
{
    if(checked)
    {
        enableSelectViolet();
    }
    else
    {
        disableSelectViolet();
    }
}

void MainWindow::on_use_cyan_checkBox_toggled(bool checked)
{
    if(checked)
    {
        enableSelectCyan();
    }
    else
    {
        disableSelectCyan();
    }
}

void MainWindow::on_use_black_checkBox_toggled(bool checked)
{
    if(checked)
    {
        enableSelectBlack();
    }
    else
    {
        disableSelectBlack();
    }
}

void MainWindow::on_redSelect_button_clicked()
{
    whichColor = "red";
    colorMode = true;
    QApplication::setOverrideCursor(Qt::CrossCursor);
}

void MainWindow::on_blueSelect_button_clicked()
{
    whichColor = "blue";
    colorMode = true;
    QApplication::setOverrideCursor(Qt::CrossCursor);
}

void MainWindow::on_greenSelect_button_clicked()
{
    whichColor = "green";
    colorMode = true;
    QApplication::setOverrideCursor(Qt::CrossCursor);
}

void MainWindow::on_yellowSelect_button_clicked()
{
    whichColor = "yellow";
    colorMode = true;
    QApplication::setOverrideCursor(Qt::CrossCursor);
}

void MainWindow::on_violetSelect_button_clicked()
{
    whichColor = "violet";
    colorMode = true;
    QApplication::setOverrideCursor(Qt::CrossCursor);
}

void MainWindow::on_cyanSelect_button_clicked()
{
    whichColor = "cyan";
    colorMode = true;
    QApplication::setOverrideCursor(Qt::CrossCursor);
}

void MainWindow::on_blackSelect_button_clicked()
{
    whichColor = "black";
    colorMode = true;
    QApplication::setOverrideCursor(Qt::CrossCursor);
}

void MainWindow::on_save_set_button_clicked()
{
    SystemSettings setting;

    setting.set_input_edit_camera_setting(ui->camSet_checkBox->isChecked());
    setting.set_input_white_balance_blue_u(ui->blue_slider->value());
    setting.set_input_white_balance_red_v(ui->red_slider->value());
    setting.set_input_exposure(ui->exposure_slider->value());
    setting.set_input_brightness(ui->brightness_slider->value());
    setting.set_input_sharpness(ui->sharpness_slider->value());
    setting.set_input_gain(ui->gain_slider->value());
    setting.set_input_network_ip(ui->ip_lineEdit->text().toStdString());
    setting.set_input_network_port(ui->port_lineEdit->text().toStdString());

    setting.set_filters_crop_photo(ui->crop_checkBox->isChecked());
    setting.set_filters_crop_firstpoint_x(ui->fX_lineEdit->text().toStdString());
    setting.set_filters_crop_firstpoint_y(ui->fY_lineEdit->text().toStdString());
    setting.set_filters_crop_secondpoint_x(ui->sX_lineEdit->text().toStdString());
    setting.set_filters_crop_secondpoint_y(ui->sY_lineEdit->text().toStdString());

    setting.set_filters_median_blur(ui->medianBlur_checkBox->isChecked());
    setting.set_filters_median_blur_kernelsize(ui->kernelSize_lineEdit->text().toStdString());

    setting.set_filters_adaptive_threshold(ui->adaptiveThreshold_checkBox->isChecked());
    setting.set_filters_adaptive_threshold_blocksize(ui->blockSize_slider->value());
    setting.set_filters_adaptive_threshold_c(ui->C_slider->value());

    setting.set_filters_threshold(ui->thresh_checkBox->isChecked());
    setting.set_filters_threshold_value(ui->thresh_slider->value());

    setting.set_filters_dilate(ui->dilate_checkBox->isChecked());
    setting.set_filters_dilationsize(ui->dilateSize_lineEdit->text().toStdString());

    setting.set_filters_canny(ui->canny_checkBox->isChecked());
    setting.set_filters_canny_aperturesize(ui->apertureSize_lineEdit->text().toStdString());
    setting.set_filters_canny_first_threshold(ui->firstThresh_slider->value());
    setting.set_filters_canny_second_threshold(ui->secondThresh_slider->value());

    setting.set_have_red(ui->use_red_checkBox->isChecked());
    setting.set_have_blue(ui->use_blue_checkBox->isChecked());
    setting.set_have_green(ui->use_green_checkBox->isChecked());
    setting.set_have_yellow(ui->use_yellow_checkBox->isChecked());
    setting.set_have_violet(ui->use_violet_checkBox->isChecked());
    setting.set_have_cyan(ui->use_cyan_checkBox->isChecked());
    setting.set_have_black(ui->use_black_checkBox->isChecked());

//    //Add Colors!
    for(int i=0;i<shapeWorker->colorDetector->red_samples.size();i++)
    {
        SystemSettings_color *color = setting.add_red_instances();
        color->set_min_hue(shapeWorker->colorDetector->red_samples.at(i).val[0]);
        color->set_max_hue(shapeWorker->colorDetector->red_samples.at(i).val[1]);
        //color->set_val(shapeWorker->colorDetector->red_samples.at(i).val[2]);
    }

    for(int i=0;i<shapeWorker->colorDetector->blue_samples.size();i++)
    {
        SystemSettings_color *color = setting.add_blue_instances();
        color->set_min_hue(shapeWorker->colorDetector->blue_samples.at(i).val[0]);
        color->set_max_hue(shapeWorker->colorDetector->blue_samples.at(i).val[1]);
        //color->set_val(shapeWorker->colorDetector->blue_samples.at(i).val[2]);
    }

    for(int i=0;i<shapeWorker->colorDetector->green_samples.size();i++)
    {
        SystemSettings_color *color = setting.add_green_instances();
        color->set_max_hue(shapeWorker->colorDetector->green_samples.at(i).val[0]);
        color->set_max_hue(shapeWorker->colorDetector->green_samples.at(i).val[1]);
        //color->set_val(shapeWorker->colorDetector->green_samples.at(i).val[2]);
    }

    for(int i=0;i<shapeWorker->colorDetector->yellow_samples.size();i++)
    {
        SystemSettings_color *color = setting.add_yellow_instances();
        color->set_max_hue(shapeWorker->colorDetector->yellow_samples.at(i).val[0]);
        color->set_max_hue(shapeWorker->colorDetector->yellow_samples.at(i).val[1]);
        //color->set_val(shapeWorker->colorDetector->yellow_samples.at(i).val[2]);
    }

    for(int i=0;i<shapeWorker->colorDetector->violet_samples.size();i++)
    {
        SystemSettings_color *color = setting.add_violet_instances();
        color->set_max_hue(shapeWorker->colorDetector->violet_samples.at(i).val[0]);
        color->set_max_hue(shapeWorker->colorDetector->violet_samples.at(i).val[1]);
        //color->set_val(shapeWorker->colorDetector->violet_samples.at(i).val[2]);
    }

    for(int i=0;i<shapeWorker->colorDetector->cyan_samples.size();i++)
    {
        SystemSettings_color *color = setting.add_cyan_instances();
        color->set_max_hue(shapeWorker->colorDetector->cyan_samples.at(i).val[0]);
        color->set_max_hue(shapeWorker->colorDetector->cyan_samples.at(i).val[1]);
        //color->set_val(shapeWorker->colorDetector->cyan_samples.at(i).val[2]);
    }

    for(int i=0;i<shapeWorker->colorDetector->black_samples.size();i++)
    {
        SystemSettings_color *color = setting.add_black_instances();
        color->set_max_hue(shapeWorker->colorDetector->black_samples.at(i).val[0]);
        color->set_max_hue(shapeWorker->colorDetector->black_samples.at(i).val[1]);
        //color->set_val(shapeWorker->colorDetector->black_samples.at(i).val[2]);
    }

    QString fileAddress = QFileDialog::getSaveFileName(this,tr("Select Directory..."), "/home", tr("Text File (*.txt)"));
    QFile file(fileAddress);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text ))
    {
        qDebug() << "File Has Been Created" << endl;
        fstream output;
        output.open(fileAddress.toUtf8(), fstream::out | fstream::trunc | fstream::binary);;
        if (!setting.SerializeToOstream(&output)) {
            qDebug() << "Failed to write data." << endl;
            //file.close();
        }
    }
    else
    {
        qDebug() << "Failed to Create File" << endl;
    }
}

void MainWindow::on_open_set_button_clicked()
{
    QString fileAddress = QFileDialog::getOpenFileName(this,tr("Select Setting File"), "/home", tr("Text File (*.txt)"));

    openSetting(fileAddress);
}

void MainWindow::on_add_red_button_clicked()
{
//    Vec3b sample;
//    sample.val[0] = ui->red_hue_lineEdit->text().toInt();
//    sample.val[1] = ui->red_sat_lineEdit->text().toInt();
//    sample.val[2] = ui->red_value_lineEdit->text().toInt();
//    imageProcessor->red_samples.push_back(sample);
    Vec2f sample;
    sample.val[0] = ui->red_hue_lineEdit->text().toFloat();
    sample.val[1] = ui->red_sat_lineEdit->text().toFloat();
    //sample.val[2] = ui->red_value_lineEdit->text().toInt();
    shapeWorker->colorDetector->red_samples.push_back(sample);

    QString status;
    status = QString::number(shapeWorker->colorDetector->red_samples.size()) + " Color at list.";
    ui->red_status_label->setText(status);
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    colorMode = false;
}

void MainWindow::on_add_blue_button_clicked()
{
    Vec2f sample;
    sample.val[0] = ui->blue_hue_lineEdit->text().toFloat();
    sample.val[1] = ui->blue_sat_lineEdit->text().toFloat();
    //sample.val[2] = ui->blue_value_lineEdit->text().toInt();
    shapeWorker->colorDetector->blue_samples.push_back(sample);
    QString status;
    status = QString::number(shapeWorker->colorDetector->blue_samples.size()) + " Color at list.";
    ui->blue_status_label->setText(status);
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    colorMode = false;
}

void MainWindow::on_add_green_button_clicked()
{
    Vec2f sample;
    sample.val[0] = ui->green_hue_lineEdit->text().toFloat();
    sample.val[1] = ui->green_sat_lineEdit->text().toFloat();
    //sample.val[2] = ui->green_value_lineEdit->text().toInt();
    shapeWorker->colorDetector->green_samples.push_back(sample);
    QString status;
    status = QString::number(shapeWorker->colorDetector->green_samples.size()) + " Color at list.";
    ui->green_status_label->setText(status);
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    colorMode = false;
}

void MainWindow::on_add_yellow_button_clicked()
{
    Vec2f sample;
    sample.val[0] = ui->yellow_hue_lineEdit->text().toFloat();
    sample.val[1] = ui->yellow_sat_lineEdit->text().toFloat();
    //sample.val[2] = ui->yellow_value_lineEdit->text().toInt();
    shapeWorker->colorDetector->yellow_samples.push_back(sample);
    QString status;
    status = QString::number(shapeWorker->colorDetector->yellow_samples.size()) + " Color at list.";
    ui->yellow_status_label->setText(status);
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    colorMode = false;
}

void MainWindow::on_add_violet_button_clicked()
{
    Vec2f sample;
    sample.val[0] = ui->violet_hue_lineEdit->text().toFloat();
    sample.val[1] = ui->violet_sat_lineEdit->text().toFloat();
    //sample.val[2] = ui->violet_value_lineEdit->text().toInt();
    shapeWorker->colorDetector->violet_samples.push_back(sample);
    QString status;
    status = QString::number(shapeWorker->colorDetector->violet_samples.size()) + " Color at list.";
    ui->violet_status_label->setText(status);
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    colorMode = false;
}

void MainWindow::on_add_cyan_button_clicked()
{
    Vec2f sample;
    sample.val[0] = ui->cyan_hue_lineEdit->text().toFloat();
    sample.val[1] = ui->cyan_sat_lineEdit->text().toFloat();
    //sample.val[2] = ui->cyan_value_lineEdit->text().toInt();
    shapeWorker->colorDetector->cyan_samples.push_back(sample);
    QString status;
    status = QString::number(shapeWorker->colorDetector->cyan_samples.size()) + " Color at list.";
    ui->cyan_status_label->setText(status);
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    colorMode = false;
}

void MainWindow::on_add_black_button_clicked()
{
    Vec2f sample;
    sample.val[0] = ui->black_hue_lineEdit->text().toFloat();
    sample.val[1] = ui->black_sat_lineEdit->text().toFloat();
    //sample.val[2] = ui->black_value_lineEdit->text().toInt();
    shapeWorker->colorDetector->black_samples.push_back(sample);
    QString status;
    status = QString::number(shapeWorker->colorDetector->black_samples.size()) + " Color at list.";
    ui->black_status_label->setText(status);
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    colorMode = false;
}

void MainWindow::on_cancel_button_clicked()
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    colorMode = false;
}

void MainWindow::on_stall_button_clicked()
{
    access2StallMode->acquire(1);
    stallMode = !stallMode;
    QString temp=(stallMode)?"Resume":"Stall";
    ui->stall_button->setText(temp);
    access2StallMode->release(1);
}

void MainWindow::on_clear_red_button_clicked()
{
    shapeWorker->colorDetector->red_samples.clear();
    ui->red_status_label->setText(QString::number(shapeWorker->colorDetector->red_samples.size()) + " Color at list.");
    ui->red_hue_lineEdit->clear();
    ui->red_sat_lineEdit->clear();
    ui->red_value_lineEdit->clear();
}

void MainWindow::on_clear_blue_button_clicked()
{
    shapeWorker->colorDetector->blue_samples.clear();
    ui->blue_status_label->setText(QString::number(shapeWorker->colorDetector->blue_samples.size()) + " Color at list.");
    ui->blue_hue_lineEdit->clear();
    ui->blue_sat_lineEdit->clear();
    ui->blue_value_lineEdit->clear();
}

void MainWindow::on_clear_green_button_clicked()
{
    shapeWorker->colorDetector->green_samples.clear();
    ui->green_status_label->setText(QString::number(shapeWorker->colorDetector->green_samples.size()) + " Color at list.");
    ui->green_hue_lineEdit->clear();
    ui->green_sat_lineEdit->clear();
    ui->green_value_lineEdit->clear();
}

void MainWindow::on_clear_yellow_button_clicked()
{
    shapeWorker->colorDetector->yellow_samples.clear();
    ui->yellow_status_label->setText(QString::number(shapeWorker->colorDetector->yellow_samples.size()) + " Color at list.");
    ui->yellow_hue_lineEdit->clear();
    ui->yellow_sat_lineEdit->clear();
    ui->yellow_value_lineEdit->clear();
}

void MainWindow::on_clear_violet_button_clicked()
{
    shapeWorker->colorDetector->violet_samples.clear();
    ui->violet_status_label->setText(QString::number(shapeWorker->colorDetector->violet_samples.size()) + " Color at list.");
    ui->violet_hue_lineEdit->clear();
    ui->violet_sat_lineEdit->clear();
    ui->violet_value_lineEdit->clear();
}

void MainWindow::on_clear_cyan_button_clicked()
{
    shapeWorker->colorDetector->cyan_samples.clear();
    ui->cyan_status_label->setText(QString::number(shapeWorker->colorDetector->cyan_samples.size()) + " Color at list.");
    ui->cyan_hue_lineEdit->clear();
    ui->cyan_sat_lineEdit->clear();
    ui->cyan_value_lineEdit->clear();
}

void MainWindow::on_clear_black_button_clicked()
{
    shapeWorker->colorDetector->black_samples.clear();
    ui->black_status_label->setText(QString::number(shapeWorker->colorDetector->black_samples.size()) + " Color at list.");
    ui->black_hue_lineEdit->clear();
    ui->black_sat_lineEdit->clear();
    ui->black_value_lineEdit->clear();
}

void MainWindow::responseForFilterSettingsChanged()
{
    updateFilterSetting();
    //imageProcessor->updateFilterSettings(filterSetting);
}

void MainWindow::on_kernelSize_lineEdit_textChanged()
{
    emit filterSettingChanged();
}

void MainWindow::on_dilateSize_lineEdit_textChanged()
{
    emit filterSettingChanged();
}

void MainWindow::on_apertureSize_lineEdit_textChanged()
{
    emit filterSettingChanged();
}

void MainWindow::on_crop_checkBox_stateChanged()
{
    if(ui->crop_checkBox->isChecked())
    {
        enableCropSetting();
    }
    else
    {
        disableCropSetting();
    }
    emit filterSettingChanged();
}

void MainWindow::on_fX_lineEdit_textChanged()
{
    emit filterSettingChanged();
}

void MainWindow::on_fY_lineEdit_textChanged()
{
    emit filterSettingChanged();
}

void MainWindow::on_sX_lineEdit_textChanged()
{
    emit filterSettingChanged();
}

void MainWindow::on_sY_lineEdit_textChanged()
{
    emit filterSettingChanged();
}

void MainWindow::on_undisort_checkBox_stateChanged()
{
    emit filterSettingChanged();
}

void MainWindow::on_hue_slider_sliderMoved(int position)
{
    ui->hueOut_label->setText(QString::number(position));
    emit cameraSettingChanged();
}

void MainWindow::on_saturation_slider_sliderMoved(int position)
{
    ui->saturationOut_label->setText(QString::number(position));
    emit cameraSettingChanged();
}

void MainWindow::on_contrast_slider_sliderMoved(int position)
{
    ui->contrastOut_label->setText(QString::number(position));
    emit cameraSettingChanged();
}
