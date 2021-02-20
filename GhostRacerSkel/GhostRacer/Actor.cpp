#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
#include "GameConstants.h"

// Actor MEMBER FUNCTION IMPLEMENTATIONS

//Constructor has GraphObject constructor construct its data members, then constructs Actor's data members
Actor::Actor(int imageID, double startX, double startY,
             int startDirection, double size, int depth,
             int vertSpeed, int horizSpeed, bool collisionAW, StudentWorld* sw)
: GraphObject(imageID, startX, startY, startDirection, size, depth), m_vertSpeed(vertSpeed), m_horizSpeed(horizSpeed), m_collisionAW(collisionAW), m_sw(sw)
{
    m_alive = true;
}

//Accessor
bool Actor::isAlive() const {
    return m_alive;
}

//Mutator
void Actor::changeAlive(bool isAlive){
    m_alive = isAlive;
}

//Accessor
int Actor::getVertSpeed() const {
    return m_vertSpeed;
}

//Mutator
void Actor::changeVertSpeed(int vSpeedChange){
    m_vertSpeed += vSpeedChange;
}

//Accessor
int Actor::getHorizSpeed() const {
    return m_horizSpeed;
}

//Mutator
void Actor::changeHorizSpeed(int hSpeedChange){
    m_horizSpeed += hSpeedChange;
}

//Accessor
bool Actor::isCollisionAW() const {
    return m_collisionAW;
}

// GhostRacer MEMBER FUNCTION IMPLEMENTATIONS

//Constructor
GhostRacer::GhostRacer(int imageID, double startX, double startY,
                       int startDirection, double size, int depth,
                       int vertSpeed, int horizSpeed, bool collisionAW, StudentWorld* sw,
                       int holyWaterAmount, int hitPoints)
: Actor(imageID, startX, startY, startDirection, size, depth, vertSpeed, horizSpeed, collisionAW, sw), m_holyWaterAmount(holyWaterAmount), m_hitPoints(hitPoints)
{ }

void GhostRacer::doSomething(){
    //TODO: FILL IN!
    
}

//Accessor
int GhostRacer::getHolyWaterAmount() const{
    return m_holyWaterAmount;
}

//Mutator
void GhostRacer::changeHolyWaterAmount(int waterChange){
    m_holyWaterAmount += waterChange;
}

//Accessor
int GhostRacer::getHitPoints() const{
    return m_hitPoints;
}

//Mutator
void GhostRacer::changeHitPoints(int hpChange){
    m_hitPoints += hpChange;
}

// BorderLine MEMBER FUNCTION IMPLEMENTATIONS

BorderLine::BorderLine(int imageID, double startX, double startY,
                       int startDirection, double size, int depth,
                       int vertSpeed, int horizSpeed, bool collisionAW, StudentWorld* sw)
: Actor(imageID, startX, startY, startDirection, size, depth, vertSpeed, horizSpeed, collisionAW, sw)
{ }

void BorderLine::doSomething(){
    //TODO: IMPLEMENT!
}




