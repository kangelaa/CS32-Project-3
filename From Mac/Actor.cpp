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

bool Actor::beSprayedIfAppropriate(){
    return false;
}

//Accessor
StudentWorld* Actor::getStudentWorld() const {
    return m_sw;
}

// Adjust the x coordinate by dx to move to a position with a y coordinate
      // determined by this actor's vertical speed relative to GhostRacser's
      // vertical speed.  Return true if the new position is within the view;
      // otherwise, return false, with the actor dead.
bool Actor::moveRelativeToGhostRacerVerticalSpeed(double dx){
    int vert_speed = getVertSpeed() - getStudentWorld()->getPointerToGhostRacer()->getVertSpeed();
    int horiz_speed = dx;
    double new_y = getY() + vert_speed;
    double new_x = getX() + horiz_speed;
    moveTo(new_x, new_y);
    
    //if Agent leaves the screen, set its status to not-alive, so it will be removed by StudentWorld later in this tick
    if (getX() < 0 || getX() > VIEW_WIDTH || getY() < 0 || getY() > VIEW_HEIGHT){
        changeAlive(false);
        return false;
    }
    return true;
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
    //move BorderLine using algorithm
    if(!moveRelativeToGhostRacerVerticalSpeed(0)){
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
        getStudentWorld()->playSound(soundWhenHurt());
    } else if (getX() >= RIGHT_EDGE){
        if (getDirection() < 90){
            takeDamageAndPossiblyDie(10);
        }
        setDirection(98);
        getStudentWorld()->playSound(soundWhenHurt()); 
    } else if (getStudentWorld()->getKey(ch)){   //check for user input (key press)
        //user hit a key during this tick!
        switch (ch){
            case KEY_PRESS_SPACE:
                if (getNumSprays() >= 1){
                    //add a new water spray directly in front of GR in whatever direction it's facing
                    int angleDegRef = getStudentWorld()->getPointerToGhostRacer()->getDirection();  //get dir in degrees
                    int angleDeg;
                    if (angleDegRef > 90){  //find acute angle
                        angleDeg = 180 - angleDegRef;
                    } else {
                        angleDeg = angleDegRef;
                    }
                    double angleRad = angleDeg * (M_PI/180);    //convert deg to rad
                    double delta_x = cos(angleRad) * SPRITE_HEIGHT; //calculate delta_x
                    double delta_y = sin(angleRad) * SPRITE_HEIGHT; //calculate delta_y
                    if (angleDegRef > 90){
                        delta_x *= (-1);        //delta_x is subtracted from GR's getX() if the original dir was > 90
                    }
                    getStudentWorld()->addActor(new Spray(IID_HOLY_WATER_PROJECTILE, getStudentWorld()->getPointerToGhostRacer()->getX() + delta_x, getStudentWorld()->getPointerToGhostRacer()->getY() + delta_y,
                                                          angleDegRef, 1.0, 1,
                                                          getStudentWorld()));
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

//Spin as result of hitting oil slick
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
: Agent(imageID, startX, startY, startDirection, size, sw, hitPoints)
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
    if (!moveRelativeToGhostRacerVerticalSpeed(getHorizSpeed())){
        return;
    }
    
    //works for human ped, not zombie ped or zombie cab (cab is same but w/ extra added to the start)
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
{
    setVertSpeed(-4);
}

void HumanPedestrian::doSomething(){
    //if human ped isn't alive, return immediately
    if (getHitPoints() <= 0){
        return;
    }
    
    //if human ped overlaps w/ GR, player loses life, level ends (GR hp set to 0 and alive set to false), return immediately (#2)
    if (getStudentWorld()->getOverlappingGhostRacer(this) != nullptr){
        getStudentWorld()->decLives();  //decrease lives by one
        getStudentWorld()->getOverlappingGhostRacer(this)->takeDamageAndPossiblyDie(100);
        return;
    }
    
    //move, update plan distance (#3-6)
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

ZombiePedestrian::ZombiePedestrian(int imageID, double startX, double startY,
                 int startDirection, double size,
                 StudentWorld* sw,
                 int hitPoints)
: IntelligentAgent(imageID, startX, startY, startDirection, size, sw, hitPoints)
{
    setVertSpeed(-4);
    m_ticksBeforeGrunt = 0;
}

void ZombiePedestrian::doSomething(){
    //if zombie ped isn't alive, return immediately
    if (getHitPoints() <= 0){
        return;
    }
    
    //if zombie ped overlaps w/ GR (#2)
    if (getStudentWorld()->getOverlappingGhostRacer(this) != nullptr){
        getStudentWorld()->getOverlappingGhostRacer(this)->takeDamageAndPossiblyDie(5);     //GR damaged (-5)
        bool died = takeDamageAndPossiblyDie(2);    //zombie ped damaged (-2), passes bool = true if died, false if alive
        zombiePedSpecificDamage(died);
        return;
    }
    
    //  zombie ped grunts every 20 ticks if it is close enough to (x-axis) GR (#3)
    if ((getX() - getStudentWorld()->getPointerToGhostRacer()->getX()) <= 30 && (getX() - getStudentWorld()->getPointerToGhostRacer()->getX()) >= -30 && getY() > getStudentWorld()->getPointerToGhostRacer()->getY()){
        setDirection(270);
        if (getX() < getStudentWorld()->getPointerToGhostRacer()->getX()){
            setHorizSpeed(1);
        } else if (getX() > getStudentWorld()->getPointerToGhostRacer()->getX()){
            setHorizSpeed(-1);
        } else {
            setHorizSpeed(0);
        }
        m_ticksBeforeGrunt--;
        if (m_ticksBeforeGrunt <= 0){
            getStudentWorld()->playSound(SOUND_ZOMBIE_ATTACK);
            m_ticksBeforeGrunt = 20;
        }
    }
    
    //move, update plan distance (#4-6)
    moveAndPossiblyPickPlan();
}

bool ZombiePedestrian::beSprayedIfAppropriate(){
    bool died = takeDamageAndPossiblyDie(1);
    zombiePedSpecificDamage(died);
    return true;
}

int ZombiePedestrian::soundWhenHurt() const {
    return SOUND_PED_HURT;
}

int ZombiePedestrian::soundWhenDie() const {
    return SOUND_PED_DIE;
}

//Private functions
bool ZombiePedestrian::updateMovementPlan(){
    if (getMovementPlan() > 0){
        setMovementPlan(getMovementPlan()-1);
        return true;    //this tells the caller of this function to return immediately if bool = true
    }
    return false;
}

void ZombiePedestrian::zombiePedSpecificDamage(bool died){
    if (died){
        if (getStudentWorld()->getOverlappingGhostRacer(this) == nullptr){
            int chance = randInt(1, 5);
            if (chance == 1){
                //add a new HealingGoodie at current pos of ZombiePedestrian
                getStudentWorld()->addActor(new HealingGoodie(IID_HEAL_GOODIE, getX(), getY(),
                                                              0, 1.0,
                                                              getStudentWorld()));
            }
        }
        getStudentWorld()->increaseScore(150);
    } else {
        getStudentWorld()->playSound(soundWhenHurt());
    }
}

// ZombieCab MEMBER FUNCTION IMPLEMENTATIONS

ZombieCab::ZombieCab(int imageID, double startX, double startY,
          int startDirection, double size,
          StudentWorld* sw,
          int hitPoints, int vertSpeed)
: IntelligentAgent(imageID, startX, startY, startDirection, size, sw, hitPoints)
{
    setVertSpeed(vertSpeed);
    m_damagedGRYet = false;
}

void ZombieCab::doSomething(){
    //if zombie cab isn't alive, return immediately
    if (getHitPoints() <= 0){
        return;
    }
    
    //if zombie cab overlaps w/ GR (#2)
    if (getStudentWorld()->getOverlappingGhostRacer(this) != nullptr){
        if (m_damagedGRYet == false){
            getStudentWorld()->playSound(SOUND_VEHICLE_CRASH);
            getStudentWorld()->getPointerToGhostRacer()->takeDamageAndPossiblyDie(20);
            if (getX() <= getStudentWorld()->getPointerToGhostRacer()->getX()){
                setHorizSpeed(-5);
                setDirection(120+randInt(0, 19));
            } else {
                setHorizSpeed(5);
                setDirection(60-randInt(0, 19));
            }
            m_damagedGRYet = true;
        }
    }
    
    //move, update plan distance (#3-6)
    moveAndPossiblyPickPlan();
}

bool ZombieCab::beSprayedIfAppropriate(){
    bool died = takeDamageAndPossiblyDie(1);
    zombieCabSpecificDamage(died);
    return true;
}

int ZombieCab::soundWhenHurt() const{
    return SOUND_VEHICLE_HURT;
}

int ZombieCab::soundWhenDie() const{
    return SOUND_VEHICLE_DIE;
}

//Private functions

bool ZombieCab::updateMovementPlan(){
    // (#4-7 on pg 39)
    int leftX;
    int rightX;
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    
    if (getX() == ROAD_CENTER - ROAD_WIDTH/3){  //left lane check
        leftX = LEFT_EDGE;
        rightX = LEFT_EDGE + ROAD_WIDTH/3;
    } else if (getX() == ROAD_CENTER){  //middle lane check
        leftX = LEFT_EDGE + ROAD_WIDTH/3;
        rightX = RIGHT_EDGE - ROAD_WIDTH/3;
    } else {    //right lane check
        leftX = RIGHT_EDGE - ROAD_WIDTH/3;
        rightX = RIGHT_EDGE;
    }
    
    if (getVertSpeed() > getStudentWorld()->getPointerToGhostRacer()->getVertSpeed() && getStudentWorld()->getClosestCAWinFrontOrBehind(leftX, rightX, getY(), true) != nullptr){
        if (getStudentWorld()->getClosestCAWinFrontOrBehind(leftX, rightX, getY(), true)->getY() - getY() < 96){
            setVertSpeed(getVertSpeed()-.5);
            return true;
        }
    }
    
    if (getVertSpeed() <= getStudentWorld()->getPointerToGhostRacer()->getVertSpeed() && getStudentWorld()->getClosestCAWinFrontOrBehind(leftX, rightX, getY(), false) != nullptr){
        if (getY() - getStudentWorld()->getClosestCAWinFrontOrBehind(leftX, rightX, getY(), false)->getY() < 96 && getStudentWorld()->getClosestCAWinFrontOrBehind(leftX, rightX, getY(), false) != getStudentWorld()->getPointerToGhostRacer()){
            setVertSpeed(getVertSpeed()+.5);
            return true;
        }
    }

    //decrement movement plan distance
    setMovementPlan(getMovementPlan()-1);
    if (getMovementPlan() > 0){
        return true;    //this tells the caller of this function to return immediately if bool = true
    }
    return false;
}

void ZombieCab::pickNewPlan(){
    //pick new movement plan for zombie cab
        //movement plan
    setMovementPlan(randInt(4, 32));
        //vertical speed
    setVertSpeed(getVertSpeed()+randInt(-2, 2));
}

//if this bool function returns true, return immediately
bool ZombieCab::zombieCabSpecificDamage(bool died){
    if (died){
        int chance = randInt(1, 5);
        if (chance == 1){
            //add new oil slick at current ZombieCab position
            double size = randInt(2, 5);
            getStudentWorld()->addActor(new OilSlick(IID_OIL_SLICK, getX(), getY(),
                                                     0, size,
                                                     getStudentWorld()));
        }
        getStudentWorld()->increaseScore(200);
        return true;
    } else {
        getStudentWorld()->playSound(soundWhenHurt());
        return false;
    }
}

//Spray MEMBER FUNCTION IMPLEMENTATIONS

Spray::Spray(int imageID, double startX, double startY,
             int startDirection, double size, int depth,
             StudentWorld* sw)
: Actor(imageID, startX, startY, startDirection, size, depth, sw)
{
    m_travelDistLeft = 160;
}

void Spray::doSomething(){
    //if spray isn't alive, return immediately
    if (!isAlive()){
        return;
    }
    
    // check to see if there are any sprayable actors currently overlapping w/ spray, spray them, change spray status to not alive, and return immediately (#2)
    if (getStudentWorld()->sprayFirstAppropriateActor(this)){
        changeAlive(false);
        return;
    }
    
    // move spray forward (#3-5) and decrement m_travelDistLeft by the # of pixels moved
    moveForward(SPRITE_HEIGHT);
    m_travelDistLeft -= SPRITE_HEIGHT;
    
    //kill spray if off screen
    if (getX() < 0 || getX() > VIEW_WIDTH || getY() < 0 || getY() > VIEW_HEIGHT){
        changeAlive(false);
        return;
    }
    
    //if spray traveled max dist (160 pixels), kill it
    if (m_travelDistLeft <= 0){
        changeAlive(false);
    }
}

// GhostRacerActivatedObject MEMBER FUNCTION IMPLEMENTATIONS

GhostRacerActivatedObject::GhostRacerActivatedObject(int imageID, double startX, double startY,
                                                     int startDirection, double size,
                                                     StudentWorld* sw)
: Actor(imageID, startX, startY, startDirection, size, 2, sw)
{
    setVertSpeed(-4);
}

bool GhostRacerActivatedObject::beSprayedIfAppropriate(){
    if (isSprayable()){
        changeAlive(false);
        return true;
    } else {
        return false;
    }
}

void GhostRacerActivatedObject::doSomething(){
    if(!moveRelativeToGhostRacerVerticalSpeed(0)){
        return;
    }
    if (getStudentWorld()->getOverlappingGhostRacer(this) != nullptr){
        doActivity(getStudentWorld()->getPointerToGhostRacer()); 
        if (selfDestructs()){
            changeAlive(false);
        }
        getStudentWorld()->playSound(getSound());
        getStudentWorld()->increaseScore(getScoreIncrease());
    }
}

  // Return the sound to be played when the object is activated.
int GhostRacerActivatedObject::getSound() const {
    return SOUND_GOT_GOODIE;
}

// OilSlick MEMBER FUNCTION IMPLEMENTATIONS

OilSlick::OilSlick(int imageID, double startX, double startY,
                   int startDirection, double size,
                   StudentWorld* sw)
: GhostRacerActivatedObject(imageID, startX, startY, startDirection, size, sw)
{ }

void OilSlick::doActivity(GhostRacer* gr){
    gr->spin();
}

int OilSlick::getScoreIncrease() const {
    return 0;
}

int OilSlick::getSound() const {
    return SOUND_OIL_SLICK;
}

bool OilSlick::selfDestructs() const {
    return false;
}

bool OilSlick::isSprayable() const {
    return false;
}

// HealingGoodie MEMBER FUNCTION IMPLEMENTATIONS

HealingGoodie::HealingGoodie(int imageID, double startX, double startY,
                             int startDirection, double size,
                             StudentWorld* sw)
: GhostRacerActivatedObject(imageID, startX, startY, startDirection, size, sw)
{ }

void HealingGoodie::doActivity(GhostRacer* gr){
    gr->heal(10);
}

int HealingGoodie::getScoreIncrease() const {
    return 250;
}

bool HealingGoodie::selfDestructs() const {
    return true;
}

bool HealingGoodie::isSprayable() const {
    return true;
}

// HolyWaterGoodie MEMBER FUNCTION IMPLEMENTATIONS

HolyWaterGoodie::HolyWaterGoodie(int imageID, double startX, double startY,
                                 int startDirection, double size,
                                 StudentWorld* sw)
: GhostRacerActivatedObject(imageID, startX, startY, startDirection, size, sw)
{ }

void HolyWaterGoodie::doActivity(GhostRacer* gr) {
    gr->changeNumSprays(10);
}

int HolyWaterGoodie::getScoreIncrease() const {
    return 50;
}

bool HolyWaterGoodie::selfDestructs() const{
    return true;
}

bool HolyWaterGoodie::isSprayable() const {
    return true;
}

// SoulGoodie MEMBER FUNCTION IMPLEMENTATIONS

SoulGoodie::SoulGoodie(int imageID, double startX, double startY,
                       int startDirection, double size,
                       StudentWorld* sw)
: GhostRacerActivatedObject(imageID, startX, startY, startDirection, size, sw)
{ }

void SoulGoodie::doSomething(){
    if(!moveRelativeToGhostRacerVerticalSpeed(0)){
        return;
    }
    if (getStudentWorld()->getOverlappingGhostRacer(this) != nullptr){
        doActivity(getStudentWorld()->getPointerToGhostRacer());
        changeAlive(false);
        getStudentWorld()->playSound(getSound());
        getStudentWorld()->increaseScore(getScoreIncrease());
    }
    setDirection(getDirection()-10);
}

void SoulGoodie::doActivity(GhostRacer* gr){
    getStudentWorld()->recordSoulSaved();
}

int SoulGoodie::getScoreIncrease() const {
    return 100;
}

int SoulGoodie::getSound() const {
    return SOUND_GOT_SOUL;
}

bool SoulGoodie::selfDestructs() const {
    return true;
}

bool SoulGoodie::isSprayable() const {
    return false;
}
