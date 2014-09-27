#include "filterprocces.h"

filterProcces::filterProcces(QObject *parent) :
    ImageProcFunction(parent)
{
    fs = new filterSettings();
    semaphoreForUpdateFilterSetting = new QSemaphore(1);
}

void filterProcces::process()
{
    disconnect(timer,SIGNAL(timeout()),this,SLOT(timer_interval()));

    semaphore->tryAcquire(1,30);
    Mat in ;
    input.crop.copyTo(in);
    //this->input.release();
    this->newUnreadData = false;
    semaphore->release(1);

    MatArray output = applyFilters(in);

    connect(timer,SIGNAL(timeout()),this,SLOT(timer_interval()));

    emit done(output);
}

void filterProcces::updateFilterSetting(filterSettings *fs)
{
    semaphoreForUpdateFilterSetting->acquire(1);
    this->fs = fs;
    semaphoreForUpdateFilterSetting->release(1);
}

MatArray filterProcces::applyFilters(Mat input)
{
    filterSettings *filterSet = new filterSettings();

    semaphore->tryAcquire(1,30);
    MatArray output = this->input;
    semaphore->release(1);

    semaphoreForUpdateFilterSetting->tryAcquire(1,30);
    filterSet = fs;
    semaphoreForUpdateFilterSetting->release(1);

    Mat src = input.clone();

    // Convert to grayscale
    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    if(filterSet->getUseMedianBlur())
    {
        medianBlur(gray,gray,filterSet->getKernelSize());
    }

    Mat threshold_mat = Mat::zeros(gray.cols,gray.rows,CV_8UC1);
    if(filterSet->getUseAdaptiveThresh())
    {
        adaptiveThreshold(gray,threshold_mat,255,ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY_INV,
                          filterSet->getBlockSize(),
                          filterSet->getC());
        threshold_mat.copyTo(output.addaptiveThreshold);
    }
    else
    {
        if(filterSet->getUseThreshold())
        {
            threshold( gray, threshold_mat,filterSet->getThreshValue(),255,THRESH_BINARY_INV);
            threshold_mat.copyTo(output.threshold);
        }
        else
        {
            threshold_mat=gray.clone();
        }
    }

    if(filterSet->getUseDilate())
    {
        Mat structure=getStructuringElement(MORPH_RECT,Size(filterSet->getDilateSize(),filterSet->getDilateSize()));
        dilate(threshold_mat,threshold_mat,structure);
    }

    // Use Canny instead of threshold to catch squares with gradient shading

    Mat bw;
    if(filterSet->getUseCanny())
    {
        Canny(threshold_mat, bw,filterSet->getFirstThresh(),filterSet->getSecondThresh()
              , filterSet->getApertureSize());
        bw.copyTo(output.canny);
    }
    else
    {
        bw=threshold_mat.clone();
    }

    return output;
}
