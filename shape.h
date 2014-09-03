#ifndef SHAPE_H
#define SHAPE_H

#include "geom.h"

class Shape
{
public:
    explicit Shape();
    void set(Vector2D position,double roundedRadios,std::string color,std::string type);
    Vector2D position;
    double roundedRadios;
    std::string color;
    std::string type;
};

#endif // SHAPE_H
