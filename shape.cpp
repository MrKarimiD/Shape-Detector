#include "shape.h"

Shape::Shape(QObject *parent) :
    QObject(parent)
{
}

void Shape::set(Vector2D position, double roundedRadios, QString color, QString type)
{
    this->position=position;
    this->roundedRadios=roundedRadios;
    this->color=color;
    this->type=type;
}
