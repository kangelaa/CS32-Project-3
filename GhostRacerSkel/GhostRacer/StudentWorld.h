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
private:
    GhostRacer* m_pointerToGhostRacer;
    std::list<Actor*> m_listOfActors;
};

#endif // STUDENTWORLD_H_
