#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    disableOpenImage();

    disableOpenCamera();

    QStringList items;
    items<<"1"<<"0";
    ui->cam_comboBox->addItems(items);

    cam_timer=new QTimer();

    connect(this,SIGNAL(imageReady(Mat)),this,SLOT(callImageProcessingFunctions(Mat)));

    imageProcessor=new ImageProcessing();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_image_rButton_toggled(bool checked)
{
    if(ui->image_rButton->isChecked())
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
    if(ui->camera_rButton->isChecked())
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
        Mat frame;

        if(ui->cam_comboBox->currentText()=="0")
            cap.open(CAP_FIREWIRE+0);
        else
            cap.open(CAP_FIREWIRE+1);

        cap.set(CAP_PROP_FPS, 15);
        cap.set(CAP_PROP_WHITE_BALANCE_BLUE_U,ui->blue_slider->value());
        cap.set(CAP_PROP_WHITE_BALANCE_RED_V,ui->red_slider->value());
//        cap.set(CAP_PROP_BRIGHTNESS,1000);
       cap.set(CAP_PROP_EXPOSURE,ui->exposure_slider->value());
        //cap.set(CAP_PROP_SHARPNESS,1000);
        //cap.set(CAP_PROP_GAIN,100);

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
    ui->red_label->setEnabled(true);
    ui->blue_label->setEnabled(true);
    ui->expo_label->setEnabled(true);
}

void MainWindow::disableCameraSetting()
{
    ui->red_slider->setDisabled(true);
    ui->blue_slider->setDisabled(true);
    ui->exposure_slider->setDisabled(true);
    ui->red_label->setDisabled(true);
    ui->blue_label->setDisabled(true);
    ui->expo_label->setDisabled(true);
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
                                        ,ui->bound_checkBox->isChecked(),ui->rotate_checkBox->isChecked());
}

void MainWindow::callImageProcessingFunctions(Mat input_mat)
{
    Mat inputFrame;
    if(ui->camera_rButton->isChecked())
    {
        imageProcessor->applyFilters(input_mat).copyTo(inputFrame);
    }
    else
    {
        input_mat.copyTo(inputFrame);
    }
    Mat outputFrame;
    imageProcessor->shapeDetection(inputFrame).copyTo(outputFrame);
    cv::resize(outputFrame,outputFrame,Size(640,480),0,0,INTER_CUBIC);
    cvtColor(outputFrame, outputFrame, COLOR_BGR2RGB);
    QImage imgIn= QImage((uchar*) outputFrame.data, outputFrame.cols, outputFrame.rows, outputFrame.step, QImage::Format_RGB888);
    ui->outputLabel->setPixmap(QPixmap::fromImage(imgIn));
}

void MainWindow::on_camSet_checkBox_stateChanged(int arg1)
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

void MainWindow::on_xml_checkBox_stateChanged(int arg1)
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

void MainWindow::on_cont_checkBox_stateChanged(int arg1)
{
    updateOutputOptions();
}

void MainWindow::on_geom_checkBox_stateChanged(int arg1)
{
    updateOutputOptions();
}

void MainWindow::on_bound_checkBox_stateChanged(int arg1)
{
    updateOutputOptions();
}

void MainWindow::on_rotate_checkBox_stateChanged(int arg1)
{
    updateOutputOptions();
}
