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
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    virtual ~StudentWorld();

    // Return a pointer to the world's GhostRacer.
    GhostRacer* getPointerToGhostRacer() const;
    
    // Add an actor to the world.
  void addActor(Actor* a);

    // Record that a soul was saved.
  void recordSoulSaved();
    
    //TODO: CHOOSE MEMBER FUNCTIONS FROM HERE DOWN!
    // If actor a overlaps some live actor that is affected by a holy water
    // projectile, inflict a holy water spray on that actor and return true;
    // otherwise, return false.  (See Actor::beSprayedIfAppropriate.)
  bool sprayFirstAppropriateActor(Actor* a);

    // Return true if actor a1 overlaps actor a2, otherwise false.
  bool overlaps(const Actor* a1, const Actor* a2) const;

    // If actor a overlaps this world's GhostRacer, return a pointer to the
    // GhostRacer; otherwise, return nullptr
  GhostRacer* getOverlappingGhostRacer(Actor* a) const;

private:
    GhostRacer* m_pointerToGhostRacer;
    std::list<Actor*> m_listOfActors; //TODO: (note) only StudentWorld should have access to this container, don't make a public member function to access it)
    int m_numSoulsToSave;
};

#endif // STUDENTWORLD_H_
