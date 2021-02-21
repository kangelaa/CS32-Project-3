#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
#include "GameConstants.h"
#define _USE_MATH_DEFINES
#include <math.h>

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

//Accessor
StudentWorld* Actor::getStudentWorld() const {
    return m_sw;
}

// GhostRacer MEMBER FUNCTION IMPLEMENTATIONS

//Constructor
GhostRacer::GhostRacer(int imageID, double startX, double startY,
                       int startDirection, double size, int depth,
                       int vertSpeed, int horizSpeed, bool collisionAW, StudentWorld* sw,
                       int holyWaterAmount, int hitPoints)
: Actor(imageID, startX, startY, startDirection, size, depth, vertSpeed, horizSpeed, collisionAW, sw), m_holyWaterAmount(holyWaterAmount), m_hitPoints(hitPoints)
{ }

//has GhostRacer read in user input (key press) and update direction/x/y accordingly
void GhostRacer::doSomething(){
    //TODO: FINISH UP for pt 2!
    
    //if GhostRacer isn't alive, return immediately
    if (getHitPoints() <= 0){    //TODO: CHECK this is called on the right GhostRacer object right? (this->)?
        return;
    }
    
    //getting user input (as key presses)
    int ch;
    if (getStudentWorld()->getKey(ch)){
        //user hit a key during this tick!
        switch (ch){
            case KEY_PRESS_LEFT:
                if (getDirection() < 114){
                    setDirection(getDirection() + 8);
                }
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() > 66){
                    setDirection(getDirection() - 8);
                }
                break;
            case KEY_PRESS_UP:
                if (getVertSpeed() < 5){
                    changeVertSpeed(1);
                }
                break;
            case KEY_PRESS_DOWN:
                if (getVertSpeed() > -1){
                    changeVertSpeed(-1);
                }
                break;
        }
    }
    
    //move GhostRacer based on specified movement algorithm
    double max_shift_per_tick = 4.0;
    int direction = getDirection();
    double delta_x = cos((direction*M_PI)/180) * max_shift_per_tick;    //convert direction from degrees to rad to use as argument for cos()
    double cur_x = getX();
    double cur_y = getY();
    moveTo(cur_x+delta_x, cur_y);   //update x and y location for GhostRacer w/ moveTo()
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

//Constructor
BorderLine::BorderLine(int imageID, double startX, double startY,
                       int startDirection, double size, int depth,
                       int vertSpeed, int horizSpeed, bool collisionAW, StudentWorld* sw)
: Actor(imageID, startX, startY, startDirection, size, depth, vertSpeed, horizSpeed, collisionAW, sw)
{ }

//has BorderLine lines move down the screen
void BorderLine::doSomething(){
    //TODO: IMPLEMENT!
    
    //move BorderLine using algorithm
    int vert_speed = getVertSpeed() - (getStudentWorld()->getPointerToGhostRacer()->getVertSpeed());    //TODO: CHECK IS THIS CORRECT?
    int horiz_speed = getHorizSpeed();
    double new_y = getY() + vert_speed;
    double new_x = getX() + horiz_speed;
    moveTo(new_x, new_y);   //update x and y location for BorderLine w/ moveTo()
    
    //if BorderLine leaves the screen, set its status to not-alive, so it will be removed by StudentWorld later in this tick
    if (getX() < 0 || getX() > VIEW_WIDTH || getY() < 0 || getY() > VIEW_HEIGHT){
        changeAlive(false);
        return;
    }
}



