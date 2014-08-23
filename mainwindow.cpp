#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cameraIsOpened=false;
    mouseButtonClicked=false;
    firstPointSelected=false;
    imProcDataAvailable=false;

    semaphore=new QSemaphore(1);
    sendingSocket = new NetworkSender();

    QStringList items;
    items<<"0"<<"1";
    ui->cam_comboBox->addItems(items);

    QStringList output_items;
    output_items<<"Croped"<<"Adaptive Threshold"<<"Threshold"<<"Canny"<<"Final";
    ui->out_comboBox->addItems(output_items);
    ui->out_comboBox->setCurrentIndex(4);

    cam_timer = new QTimer();
    send_timer = new QTimer();
    filterSetting=new filterSettings();

    connect(this,SIGNAL(imageReady(Mat)),this,SLOT(callImageProcessingFunctions(Mat)));
    connect(this,SIGNAL(cameraSettingChanged()),this,SLOT(updateCameraSetting()));
    connect(send_timer,SIGNAL(timeout()),this,SLOT(send_timer_interval()));

    imageProcessor=new ImageProcessing();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_image_rButton_toggled(bool checked)
{
    if(checked)
    {
        enableOpenImage();

    }
    else
    {
        disableOpenImage();
    }
}

void MainWindow::on_address_button_clicked()
{
    QString fileAddress = QFileDialog::getOpenFileName(this,tr("Select Your Source Image"), "/home", tr("Image Files (*.png *.jpg *.bmp)"));
    ui->address_lineEdit->setText(fileAddress);
    Mat inputFrame=imread(fileAddress.toStdString());
    emit imageReady(inputFrame);
}

void MainWindow::on_camera_rButton_toggled(bool checked)
{
    if(checked)
    {
        enableOpenCamera();
    }
    else
    {
        disableOpenCamera();
        disableCameraSetting();
        disableXML();
        cam_timer->stop();
    }
}

void MainWindow::on_open_button_clicked()
{
        sendingSocket->configure(ui->ip_lineEdit->text(),ui->port_lineEdit->text().toInt());

        Mat frame;

        if(ui->cam_comboBox->currentText()=="0")
        {
            cameraIsOpened=cap.open(CAP_FIREWIRE+0);
        }
        else
        {
            cameraIsOpened=cap.open(CAP_FIREWIRE+1);
        }

        setCameraSetting();

        cap.read(frame);

        cam_timer->start(15);
        connect(cam_timer,SIGNAL(timeout()),this,SLOT(cam_timeout()));

        emit imageReady(frame);
}

void MainWindow::cam_timeout()
{
    Mat frame;
    cap.read(frame);
    emit imageReady(frame);
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

void MainWindow::enableOpenCamera()
{
    ui->cam_label->setEnabled(true);
    ui->cam_comboBox->setEnabled(true);
    ui->open_button->setEnabled(true);
    ui->camSet_checkBox->setEnabled(true);
    ui->xml_checkBox->setEnabled(true);
}

void MainWindow::disableOpenCamera()
{
    ui->cam_label->setDisabled(true);
    ui->cam_comboBox->setDisabled(true);
    ui->open_button->setDisabled(true);
    ui->camSet_checkBox->setDisabled(true);
    ui->xml_checkBox->setDisabled(true);
}

void MainWindow::enableOpenImage()
{
    ui->add_label->setEnabled(true);
    ui->address_button->setEnabled(true);
    ui->address_lineEdit->setEnabled(true);
}

void MainWindow::disableOpenImage()
{
    ui->add_label->setDisabled(true);
    ui->address_button->setDisabled(true);
    ui->address_lineEdit->setDisabled(true);
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
    imageProcessor->changeOutputSetting(ui->cont_checkBox->isChecked(),ui->geom_checkBox->isChecked()
                                        ,ui->bound_checkBox->isChecked(),ui->rotate_checkBox->isChecked()
                                        ,ui->drawCrop_checkBox->isChecked());
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

    ui->mouse_button->setEnabled(true);
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

    ui->mouse_button->setDisabled(true);
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

void MainWindow::callImageProcessingFunctions(Mat input_mat)
{
    //undisort image
    Mat inputFrame;
    if(ui->camera_rButton->isChecked())
    {
        imageProcessor->undistortImage(input_mat).copyTo(inputFrame);
    }
    else
    {
        input_mat.copyTo(inputFrame);
    }

    //update filter setting from ui
    updateFilterSetting();
    imageProcessor->updateFilterSettings(filterSetting);

    //croped image for better performance
    Mat filteredImage;
    Mat crop;
    Rect cropedRect;
    if(ui->crop_checkBox->isChecked())
    {
        cropedRect.width = ui->sX_lineEdit->text().toInt()-ui->fX_lineEdit->text().toInt();
        cropedRect.height = ui->sY_lineEdit->text().toInt()-ui->fY_lineEdit->text().toInt();
        cropedRect.x = ui->fX_lineEdit->text().toInt();
        cropedRect.y = ui->fY_lineEdit->text().toInt();

        Mat CropFrame(inputFrame,cropedRect);
        CropFrame.copyTo(crop);
        imageProcessor->applyFilters(CropFrame).copyTo(filteredImage);
   }
    else
    {
        imageProcessor->applyFilters(inputFrame).copyTo(filteredImage);
        cropedRect.width = inputFrame.rows;
        cropedRect.height = inputFrame.cols;
        cropedRect.x = 0;
        cropedRect.y = 0;
    }

    semaphore->acquire();
    imProcDataAvailable=false;
    Mat outputFrame;
    imageProcessor->shapeDetection(filteredImage,inputFrame,cropedRect).copyTo(outputFrame);
    imageProcessor->findColors(inputFrame);
    imProcDataAvailable=true;
    semaphore->release();

    if(ui->out_comboBox->currentText() == "Croped")
    {
        crop.copyTo(outputFrame);
    }
    else if(ui->out_comboBox->currentText() == "Adaptive Threshold")
    {
        imageProcessor->returnAdaptiveThreshlodImage().copyTo(outputFrame);
    }
    else if(ui->out_comboBox->currentText() == "Threshold")
    {
        imageProcessor->returnThreshlodImage().copyTo(outputFrame);
    }
    else if(ui->out_comboBox->currentText() == "Canny")
    {
        imageProcessor->returnCannyImage().copyTo(outputFrame);
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
    updateOutputOptions();
}

void MainWindow::on_geom_checkBox_stateChanged()
{
    updateOutputOptions();
}

void MainWindow::on_bound_checkBox_stateChanged()
{
    updateOutputOptions();
}

void MainWindow::on_rotate_checkBox_stateChanged()
{
    updateOutputOptions();
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
}

void MainWindow::on_blockSize_slider_sliderMoved(int position)
{
    ui->blockSizeOut_label->setText(QString::number(position));
}

void MainWindow::on_C_slider_sliderMoved(int position)
{
    ui->cOut_label->setText(QString::number(position));
}

void MainWindow::on_thresh_slider_sliderMoved(int position)
{
    ui->threshOut_label->setText(QString::number(position));
}

void MainWindow::on_firstThresh_slider_sliderMoved(int position)
{
    ui->firstThreshOut_label->setText(QString::number(position));
}

void MainWindow::on_secondThresh_slider_sliderMoved(int position)
{
    ui->secondThreshOut_label->setText(QString::number(position));
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
}

void MainWindow::setCameraSetting()
{
    if(cameraIsOpened)
    {
        cap.set(CAP_PROP_FPS, 15);
        cap.set(CAP_PROP_WHITE_BALANCE_BLUE_U,ui->blue_slider->value());
        cap.set(CAP_PROP_WHITE_BALANCE_RED_V,ui->red_slider->value());
        cap.set(CAP_PROP_BRIGHTNESS,ui->brightness_slider->value());
        cap.set(CAP_PROP_EXPOSURE,ui->exposure_slider->value());
        cap.set(CAP_PROP_SHARPNESS,ui->sharpness_slider->value());
        cap.set(CAP_PROP_GAIN,ui->gain_slider->value());
    }
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

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
     if(mouseButtonClicked)
     {
         if(!firstPointSelected)
         {
             qDebug()<<"X:"+QString::number(event->x())+"-- Y:"+QString::number(event->y());
             ui->fX_lineEdit->setText(QString::number(event->x()));
             ui->fY_lineEdit->setText(QString::number(event->y()));
             firstPointSelected=true;
         }
         else
         {
             qDebug()<<"X:"+QString::number(event->x())+"-- Y:"+QString::number(event->y());
             ui->sX_lineEdit->setText(QString::number(event->x()));
             ui->sY_lineEdit->setText(QString::number(event->y()));
             firstPointSelected=false;
         }
     }
}

void MainWindow::on_drawCrop_checkBox_stateChanged()
{
    updateOutputOptions();
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
    imageProcessor->result.Clear();

    if(ui->firstM_rButton->isChecked())
    {
        outputPacket_vector2D tl;
        tl.set_x(ui->region1_tlX_lineEdit->text().toFloat());
        tl.set_y(ui->region1_tlY_lineEdit->text().toFloat());
        outputPacket_vector2D br;
        br.set_x(ui->region1_brX_lineEdit->text().toFloat());
        br.set_y(ui->region1_brY_lineEdit->text().toFloat());

        outputPacket_rect2D region1;
        region1.mutable_tl()->CopyFrom(tl);
        region1.mutable_br()->CopyFrom(br);

        tl.Clear();  br.Clear();
        tl.set_x(ui->region2_tlX_lineEdit->text().toFloat());
        tl.set_y(ui->region2_tlY_lineEdit->text().toFloat());
        br.set_x(ui->region2_brX_lineEdit->text().toFloat());
        br.set_y(ui->region2_brY_lineEdit->text().toFloat());
        outputPacket_rect2D region2;
        region2.mutable_tl()->CopyFrom(tl);
        region2.mutable_br()->CopyFrom(br);

        outputPacket_vector2D end;
        end.set_x(ui->fMendX_lineEdit->text().toFloat());
        end.set_y(ui->fMendY_lineEdit->text().toFloat());

        outputPacket_Mission1 mission1;
        mission1.set_isvalid(true);
        mission1.mutable_region1()->CopyFrom(region1);
        mission1.mutable_region2()->CopyFrom(region2);
        mission1.mutable_end()->CopyFrom(end);

        imageProcessor->result.set_mission(1);
        imageProcessor->result.set_type(outputPacket_MessageType_INITIALIZE);
        imageProcessor->result.mutable_mission1_data()->CopyFrom(mission1);


        //sendingSocket->sendData(imageProcessor->result.SerializeToCodedStream());
    }
    else if(ui->secondM_rButton->isChecked())
    {
        imageProcessor->result.set_mission(2);
        imageProcessor->result.set_type(outputPacket_MessageType_INITIALIZE);
        outputPacket_vector2D end;
        end.set_x(ui->sMendX_lineEdit->text().toFloat());
        end.set_y(ui->sMendY_lineEdit->text().toFloat());
        outputPacket_Mission2 mission2;
        mission2.set_isvalid(true);
        mission2.mutable_end()->CopyFrom(end);

        //sendingSocket->sendData("Mission2");
    }
    else if(ui->thirsM_rButton->isChecked())
    {
        imageProcessor->result.set_mission(3);
        imageProcessor->result.set_type(outputPacket_MessageType_INITIALIZE);
        outputPacket_Mission3 misssion3;
        misssion3.set_isvalid(true);

        if(ui->attacker_rButton->isChecked())
            misssion3.set_isattacker(true);
        else if(ui->defender_rButton->isChecked())
            misssion3.set_isattacker(false);

        outputPacket_vector2D center;
        //Border Center
        center.set_x(ui->border_X_lineEdit->text().toFloat());
        center.set_y(ui->border_Y_lineEdit->text().toFloat());
        misssion3.mutable_circularborde()->CopyFrom(center);

        center.Clear();

        //goal1 center
        center.set_x(ui->goal1_X_lineEdit->text().toFloat());
        center.set_y(ui->goal1_Y_lineEdit->text().toFloat());
        misssion3.mutable_goal1()->CopyFrom(center);

        center.Clear();

        //goal2 center
        center.set_x(ui->goal2_X_lineEdit->text().toFloat());
        center.set_y(ui->goal2_Y_lineEdit->text().toFloat());
        misssion3.mutable_goal2()->CopyFrom(center);

        imageProcessor->result.mutable_mission3_data()->CopyFrom(misssion3);

        //sendingSocket->sendData("Mission3");
    }
    else
    {
        qDebug()<<"Error : Select a Mission!";
    }

    send_timer->start(15);
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

void MainWindow::send_timer_interval()
{
    if(imProcDataAvailable)
    {
        if(ui->firstM_rButton->isChecked())
        {
            outputPacket_vector2D tl;
            tl.set_x(ui->region1_tlX_lineEdit->text().toFloat());
            tl.set_y(ui->region1_tlY_lineEdit->text().toFloat());
            outputPacket_vector2D br;
            br.set_x(ui->region1_brX_lineEdit->text().toFloat());
            br.set_y(ui->region1_brY_lineEdit->text().toFloat());

            outputPacket_rect2D region1;
            region1.mutable_tl()->CopyFrom(tl);
            region1.mutable_br()->CopyFrom(br);

            tl.Clear();  br.Clear();
            tl.set_x(ui->region2_tlX_lineEdit->text().toFloat());
            tl.set_y(ui->region2_tlY_lineEdit->text().toFloat());
            br.set_x(ui->region2_brX_lineEdit->text().toFloat());
            br.set_y(ui->region2_brY_lineEdit->text().toFloat());
            outputPacket_rect2D region2;
            region2.mutable_tl()->CopyFrom(tl);
            region2.mutable_br()->CopyFrom(br);

            outputPacket_vector2D end;
            end.set_x(ui->fMendX_lineEdit->text().toFloat());
            end.set_y(ui->fMendY_lineEdit->text().toFloat());

            outputPacket_Mission1 mission1;
            mission1.set_isvalid(true);
            mission1.mutable_region1()->CopyFrom(region1);
            mission1.mutable_region2()->CopyFrom(region2);
            mission1.mutable_end()->CopyFrom(end);

            imageProcessor->result.set_mission(1);
            imageProcessor->result.set_type(outputPacket_MessageType_INITIALIZE);
            imageProcessor->result.mutable_mission1_data()->CopyFrom(mission1);


            //sendingSocket->sendData(imageProcessor->result.SerializeToCodedStream());
        }
        else if(ui->secondM_rButton->isChecked())
        {
            imageProcessor->result.set_mission(2);
            outputPacket_vector2D end;
            end.set_x(ui->sMendX_lineEdit->text().toFloat());
            end.set_y(ui->sMendY_lineEdit->text().toFloat());
            outputPacket_Mission2 mission2;
            mission2.set_isvalid(true);
            mission2.mutable_end()->CopyFrom(end);
            //sendingSocket->sendData("Mission2");
        }
        else if(ui->thirsM_rButton->isChecked())
        {
            imageProcessor->result.set_mission(3);
            outputPacket_Mission3 misssion3;
            misssion3.set_isvalid(true);

            if(ui->attacker_rButton->isChecked())
                misssion3.set_isattacker(true);
            else if(ui->defender_rButton->isChecked())
                misssion3.set_isattacker(false);

            outputPacket_circle2D circle;
            //circle.set

            sendingSocket->sendData("Mission3");
        }
        else
        {
            qDebug()<<"Error : Select a Mission!";
        }
    }
    else
    {
        qDebug()<<"Error : First Turn On Camera!";
    }

    semaphore->release(1);
}
