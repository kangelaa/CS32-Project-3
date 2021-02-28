#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
#include "Actor.h"
#include <list>

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);

    // Return a pointer to the world's GhostRacer.
    GhostRacer* getPointerToGhostRacer() const;
    
    // Add an actor to the world.
  void addActor(Actor* a);

    // Record that a soul was saved.
  void recordSoulSaved();
    
    // If actor a overlaps some live actor that is affected by a holy water
    // projectile, inflict a holy water spray on that actor and return true;
    // otherwise, return false.  (See Actor::beSprayedIfAppropriate.)
  bool sprayFirstAppropriateActor(Actor* a);

    // If actor a overlaps this world's GhostRacer, return a pointer to the
    // GhostRacer; otherwise, return nullptr
  GhostRacer* getOverlappingGhostRacer(Actor* a) const;
    
    //Returns pointer to CAW actor in specified lane in FRONT of/BEHIND and closest to specified y, nullptr if none
  Actor* getClosestCAWinFrontOrBehind(int leftX, int rightX, int y, bool front);
    
private:
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual ~StudentWorld();
    
    // Return true if actor a1 overlaps actor a2, otherwise false.
  bool overlaps(const Actor* a1, const Actor* a2) const;
    
    //Returns pointer to closest CAW actor in specified lane to specified y, nullptr if none
  Actor* getClosestCAW(int leftX, int rightX, int y);

    GhostRacer* m_pointerToGhostRacer;
    std::list<Actor*> m_listOfActors;
    int m_numSoulsToSave;
    int m_bonus;
};

#endif // STUDENTWORLD_H_
