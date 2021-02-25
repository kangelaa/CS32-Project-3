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
             StudentWorld* sw)
: GraphObject(imageID, startX, startY, startDirection, size, depth), m_sw(sw)
{
    m_alive = true;
    m_vertSpeed = 0; //TODO: NECESSARY?
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
void Actor::setVertSpeed(int vSpeed){
    m_vertSpeed = vSpeed;
}

//Accessor
bool Actor::isCollisionAW() const {
    return false;
}

//Mutator //TODO: IMPLEMENT THIS ONE
bool Actor::beSprayedIfAppropriate(){
    return true;
}

//Accessor
StudentWorld* Actor::getStudentWorld() const {
    return m_sw;
}

// Adjust the x coordinate by dx to move to a position with a y coordinate
      // determined by this actor's vertical speed relative to GhostRacser's
      // vertical speed.  Return true if the new position is within the view;
      // otherwise, return false, with the actor dead.
//TODO: IMPLEMENT THIS ONE
bool Actor::moveRelativeToGhostRacerVerticalSpeed(double dx){
    return false;
}

// BorderLine MEMBER FUNCTION IMPLEMENTATIONS

//Constructor
BorderLine::BorderLine(int imageID, double startX, double startY,
                       int startDirection, double size, int depth,
                       int vertSpeed, StudentWorld* sw)
: Actor(imageID, startX, startY, startDirection, size, depth, sw)
{
    setVertSpeed(vertSpeed);
}

//has BorderLine lines move down the screen
void BorderLine::doSomething(){
    //TODO: IMPLEMENT! + FIX THE CODE DUPLICATIONS (move() in actor? add BorderLine to actor?)
    
    //move BorderLine using algorithm
    int vert_speed = getVertSpeed() - (getStudentWorld()->getPointerToGhostRacer()->getVertSpeed());    //TODO: CHECK IS THIS CORRECT?
    int horiz_speed = 0;
    double new_y = getY() + vert_speed;
    double new_x = getX() + horiz_speed;
    moveTo(new_x, new_y);   //update x and y location for BorderLine w/ moveTo()
    
    //if BorderLine leaves the screen, set its status to not-alive, so it will be removed by StudentWorld later in this tick
    if (getX() < 0 || getX() > VIEW_WIDTH || getY() < 0 || getY() > VIEW_HEIGHT){
        changeAlive(false);
        return;
    }
}

// Agent MEMBER FUNCTION IMPLEMENTATIONS

//Constructor
Agent::Agent(int imageID, double startX, double startY,
      int startDirection, double size,
      StudentWorld* sw,
      int hitPoints)
: Actor(imageID, startX, startY, startDirection, size, 0, sw)
{
    m_hitPoints = hitPoints;
}

//Accessor
int Agent::getHitPoints() const{
    return m_hitPoints;
}

//Mutator
void Agent::setHitPoints(int hp){
    m_hitPoints = hp;
}

//Accessor
int Agent::getHorizSpeed() const {
    return m_horizSpeed;
}

//Mutator
void Agent::setHorizSpeed(int hSpeed){
    m_horizSpeed = hSpeed;
}

bool Agent::isCollisionAW() const {
    return true;
}

// Do what the spec says happens when hp units of damage is inflicted.
  // Return true if this agent dies as a result, otherwise false.
bool Agent::takeDamageAndPossiblyDie(int hp){   //hp is the decrement amount
    setHitPoints(getHitPoints() - hp);
    if (getHitPoints() <= 0){
        changeAlive(false);
        getStudentWorld()->playSound(soundWhenDie());
        return true;
    }
    return false;
}

//Accessor
int Agent::soundWhenHurt() const {
    return SOUND_VEHICLE_CRASH;     //sounds for Ghost Racer
}

//Accessor
int Agent::soundWhenDie() const {
    return SOUND_PLAYER_DIE;    //sounds for Ghost Racer
}

// GhostRacer MEMBER FUNCTION IMPLEMENTATIONS

//Constructor
GhostRacer::GhostRacer(int imageID, double startX, double startY,
                       int startDirection, double size, 
                       int vertSpeed, int horizSpeed, StudentWorld* sw,
                       int holyWaterAmount)
: Agent(imageID, startX, startY, startDirection, size, sw, 100), m_holyWaterAmount(holyWaterAmount)
{
    setVertSpeed(vertSpeed);
    setHorizSpeed(horizSpeed);
}

//has GhostRacer read in user input (key press) and update direction/x/y accordingly
void GhostRacer::doSomething(){
    //TODO: FINISH UP for pt 2!
    
    //if GhostRacer isn't alive, return immediately
    if (getHitPoints() <= 0){
        return;
    }
    
    int ch;
    //check for collision w/ BorderLine
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    if (getX() <= LEFT_EDGE){
        if (getDirection() > 90){
            takeDamageAndPossiblyDie(10);
        }
        setDirection(82);
        getStudentWorld()->playSound(SOUND_VEHICLE_CRASH);
    } else if (getX() >= RIGHT_EDGE){
        if (getDirection() < 90){
            takeDamageAndPossiblyDie(10);
        }
        setDirection(98);
        getStudentWorld()->playSound(SOUND_VEHICLE_CRASH); //TODO: SEARCH AND REPLACE ALL SOUND W/ MEMEBR FUNCTIONS
    } else if (getStudentWorld()->getKey(ch)){   //check for user input (key press)
        //user hit a key during this tick!
        switch (ch){
            case KEY_PRESS_SPACE:
                if (getNumSprays() >= 1){
                    //TODO: IMPLEMENT THIS PART pg 30! (a.i.)
                    getStudentWorld()->playSound(SOUND_PLAYER_SPRAY);
                    changeNumSprays(-1);
                }
                break;
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
                    setVertSpeed(getVertSpeed()+1);
                }
                break;
            case KEY_PRESS_DOWN:
                if (getVertSpeed() > -1){
                    setVertSpeed(getVertSpeed()-1);
                }
                break;
        }
    }
    
    //move GhostRacer based on specified movement algorithm
    moveAndPossiblyPickPlan();
}

//Accessor
int GhostRacer::getNumSprays() const{
    return m_holyWaterAmount;
}

//Mutator
void GhostRacer::changeNumSprays(int waterChange){
    m_holyWaterAmount += waterChange;
}

//Mutator, spin as result of hitting oil slick
void GhostRacer::spin(){
    int orientation = randInt(0, 1);  //pick clockwise or counterclockwise
    int angle = randInt(5, 20);
    if (orientation == 0 && getDirection() - angle >= 60){  //if randInt causes angle to change to outside of (60, 120) range, set 60 or 120 as min/max depending on orientation
        setDirection(getDirection() - angle);
    } else if (orientation == 0){
        setDirection(60);
    } else if (orientation == 1 && getDirection() + angle <= 120){
        setDirection(getDirection() + angle);
    } else {
        setDirection(120);
    }
}

//Mutator
void GhostRacer::heal(int hp){  //hp is the increment/decrement (change) amount!
    if (getHitPoints()+hp <= 100){   //hp can't exceed 100
        setHitPoints(getHitPoints()+hp);
    } else {
        setHitPoints(100);
    }
}

//Move GhostRacer based on specified movement algorithm
void GhostRacer::moveAndPossiblyPickPlan(){
    double max_shift_per_tick = 4.0;
    int direction = getDirection();
    double delta_x = cos((direction*M_PI)/180) * max_shift_per_tick;    //convert direction from degrees to rad to use as argument for cos()
    double cur_x = getX();
    double cur_y = getY();
    moveTo(cur_x+delta_x, cur_y);   //update x and y location for GhostRacer w/ moveTo()
}

// IntelligentAgent MEMBER FUNCTION IMPLEMENTATIONS

IntelligentAgent::IntelligentAgent(int imageID, double startX, double startY,
                   int startDirection, double size,
                   StudentWorld* sw,
                   int hitPoints)
: Agent(imageID, startX, startY, startDirection, size, sw, hitPoints) //TODO: ZOMBIE CAB STARTS OFF W/ 3 HP!, other two 2
{
    m_movementPlan = 0;
    setHorizSpeed(0);
}

//Accessor
int IntelligentAgent::getMovementPlan(){
    return m_movementPlan;
}
                    
//Mutator
void IntelligentAgent::setMovementPlan(int plan){
    m_movementPlan = plan;
}

// Move the agent.  If the agent doesn't go off screen and
// should pick a new movement plan, pick a new plan.
void IntelligentAgent::moveAndPossiblyPickPlan(){
    int vert_speed = getVertSpeed() - getStudentWorld()->getPointerToGhostRacer()->getVertSpeed();
    int horiz_speed = getHorizSpeed();
    double new_y = getY() + vert_speed;
    double new_x = getX() + horiz_speed;
    moveTo(new_x, new_y);
    
    //if Agent leaves the screen, set its status to not-alive, so it will be removed by StudentWorld later in this tick
    if (getX() < 0 || getX() > VIEW_WIDTH || getY() < 0 || getY() > VIEW_HEIGHT){
        changeAlive(false);
        return;
    }
    
    //works for human ped/zombie cab, not zombie ped
    if(updateMovementPlan()){
        return;     //return immediately if plan is > 0
    }

    pickNewPlan();  //works for both human/zombie peds, overwritten for zombie cab
}

bool IntelligentAgent::updateMovementPlan(){
    //decrement movement plan distance
    setMovementPlan(getMovementPlan()-1);
    if (getMovementPlan() > 0){
        return true;    //this tells the caller of this function to return immediately if bool = true
    }
    return false;
}

void IntelligentAgent::pickNewPlan(){
    //pick new movement plan for human/zombie peds:
        //horiz speed
    int randHSpeed = randInt(-3, 3);
    while (randHSpeed == 0){
        randHSpeed = randInt(-3, 3);
    }
    setHorizSpeed(randHSpeed);
        //movement plan
    setMovementPlan(randInt(4, 32));
        //direction
    if (getHorizSpeed() < 0){
        setDirection(180);
    } else {
        setDirection(0);
    }
}

// HumanPedestrian MEMBER FUNCTION IMPLEMENTATIONS

HumanPedestrian::HumanPedestrian(int imageID, double startX, double startY,
                                 int startDirection, double size,
                                 StudentWorld* sw,
                                 int hitPoints)
: IntelligentAgent(imageID, startX, startY, startDirection, size, sw, hitPoints)
{ }

void HumanPedestrian::doSomething(){
    //if human ped isn't alive, return immediately
    if (getHitPoints() <= 0){
        return;
    }
    
    //if human ped overlaps w/ GR, player loses life, level ends (GR hp set to 0 and alive set to false), return immediately (#2)
    if (getStudentWorld()->getOverlappingGhostRacer(this) != nullptr){
        getStudentWorld()->decLives();  //decrease lives by one
        getStudentWorld()->getOverlappingGhostRacer(this)->setHitPoints(0); //update GR hp to 0
        getStudentWorld()->getOverlappingGhostRacer(this)->changeAlive(false);  //update GR alive status to false
        return;
    }
    
    //move, update plan distance
    moveAndPossiblyPickPlan();
}

bool HumanPedestrian::beSprayedIfAppropriate(){
    setHorizSpeed(getHorizSpeed() * (-1));
    if (getDirection() == 0){
        setDirection(180);
    } else if (getDirection() == 180){
        setDirection(0);
    }
    getStudentWorld()->playSound(SOUND_PED_HURT);
    return true;
}

bool HumanPedestrian::takeDamageAndPossiblyDie(int hp){
    return false;
}

// ZombiePedestrian MEMBER FUNCTION IMPLEMENTATIONS

//Private
bool ZombiePedestrian::updateMovementPlan(){
    if (getMovementPlan() > 0){
        setMovementPlan(getMovementPlan()-1);
        return true;    //this tells the caller of this function to return immediately if bool = true
    }
    return false;
}

// ZombieCab MEMBER FUNCTION IMPLEMENTATIONS
void ZombieCab::pickNewPlan(){
    //pick new movement plan for zombie cab
        //movement plan
    setMovementPlan(randInt(4, 32));
        //vertical speed
    setVertSpeed(getVertSpeed()+randInt(-2, 2));
}

