#ifndef SHOWOUTPUTSETTING_H
#define SHOWOUTPUTSETTING_H

class showOutputSetting
{
public:
    showOutputSetting();

    void set_drawContoursBool(bool input);
    void set_drawGeometricLabels(bool input);
    void set_drawBoundedRect(bool input);
    void set_drawRotatedRect(bool input);
    void set_drawBoundries(bool input);
    void set_drawGameGround(bool input);
    void set_drawColors(bool input);

    bool get_drawContoursBool();
    bool get_drawGeometricLabels();
    bool get_drawBoundedRect();
    bool get_drawRotatedRect();
    bool get_drawBoundries();
    bool get_drawGameGround();
    bool get_drawColors();

private:
    bool drawContoursBool;
    bool drawGeometricLabels;
    bool drawBoundedRect;
    bool drawRotatedRect;
    bool drawBoundries;
    bool drawGameGround;
    bool drawColors;
};

#endif // SHOWOUTPUTSETTING_H
