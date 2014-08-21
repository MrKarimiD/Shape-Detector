#ifndef CAMERAOUTPUT_H
#define CAMERAOUTPUT_H

#include <QObject>
#include "geom/geom.h"
#include "shape.h"
#include "QDebug"

class CameraOutput : public QObject
{
    Q_OBJECT
public:
    explicit CameraOutput(QObject *parent = 0);
    void addShape(Vector2D pos,double r,QString color,QString type);
    void clearShapeList();

private:
    QList<Shape*> shapes;
    Vector2D ourPosition;
    Vector2D *oppPosition=NULL;
    int nummberOfShapes;
    QList<Segment2D> midleLine;

signals:

public slots:

};

#endif // CAMERAOUTPUT_H
