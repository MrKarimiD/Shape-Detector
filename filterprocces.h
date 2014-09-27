#ifndef FILTERPROCCES_H
#define FILTERPROCCES_H

#include <QObject>
#include "imageprocfunction.h"

class filterProcces : public ImageProcFunction
{
    Q_OBJECT
public:
    explicit filterProcces(QObject *parent = 0);
    void process();
    void updateFilterSetting(filterSettings *fs);
private:
    MatArray applyFilters(Mat input);

    filterSettings *fs;
    QSemaphore *semaphoreForUpdateFilterSetting;

signals:

public slots:

};

#endif // FILTERPROCCES_H
