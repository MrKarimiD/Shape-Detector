#ifndef CAMERASETTING_H
#define CAMERASETTING_H

class cameraSetting
{
public:
    cameraSetting();

    void set_fps(int input);
    void set_WHITE_BALANCE_BLUE_U(int input);
    void set_WHITE_BALANCE_RED_V(int input);
    void set_EXPOSURE(int input);
    void set_BRIGHTNESS(int input);
    void set_SHARPNESS(int input);
    void set_GAIN(int input);
    void set_HUE(int input);
    void set_SATURATION(int input);
    void set_CONTRAST(int input);

    int get_fps();
    int get_WHITE_BALANCE_BLUE_U();
    int get_WHITE_BALANCE_RED_V();
    int get_EXPOSURE();
    int get_BRIGHTNESS();
    int get_SHARPNESS();
    int get_GAIN();
    int get_HUE();
    int get_SATURATION();
    int get_CONTRAST();

private:
    int fps;
    int WHITE_BALANCE_BLUE_U;
    int WHITE_BALANCE_RED_V;
    int EXPOSURE;
    int BRIGHTNESS;
    int SHARPNESS;
    int GAIN;
    int HUE;
    int SATURATION;
    int CONTRAST;
};

#endif // CAMERASETTING_H
