#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:     //TODO: GO IN LATER AND CHANGE EVERYTHING TO VIRTUAL/VIRTUAL PURE/CONST AS NECESSARY FOR ALL CLASSES!
    Actor(int imageID, double startX, double startY,
          int startDirection, double size, int depth,
          int vertSpeed, int horizSpeed, bool collisionAW, StudentWorld* sw);
    virtual ~Actor(){ } //"implementation" necessary even though it's not functional
    virtual void doSomething() = 0;
    bool isAlive() const;
    void changeAlive(bool isAlive);
    int getVertSpeed() const;
    void changeVertSpeed(int vSpeedChange); //parameter passed is the increment/decrement amount
    int getHorizSpeed() const;
    void changeHorizSpeed(int hSpeedChange); //parameter passed is the increment/decrement amount
    bool isCollisionAW() const;
private:
    bool m_alive;
    int m_vertSpeed;
    int m_horizSpeed;   //TODO: CHECK IF THIS BELONGS IN ACTOR CLASS? (GHOST RACER DOESN'T HAVE ONE?)
    bool m_collisionAW; //TODO: UPDATE THIS FOR PT 2, can't have same data members among classes (same w/ hit points!)
    StudentWorld* m_sw;
    //TODO: ADD IN DATA MEMBERS/FUNCTIONS AS NECESSARY, ADD ACCESSOR/MUTATORS FOR PRIV DATA MEMBERS; FOR ALL CLASSES!
};

class GhostRacer : public Actor
{
public: //TODO: GO IN LATER AND UDPATE EVERYTHING TO VIRTUAL AS NECESSARY
    GhostRacer(int imageID, double startX, double startY,
               int startDirection, double size, int depth,
               int vertSpeed, int horizSpeed, bool collisionAW, StudentWorld* sw,
               int holyWaterAmount, int hitPoints);
    virtual ~GhostRacer(){ }
    virtual void doSomething();
    int getHolyWaterAmount() const;
    void changeHolyWaterAmount(int waterChange);
    int getHitPoints() const;
    void changeHitPoints(int hpChange);
private:
    int m_holyWaterAmount;
    int m_hitPoints;
};

class BorderLine : public Actor
{
public:
    BorderLine(int imageID, double startX, double startY,
               int startDirection, double size, int depth,
               int vertSpeed, int horizSpeed, bool collisionAW, StudentWorld* sw);
    virtual ~BorderLine(){ }
    virtual void doSomething();
private:
};

//TODO: ORGANIZE HOW MANY OTHER DERIVED CLASSES THAT AREN'T DIRECT ACTORS? (3?)

#endif // ACTOR_H_
