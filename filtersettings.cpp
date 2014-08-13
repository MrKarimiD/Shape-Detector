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

void filterSettings::setUseMedianBlur(bool input)
{
    this->useMedianBlur=input;
}

void filterSettings::setKernelSize(int input)
{
    this->kernelSize=input;
}

void filterSettings::setUseAdaptiveThresh(bool input)
{
    this->useAdaptiveThresh=input;
}

void filterSettings::setBlockSize(int input)
{
    this->blockSize=input;
}

void filterSettings::setC(int input)
{
    this->C=input;
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
    this->dilateSize=input;
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
    this->apertureSize=input;
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
