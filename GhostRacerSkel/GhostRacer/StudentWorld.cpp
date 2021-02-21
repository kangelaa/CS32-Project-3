#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_pointerToGhostRacer = nullptr;
}

int StudentWorld::init()
{
    //add new GhostRacer and initialize pointer to point to it
    m_pointerToGhostRacer = new GhostRacer(IID_GHOST_RACER, 128, 32, 90, 4.0, 0, 0, 0, true, this, 10, 100);

    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    //add N yellow border line objects (N = VIEW_HEIGHT / SPRITE_HEIGHT) on each side of the road (L/R)
    for (int j=0; j<(VIEW_HEIGHT / SPRITE_HEIGHT); j++){
        m_listOfActors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE, j * SPRITE_HEIGHT, 0, 2.0, 2, -4, 0, false, this));
        m_listOfActors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, RIGHT_EDGE, j * SPRITE_HEIGHT, 0, 2.0, 2, -4, 0, false, this));
    }
    
    //add M white border line objects (M = VIEW_HEIGHT / (4*SPRITE_HEIGHT)) to separate the three lanes (2 sets of "lines")
    for (int j=0; j<(VIEW_HEIGHT / (4*SPRITE_HEIGHT)); j++){
        m_listOfActors.push_front(new BorderLine(IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH/3, j *
                                                (4*SPRITE_HEIGHT), 0, 2.0, 2, -4, 0, false, this));
        m_listOfActors.push_front(new BorderLine(IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH/3, j *
                                                (4*SPRITE_HEIGHT), 0, 2.0, 2, -4, 0, false, this));
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
//    // The term "actors" refers to all actors, Ghost Racer, pedestrians,
//     // vehicles, goodies, oil slicks, holy water, spray, lost souls, etc.
//     // Give each actor a chance to do something, including Ghost Racer
//    for each of the actors in the game world
//    {
//     if (the actor is still active/alive)
//     {
//     // tell that actor to do something (e.g. move)
//     the actor->doSomething();
//    if (Ghost Racer was destroyed during this tick)
//    return GWSTATUS_PLAYER_DIED;
//    if (Ghost Racer completed the currentLevel)
//    {
//    add bonus points to the score
//    return GWSTATUS_FINISHED_LEVEL;
//    }
//     }
//    }
//     // Remove newly-dead actors after each tick
//    Remove and delete dead game objects
//     // Potentially add new actors to the game
//     // (e.g., oil slicks or goodies or border lines)
//    Add new actors
//     // Update the Game Status Line
//    Update display text // update the score/lives/level text at screen top
//     // the player hasn’t completed the current level and hasn’t died, so
//     // continue playing the current level
//    return GWSTATUS_CONTINUE_GAME;
    
    if (getPointerToGhostRacer()->isAlive()){
        getPointerToGhostRacer()->doSomething();
    }
    
    for (list<Actor*>::iterator p = m_listOfActors.begin(); p != m_listOfActors.end(); p++){
        if ((*p)->isAlive()){
            (*p)->doSomething();
            //TODO: FIX/FINISH IMPLEMENTATION FOR PT 2
//            if (!(getPointerToGhostRacer()->isAlive())){
//                return GWSTATUS_PLAYER_DIED;
//            }
        }
    }
    
    // Remove newly-dead actors after each tick
    for (list<Actor*>::iterator p = m_listOfActors.begin(); p != m_listOfActors.end(); p++){
        if (!((*p)->isAlive())){
            delete (*p);
            p = m_listOfActors.erase(p);    //erase pointer and reassign p to point to object following the one erased
            p--;    //decrement p to prepare for next iteration of loop
        }
    }
    
    // Potentially add new actors to the game
    double new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    double m_last_white_y_added = m_listOfActors.front()->getY();   //TODO: MAKE SURE UR ONLY PUSHING WHITE BORDERS TO FRONT!!! EVERYTHING ELSE TO BACK!
    double delta_y = new_border_y - m_last_white_y_added;
    if (delta_y >= SPRITE_HEIGHT){
        m_listOfActors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH/2, new_border_y, 0, 2.0, 2, -4, 0, false, this));
        m_listOfActors.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH/2, new_border_y, 0, 2.0, 2, -4, 0, false, this));
    }
    if (delta_y >= 4*SPRITE_HEIGHT){
        m_listOfActors.push_front(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 2 + ROAD_WIDTH/3, new_border_y, 0, 2.0, 2, -4, 0, false, this));
        m_listOfActors.push_front(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 2 - ROAD_WIDTH/3, new_border_y, 0, 2.0, 2, -4, 0, false, this));
    }
    
    // Update the Game Status Line
    
    // the player hasn’t completed the current level and hasn’t died, so
    // continue playing the current level
    return GWSTATUS_CONTINUE_GAME;
    
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
//    decLives();
//    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //delete GhostRacer
    delete m_pointerToGhostRacer;
    
    //iterates through the list container, deleting each Actor
    for (list<Actor*>::iterator p = m_listOfActors.begin(); p != m_listOfActors.end(); p++){
        delete (*p); //TODO: CHECK THIS IS CORRECT?
    }
    
    //delete each object (pointer) in the container, emptying the container
    m_listOfActors.clear();
}

//Destructor that frees any remaining dynamically allocated data that hasn't been freed before StudentWorld is destroyed
StudentWorld::~StudentWorld(){
    cleanUp();
}

//Accessor
GhostRacer* StudentWorld::getPointerToGhostRacer() const {
    return m_pointerToGhostRacer;
}
