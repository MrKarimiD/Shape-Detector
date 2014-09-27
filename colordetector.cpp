#include "colordetector.h"

ColorDetector::ColorDetector(QObject *parent) :
    QObject(parent)
{
}

QString ColorDetector::findColor(vector<Mat> hsvPlanes)
{
    QString color = "not found";

    Mat thresh;

    Mat src = hsvPlanes[0];

    for(int j=0;j<red_samples.size();j++)
    {
        inRange(src, Scalar(red_samples.at(j).val[0]), Scalar(red_samples.at(j).val[1]), thresh);
        int numberOfWhitePixels = countNonZero(thresh);
        double percent = numberOfWhitePixels/(src.rows*src.cols)*100;
        //qDebug()<<"percent:"<<QString::number(percent);
        if( percent > ColorPercentage)
        {
            color = "red";
            break;
        }
    }

    if(color == "not found")
    {
        for(int j=0;j<blue_samples.size();j++)
        {
            inRange(src, Scalar(blue_samples.at(j).val[0]), Scalar(blue_samples.at(j).val[1]), thresh);
            int numberOfWhitePixels = countNonZero(thresh);
            if( (numberOfWhitePixels*100/(src.rows*src.cols)) > ColorPercentage)
            {
                color = "blue";
                break;
            }
        }
    }

    if(color == "not found")
    {
        for(int j=0;j<green_samples.size();j++)
        {
            inRange(src, Scalar(green_samples.at(j).val[0]), Scalar(green_samples.at(j).val[1]), thresh);
            int numberOfWhitePixels = countNonZero(thresh);
            if( (numberOfWhitePixels*100/(src.rows*src.cols)) > ColorPercentage)
            {
                color = "green";
                break;
            }
        }
    }

    if(color == "not found")
    {
        for(int j=0;j<yellow_samples.size();j++)
        {
            inRange(src, Scalar(yellow_samples.at(j).val[0]), Scalar(yellow_samples.at(j).val[1]), thresh);
            int numberOfWhitePixels = countNonZero(thresh);
            if( (numberOfWhitePixels*100/(src.rows*src.cols)) > ColorPercentage)
            {
                color = "yellow";
                break;
            }
        }
    }

    if(color == "not found")
    {
        for(int j=0;j<cyan_samples.size();j++)
        {
            inRange(src, Scalar(cyan_samples.at(j).val[0]), Scalar(cyan_samples.at(j).val[1]), thresh);
            int numberOfWhitePixels = countNonZero(thresh);
            if( (numberOfWhitePixels*100/(src.rows*src.cols)) > ColorPercentage)
            {
                color = "cyan";
                break;
            }
        }
    }

    if(color == "not found")
    {
        for(int j=0;j<violet_samples.size();j++)
        {
            inRange(src, Scalar(violet_samples.at(j).val[0]), Scalar(violet_samples.at(j).val[1]), thresh);
            int numberOfWhitePixels = countNonZero(thresh);
            if( (numberOfWhitePixels*100/(src.rows*src.cols)) > ColorPercentage)
            {
                color = "violet";
                break;
            }
        }
    }

    if(color == "not found")
    {
        for(int j=0;j<black_samples.size();j++)
        {
            inRange(src, Scalar(black_samples.at(j).val[0]), Scalar(black_samples.at(j).val[1]), thresh);
            int numberOfWhitePixels = countNonZero(thresh);
            if( (numberOfWhitePixels*100/(src.rows*src.cols)) > ColorPercentage)
            {
                color = "black";
                break;
            }
        }
    }

    return color;
}

Vec2f ColorDetector::returnColor(Mat input, Mat src)
{
    Mat HSV,show;
    cvtColor(input,HSV,COLOR_RGB2HSV);
    cvtColor(src,show,COLOR_RGB2HSV);

    vector<cv::Mat> hsvPlanes;
    vector<cv::Mat> hsvPlanes_show;
    split(HSV, hsvPlanes);
    split(show, hsvPlanes_show);

    // compute statistics for Hue value
    Scalar means, stddev;
    meanStdDev(hsvPlanes[0], means, stddev);

    // ensure we get 95% of all valid Hue samples (statistics 3*sigma rule)
    float minHue = means[0] - stddev[0]*3;
    float maxHue = means[0] + stddev[0]*3;

    Scalar means1, stddev1;
    meanStdDev(hsvPlanes[1], means1, stddev1);

    // ensure we get 95% of all valid Hue samples (statistics 3*sigma rule)
    float minSat = means1[0] - stddev1[0]*3;
    float maxSat = means1[0] + stddev1[0]*3;

    Scalar means2, stddev2;
    meanStdDev(hsvPlanes[2], means2, stddev2);

    // ensure we get 95% of all valid Hue samples (statistics 3*sigma rule)
    float minVal = means2[0] - stddev2[0]*3;
    float maxVal = means2[0] + stddev2[0]*3;

    qDebug()<<"min:"<<minHue<<","<<minSat<<","<<minVal;
    qDebug()<<"max:"<<maxHue<<","<<maxSat<<","<<maxVal;;


    // STEP 2: detection phase
    Mat imgThreshed;
    inRange(hsvPlanes_show[0], Scalar(minHue,minSat,minVal), Scalar(maxHue,maxSat,maxVal), imgThreshed);
    imshow("thresholded", imgThreshed);

    Vec2f hue;
    hue.val[0]=minHue;
    hue.val[1]=maxHue;

    return hue;
}
