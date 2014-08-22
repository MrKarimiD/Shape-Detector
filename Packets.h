#ifndef PACKETS_H
#define PACKETS_H

#include "geom/geom.h"
#include "shape.h"

union sendingPacket
{
    struct GameData
    {
        int mission;
        Rect2D region[2];
        Vector2D endPoint;
        bool isAttacker;
        QList<Shape> shapes;
        Vector2D ourPosition;
        Vector2D *oppPosition=NULL;
        int nummberOfShapes;
        QList<Segment2D> midleLine;
    };
    QByteArray *bytes;
};
#endif // PACKETS_H
