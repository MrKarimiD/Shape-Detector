#ifndef GAMEGROUND_H
#define GAMEGROUND_H

#include <QObject>
#include "cameraoutput.h"
#include "Prototype_Messages/GameGround.pb.h"

class GameGround : public QObject
{
    Q_OBJECT
public:
    explicit GameGround(QObject *parent = 0);
    void setMission(int mission);
    void setEndPoint(Vector2D input);
    void setFirstRegion(Rect2D input);
    void setSecondRegion(Rect2D input);
    void setRole(bool isAttacker);

    int getMission();
    Vector2D getEndPoint();
    Rect2D getRegion1();
    Rect2D getRegion2();
    bool getRole();

    void addEveryThingToPackets();

    CameraOutput output;

private:
    int mission;
    Rect2D region[2];
    Vector2D endPoint;
    bool isAttacker;
    outputPacket dataGram;
signals:

public slots:

};

#endif // GAMEGROUND_H
