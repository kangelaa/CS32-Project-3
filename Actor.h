#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public: 
    Actor(int imageID, double startX, double startY,
          int startDirection, double size, int depth,
          StudentWorld* sw);
    virtual ~Actor(){ } //"implementation" necessary even though it's not functional
    virtual void doSomething() = 0;
    bool isAlive() const;
    void changeAlive(bool isAlive);
    int getVertSpeed() const;
    void setVertSpeed(int vSpeed); //parameter passed is the new value
    virtual bool isCollisionAW() const; //Does this object affect zombie cab placement and speed?
    virtual bool beSprayedIfAppropriate();
    StudentWorld* getStudentWorld() const;
    // Adjust the x coordinate by dx to move to a position with a y coordinate
          // determined by this actor's vertical speed relative to GhostRacser's
          // vertical speed.  Return true if the new position is within the view;
          // otherwise, return false, with the actor dead.
    virtual bool moveRelativeToGhostRacerVerticalSpeed(double dx);
private:
    bool m_alive;
    int m_vertSpeed;
    StudentWorld* m_sw;
};

class BorderLine : public Actor
{
public:
    BorderLine(int imageID, double startX, double startY,
               int startDirection, double size, int depth,
               int vertSpeed, StudentWorld* sw);
private:
    virtual ~BorderLine(){ }
    virtual void doSomething();
};

class Agent : public Actor
{
public:
    Agent(int imageID, double startX, double startY,
          int startDirection, double size,
          StudentWorld* sw,
          int hitPoints);
    virtual ~Agent(){ }
    int getHitPoints() const;
    void setHitPoints(int hp);
    int getHorizSpeed() const;
    void setHorizSpeed(int hSpeed); //parameter passed is the new value
    virtual bool isCollisionAW() const;
    // Move the agent. If the agent doesn't go off screen and
    // should pick a new movement plan, pick a new plan.
    virtual void moveAndPossiblyPickPlan() = 0;
    
// Do what the spec says happens when hp units of damage is inflicted.
      // Return true if this agent dies as a result, otherwise false.
    virtual bool takeDamageAndPossiblyDie(int hp);

      // What sound should play when this agent is damaged but does not die?
    virtual int soundWhenHurt() const;

      // What sound should play when this agent is damaged and dies?
    virtual int soundWhenDie() const;
    
private:
    int m_hitPoints;
    int m_horizSpeed;
};

class GhostRacer : public Agent
{
public:
    GhostRacer(int imageID, double startX, double startY,
               int startDirection, double size,
               int vertSpeed, int horizSpeed, StudentWorld* sw,
               int holyWaterAmount);
    virtual ~GhostRacer(){ }
    virtual void doSomething();
    int getNumSprays() const;
    void changeNumSprays(int waterChange);
    void spin();
    void heal(int hp);
private:
    virtual void moveAndPossiblyPickPlan();
    int m_holyWaterAmount;
};

class IntelligentAgent : public Agent
{
public:
    IntelligentAgent(int imageID, double startX, double startY,
                       int startDirection, double size,
                       StudentWorld* sw,
                       int hitPoints);
    virtual ~IntelligentAgent(){ }
    int getMovementPlan();
    void setMovementPlan(int plan);
    virtual void moveAndPossiblyPickPlan();
private:
    virtual bool updateMovementPlan();
    virtual void pickNewPlan();
    int m_movementPlan;
};

class HumanPedestrian : public IntelligentAgent
{
public:
    HumanPedestrian(int imageID, double startX, double startY,
                    int startDirection, double size,
                    StudentWorld* sw,
                    int hitPoints);
private:
    virtual ~HumanPedestrian(){ }
    virtual void doSomething();
    virtual bool beSprayedIfAppropriate();
    virtual bool takeDamageAndPossiblyDie(int hp);
};

class ZombiePedestrian : public IntelligentAgent
{
public:
    ZombiePedestrian(int imageID, double startX, double startY,
                     int startDirection, double size,
                     StudentWorld* sw,
                     int hitPoints);
private:
    virtual ~ZombiePedestrian(){ }
    virtual void doSomething();
    virtual int soundWhenHurt() const;
    virtual int soundWhenDie() const;
    virtual bool beSprayedIfAppropriate();
    virtual bool updateMovementPlan();
    void zombiePedSpecificDamage(bool died);
    int m_ticksBeforeGrunt;
};

class ZombieCab : public IntelligentAgent
{
public:
    ZombieCab(int imageID, double startX, double startY,
              int startDirection, double size,
              StudentWorld* sw,
              int hitPoints, int vertSpeed);
private:
    virtual ~ZombieCab(){ }
    virtual void doSomething();
    virtual bool beSprayedIfAppropriate();
    virtual int soundWhenHurt() const;
    virtual int soundWhenDie() const;
    virtual bool updateMovementPlan();
    virtual void pickNewPlan();
    bool zombieCabSpecificDamage(bool died);
    bool m_damagedGRYet;
};

class Spray : public Actor
{
public:
    Spray(int imageID, double startX, double startY,
          int startDirection, double size, int depth,
          StudentWorld* sw);
private:
    virtual ~Spray(){ }
    virtual void doSomething();
    int m_travelDistLeft;
};

class GhostRacerActivatedObject : public Actor
{
public:
    GhostRacerActivatedObject(int imageID, double startX, double startY,
                              int startDirection, double size,
                              StudentWorld* sw);
    virtual ~GhostRacerActivatedObject(){ }
private:
    virtual bool beSprayedIfAppropriate();
    
    virtual void doSomething();
    
      // Do the object's special activity (increase health, spin Ghostracer, etc.)
    virtual void doActivity(GhostRacer* gr) = 0;

      // Return the object's increase to the score when activated.
    virtual int getScoreIncrease() const = 0;

      // Return the sound to be played when the object is activated.
    virtual int getSound() const;

      // Return whether the object dies after activation.
    virtual bool selfDestructs() const = 0;

      // Return whether the object is affected by a holy water projectile.
    virtual bool isSprayable() const = 0;
};

class OilSlick : public GhostRacerActivatedObject
{
public:
    OilSlick(int imageID, double startX, double startY,
             int startDirection, double size,
             StudentWorld* sw);
private:
    virtual ~OilSlick(){ }
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual int getSound() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};

class HealingGoodie : public GhostRacerActivatedObject
{
public:
    HealingGoodie(int imageID, double startX, double startY,
                  int startDirection, double size,
                  StudentWorld* sw);
private:
    virtual ~HealingGoodie(){ }
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};

class HolyWaterGoodie : public GhostRacerActivatedObject
{
public:
    HolyWaterGoodie(int imageID, double startX, double startY,
                    int startDirection, double size,
                    StudentWorld* sw);
private:
    virtual ~HolyWaterGoodie(){ }
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};

class SoulGoodie : public GhostRacerActivatedObject
{
public:
    SoulGoodie(int imageID, double startX, double startY,
               int startDirection, double size,
               StudentWorld* sw);
private:
    virtual ~SoulGoodie(){ }
    virtual void doSomething();
    virtual void doActivity(GhostRacer* gr);
    virtual int getScoreIncrease() const;
    virtual int getSound() const;
    virtual bool selfDestructs() const;
    virtual bool isSprayable() const;
};

#endif // ACTOR_H_
