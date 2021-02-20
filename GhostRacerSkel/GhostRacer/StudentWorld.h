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
    GhostRacer* getPointerToGhostRacer() const;
private:
    GhostRacer* m_pointerToGhostRacer;
    std::list<Actor*> m_listOfActors; //TODO: DO I NEED TO IMPLEMENT WAYS OF ACCESSING THESE POINTERS? (not this container! only StudentWorld should have access to this container, don't make a public member function to access it)
};

#endif // STUDENTWORLD_H_
