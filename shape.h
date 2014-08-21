#ifndef SHAPE_H
#define SHAPE_H

#include <QObject>
#include "geom/geom.h"
#include "opencv2/imgproc/imgproc.hpp"

class Shape : public QObject
{
    Q_OBJECT
public:
    explicit Shape(QObject *parent = 0);
    void set(Vector2D position,double roundedRadios,QString color,QString type);
private:
    Vector2D position;
    double roundedRadios;
    QString color;
    QString type;

signals:

public slots:

};

#endif // SHAPE_H
