#include "shape.h"

Shape::Shape()
{

}

void Shape::set(Vector2D position, double roundedRadios, std::string color, std::string type)
{
    this->position=position;
    this->roundedRadios=roundedRadios;
    this->color=color;
    this->type=type;

}
