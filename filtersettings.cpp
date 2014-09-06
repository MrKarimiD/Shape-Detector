#include "filtersettings.h"

filterSettings::filterSettings(QObject *parent) :
    QObject(parent)
{
    useMedianBlur=true;
    kernelSize=5;
    useAdaptiveThresh=true;
    blockSize=41;
    C=5;
    useThreshold=false;
    threshValue=50;
    useDilate=true;
    dilateSize=3;
    useCanny=true;
    firstThresh=0;
    secondThresh=50;
    apertureSize=5;
}

void filterSettings::setUseUndisort(bool input)
{
    this->useUndisort = input;
}

void filterSettings::setUseCrop(bool input)
{
    this->useCrop = input;
}

void filterSettings::setCrop_firstX(int input)
{
    this->crop_firstX = input;
}

void filterSettings::setCrop_firstY(int input)
{
    this->crop_firstY = input;
}

void filterSettings::setCrop_secondX(int input)
{
    this->crop_secondX = input;
}

void filterSettings::setCrop_secondY(int input)
{
    this->crop_secondY = input;
}

void filterSettings::setUseMedianBlur(bool input)
{
    this->useMedianBlur=input;
}

void filterSettings::setKernelSize(int input)
{
    if(input%2==1)
        this->kernelSize=input;
    else
        this->kernelSize=input-1;
}

void filterSettings::setUseAdaptiveThresh(bool input)
{
    this->useAdaptiveThresh=input;
}

void filterSettings::setBlockSize(int input)
{
    if(input%2==1)
        this->blockSize=input;
    else
        this->blockSize=input-1;
}

void filterSettings::setC(int input)
{
    if(input%2==1)
        this->C=input;
    else
        this->C=input-1;
}

void filterSettings::setUseThreshold(bool input)
{
    this->useThreshold=input;
}

void filterSettings::setThreshValue(int input)
{
    this->threshValue=input;
}

void filterSettings::setUseDilate(bool input)
{
    this->useDilate=input;
}

void filterSettings::setDilateSize(int input)
{
    if(input%2==1)
        this->dilateSize=input;
    else
        this->dilateSize=input-1;
}

void filterSettings::setUseCanny(bool input)
{
    this->useCanny=input;
}

void filterSettings::setFirstThresh(int input)
{
    this->firstThresh=input;
}

void filterSettings::setSecondThresh(int input)
{
    this->secondThresh=input;
}

void filterSettings::setApertureSize(int input)
{
    if(input%2==1)
        this->apertureSize=input;
    else
        this->apertureSize=input-1;
}

bool filterSettings::getUseUndisort()
{
    return this->useUndisort;
}

bool filterSettings::getUseCrop()
{
    return this->useCrop;
}

int filterSettings::getCropFirstX()
{
    return this->crop_firstX;
}

int filterSettings::getCropFirstY()
{
    return this->crop_firstY;
}

int filterSettings::getCropSecondX()
{
    return this->crop_secondX;
}

int filterSettings::getCropSecondY()
{
    return this->crop_secondY;
}

bool filterSettings::getUseMedianBlur()
{
    return this->useMedianBlur;
}

int filterSettings::getKernelSize()
{
    return this->kernelSize;
}

bool filterSettings::getUseAdaptiveThresh()
{
    return this->useAdaptiveThresh;
}

int filterSettings::getBlockSize()
{
    return this->blockSize;
}

int filterSettings::getC()
{
    return this->C;
}

bool filterSettings::getUseThreshold()
{
    return this->useThreshold;
}

int filterSettings::getThreshValue()
{
    return this->threshValue;
}

bool filterSettings::getUseDilate()
{
    return this->useDilate;
}

int filterSettings::getDilateSize()
{
    return this->dilateSize;
}

bool filterSettings::getUseCanny()
{
    return this->useCanny;
}

int filterSettings::getFirstThresh()
{
    return this->firstThresh;
}

int filterSettings::getSecondThresh()
{
    return this->secondThresh;
}

int filterSettings::getApertureSize()
{
    return this->apertureSize;
}
