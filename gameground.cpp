#include "gameground.h"

GameGround::GameGround(QObject *parent) :
    QObject(parent)
{
}

void GameGround::setMission(int mission)
{
    this->mission=mission;
}

void GameGround::setEndPoint(Vector2D input)
{
    this->endPoint=input;
}

void GameGround::setFirstRegion(Rect2D input)
{
    this->region[0]=input;
}

void GameGround::setSecondRegion(Rect2D input)
{
    this->region[1]=input;
}

void GameGround::setRole(bool isAttacker)
{
    this->isAttacker=isAttacker;
}

int GameGround::getMission()
{
    return this->mission;
}

Vector2D GameGround::getEndPoint()
{
    return this->endPoint;
}

Rect2D GameGround::getRegion1()
{
    return this->region[0];
}

Rect2D GameGround::getRegion2()
{
    return this->region[1];
}

bool GameGround::getRole()
{
    return this->isAttacker;
}
