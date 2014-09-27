#include "showoutputsetting.h"

showOutputSetting::showOutputSetting()
{
    drawContoursBool = false;
    drawGeometricLabels = false;
    drawBoundedRect = false;
    drawRotatedRect = false;
    drawBoundries = false;
    drawGameGround = false;
    drawColors = false;
}

void showOutputSetting::set_drawContoursBool(bool input)
{
    this->drawContoursBool = input;
}

void showOutputSetting::set_drawGeometricLabels(bool input)
{
    this->drawGeometricLabels = input;
}

void showOutputSetting::set_drawBoundedRect(bool input)
{
    this->drawBoundedRect = input;
}

void showOutputSetting::set_drawRotatedRect(bool input)
{
    this->drawRotatedRect = input;
}

void showOutputSetting::set_drawBoundries(bool input)
{
    this->drawBoundries = input;
}

void showOutputSetting::set_drawGameGround(bool input)
{
    this->drawGameGround = input;
}

void showOutputSetting::set_drawColors(bool input)
{
    this->drawColors = input;
}

bool showOutputSetting::get_drawContoursBool()
{
    return this->drawContoursBool;
}

bool showOutputSetting::get_drawGeometricLabels()
{
    return this->drawGeometricLabels;
}

bool showOutputSetting::get_drawBoundedRect()
{
    return this->drawBoundedRect;
}

bool showOutputSetting::get_drawRotatedRect()
{
    return this->drawRotatedRect;
}

bool showOutputSetting::get_drawBoundries()
{
    return this->drawBoundries;
}

bool showOutputSetting::get_drawGameGround()
{
    return this->drawGameGround;
}

bool showOutputSetting::get_drawColors()
{
    return this->drawColors;
}
