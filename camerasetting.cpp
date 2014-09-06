#include "camerasetting.h"

cameraSetting::cameraSetting()
{
}

void cameraSetting::set_fps(int input)
{
    this->fps = input;
}

void cameraSetting::set_WHITE_BALANCE_BLUE_U(int input)
{
    this->WHITE_BALANCE_BLUE_U = input;
}

void cameraSetting::set_WHITE_BALANCE_RED_V(int input)
{
    this->WHITE_BALANCE_RED_V = input;
}

void cameraSetting::set_EXPOSURE(int input)
{
    this->EXPOSURE = input;
}

void cameraSetting::set_BRIGHTNESS(int input)
{
    this->BRIGHTNESS = input;
}

void cameraSetting::set_SHARPNESS(int input)
{
    this->SHARPNESS = input;
}

void cameraSetting::set_GAIN(int input)
{
    this->GAIN = input;
}

int cameraSetting::get_fps()
{
    return this->fps;
}

int cameraSetting::get_WHITE_BALANCE_BLUE_U()
{
    return this->WHITE_BALANCE_BLUE_U;
}

int cameraSetting::get_WHITE_BALANCE_RED_V()
{
    return this->WHITE_BALANCE_RED_V;
}

int cameraSetting::get_EXPOSURE()
{
    return this->EXPOSURE;
}

int cameraSetting::get_BRIGHTNESS()
{
    return this->BRIGHTNESS;
}

int cameraSetting::get_SHARPNESS()
{
    return this->SHARPNESS;
}

int cameraSetting::get_GAIN()
{
    return this->GAIN;
}
