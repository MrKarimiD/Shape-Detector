#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->add_label->setDisabled(true);
    ui->address_button->setDisabled(true);
    ui->address_lineEdit->setDisabled(true);
    ui->cam_label->setDisabled(true);
    ui->cam_spinBox->setDisabled(true);
    ui->open_button->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_image_rButton_toggled(bool checked)
{
    if(ui->image_rButton->isChecked())
    {
        ui->add_label->setEnabled(true);
        ui->address_button->setEnabled(true);
        ui->address_lineEdit->setEnabled(true);

    }
    else
    {
        ui->add_label->setDisabled(true);
        ui->address_button->setDisabled(true);
        ui->address_lineEdit->setDisabled(true);
    }
}

void MainWindow::on_address_button_clicked()
{
    QString fileAddress = QFileDialog::getOpenFileName(this,tr("Select Your Source Image"), "/home", tr("Image Files (*.png *.jpg *.bmp)"));
    ui->address_lineEdit->setText(fileAddress);
    Mat inputFrame=imread(fileAddress.toStdString());
    Mat outputFrame=imageProcessor->shapeDetection(inputFrame);
    cv::resize(outputFrame,outputFrame,Size(640,480),0,0,INTER_CUBIC);
    QImage imgIn= QImage((uchar*) outputFrame.data, outputFrame.cols, outputFrame.rows, outputFrame.step, QImage::Format_RGB888);

    ui->outputLabel->setPixmap(QPixmap::fromImage(imgIn));
}

void MainWindow::on_camera_rButton_toggled(bool checked)
{
    if(ui->camera_rButton->isChecked())
    {
        ui->cam_label->setEnabled(true);
        ui->cam_spinBox->setEnabled(true);
        ui->open_button->setEnabled(true);
    }
    else
    {
        ui->cam_label->setDisabled(true);
        ui->cam_spinBox->setDisabled(true);
        ui->open_button->setDisabled(true);
    }
}

void MainWindow::on_open_button_clicked()
{
        Mat frame;
        VideoCapture cap;
        cap.open(CAP_FIREWIRE+1);
        cap >> frame;
//        while(true)
//        {
//            cap >> frame;
//            cv::resize(frame,frame,Size(640,480),0,0,INTER_CUBIC);
//            QImage imgIn= QImage((uchar*) frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

//            ui->outputLabel->setPixmap(QPixmap::fromImage(imgIn));
//            if(waitKey(1) > 0) break;
//        }

        Mat outputFrame=imageProcessor->shapeDetection(frame);
        cv::resize(outputFrame,outputFrame,Size(640,480),0,0,INTER_CUBIC);
        QImage imgIn= QImage((uchar*) outputFrame.data, outputFrame.cols, outputFrame.rows, outputFrame.step, QImage::Format_RGB888);

        ui->outputLabel->setPixmap(QPixmap::fromImage(imgIn));
}
