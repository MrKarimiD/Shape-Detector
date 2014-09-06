#ifndef FILTERSETTINGS_H
#define FILTERSETTINGS_H

#include <QObject>

class filterSettings : public QObject
{
    Q_OBJECT
public:
    explicit filterSettings(QObject *parent = 0);

    void setUseUndisort(bool input);
    void setUseCrop(bool input);
    void setCrop_firstX(int input);
    void setCrop_firstY(int input);
    void setCrop_secondX(int input);
    void setCrop_secondY(int input);
    void setUseMedianBlur(bool input);
    void setKernelSize(int input);
    void setUseAdaptiveThresh(bool input);
    void setBlockSize(int input);
    void setC(int input);
    void setUseThreshold(bool input);
    void setThreshValue(int input);
    void setUseDilate(bool input);
    void setDilateSize(int input);
    void setUseCanny(bool input);
    void setFirstThresh(int input);
    void setSecondThresh(int input);
    void setApertureSize(int input);

    bool getUseUndisort();
    bool getUseCrop();
    int getCropFirstX();
    int getCropFirstY();
    int getCropSecondX();
    int getCropSecondY();
    bool getUseMedianBlur();
    int getKernelSize();
    bool getUseAdaptiveThresh();
    int getBlockSize();
    int getC();
    bool getUseThreshold();
    int getThreshValue();
    bool getUseDilate();
    int getDilateSize();
    bool getUseCanny();
    int getFirstThresh();
    int getSecondThresh();
    int getApertureSize();

signals:

public slots:

private:
    bool useUndisort;
    bool useCrop;
    int crop_firstX;
    int crop_firstY;
    int crop_secondX;
    int crop_secondY;
    bool useMedianBlur;
    int kernelSize;
    bool useAdaptiveThresh;
    int blockSize;
    int C;
    bool useThreshold;
    int threshValue;
    bool useDilate;
    int dilateSize;
    bool useCanny;
    int firstThresh;
    int secondThresh;
    int apertureSize;
};

#endif // FILTERSETTINGS_H
