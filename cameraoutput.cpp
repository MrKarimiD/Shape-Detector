#include "cameraoutput.h"

CameraOutput::CameraOutput(QObject *parent) :
    QObject(parent)
{
}

void CameraOutput::addShape(Vector2D pos, double r, QString color, QString type)
{
    Shape *temp=new Shape();
    temp->set(pos,r,color,type);
    shapes.insert(nummberOfShapes,temp);
    nummberOfShapes++;
}

void CameraOutput::clearShapeList()
{
    shapes.clear();
    nummberOfShapes=0;
}
