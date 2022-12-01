#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream> // defines the overloads of the << operator
#include <sstream>  // defines the type std::ostringstream

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
    //initialize m_numSoulsToSave with the # for the level and m_bonus w/ starting bonus of 5000
    m_numSoulsToSave = 2 * getLevel() + 5;
    m_bonus = 5000;
    
    //add new GhostRacer and initialize pointer to point to it
    m_pointerToGhostRacer = new GhostRacer(IID_GHOST_RACER, 128, 32, 90, 4.0, 0, 0, this, 10);

    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    //add N yellow border line objects (N = VIEW_HEIGHT / SPRITE_HEIGHT) on each side of the road (L/R)
    for (int j=0; j<(VIEW_HEIGHT / SPRITE_HEIGHT); j++){
        addActor(new BorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE, j * SPRITE_HEIGHT, 0, 2.0, 2, -4, this));
        addActor(new BorderLine(IID_YELLOW_BORDER_LINE, RIGHT_EDGE, j * SPRITE_HEIGHT, 0, 2.0, 2, -4, this));
    }
    
    //add M white border line objects (M = VIEW_HEIGHT / (4*SPRITE_HEIGHT)) to separate the three lanes (2 sets of "lines")
    for (int j=0; j<(VIEW_HEIGHT / (4*SPRITE_HEIGHT)); j++){
        m_listOfActors.push_front(new BorderLine(IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH/3, j *
                                                (4*SPRITE_HEIGHT), 0, 2.0, 2, -4, this));
        m_listOfActors.push_front(new BorderLine(IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH/3, j *
                                                (4*SPRITE_HEIGHT), 0, 2.0, 2, -4, this));
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // Give each actor a chance to do something, including Ghost Racer
    if (getPointerToGhostRacer()->isAlive()){
        getPointerToGhostRacer()->doSomething();
    }
    
    for (list<Actor*>::iterator p = m_listOfActors.begin(); p != m_listOfActors.end(); p++){
        if ((*p)->isAlive()){
            (*p)->doSomething();
            if (!(getPointerToGhostRacer()->isAlive())){
                return GWSTATUS_PLAYER_DIED;
            }
            
            if (m_numSoulsToSave == 0){
                increaseScore(m_bonus);
                return GWSTATUS_FINISHED_LEVEL;
            }
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
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
    
    //BorderLine
    double new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    double m_last_white_y_added = m_listOfActors.front()->getY(); 
    double delta_y = new_border_y - m_last_white_y_added;
    if (delta_y >= SPRITE_HEIGHT){
        addActor(new BorderLine(IID_YELLOW_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH/2, new_border_y, 0, 2.0, 2, -4, this));
        addActor(new BorderLine(IID_YELLOW_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH/2, new_border_y, 0, 2.0, 2, -4, this));
    }
    if (delta_y >= 4*SPRITE_HEIGHT){
        m_listOfActors.push_front(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 2 + ROAD_WIDTH/3, new_border_y, 0, 2.0, 2, -4, this));
        m_listOfActors.push_front(new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 2 - ROAD_WIDTH/3, new_border_y, 0, 2.0, 2, -4, this));
    }
    
    //ZombieCab
    int chanceVehicle = max(100 - getLevel()*10, 20);
    if (randInt(0, chanceVehicle-1) == 0){
        //choose random lane to start evaluating (0 is left, 1 is middle, 2 is right)
        int cur_lane = randInt(0, 2);
        int chosen_lane = -1;   //no chosen lane yet [0-2]
        double zombieCabX;
        double zombieCabY;
        int zombieCabvSpeed;
        switch(cur_lane) {
            case 0: //check left lane first->middle->right lanes
                if (getClosestCAW(LEFT_EDGE, LEFT_EDGE + ROAD_WIDTH/3, 0) == nullptr || (getClosestCAW(LEFT_EDGE, LEFT_EDGE + ROAD_WIDTH/3, 0)->getY() > (VIEW_HEIGHT / 3))){
                    chosen_lane = 0;
                    zombieCabX = ROAD_CENTER - ROAD_WIDTH/3;
                    zombieCabY = SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() + randInt(2, 4);
                    break;
                }
                if (getClosestCAW(LEFT_EDGE, LEFT_EDGE + ROAD_WIDTH/3, 255) == nullptr || (getClosestCAW(LEFT_EDGE, LEFT_EDGE + ROAD_WIDTH/3, 255)->getY() < (VIEW_HEIGHT*2/ 3))){
                    chosen_lane = 0;
                    zombieCabX = ROAD_CENTER - ROAD_WIDTH/3;
                    zombieCabY = VIEW_HEIGHT - SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() - randInt(2, 4);
                    break;
                }
                if (getClosestCAW(LEFT_EDGE + ROAD_WIDTH/3, RIGHT_EDGE - ROAD_WIDTH/3, 0) == nullptr || (getClosestCAW(LEFT_EDGE + ROAD_WIDTH/3, RIGHT_EDGE - ROAD_WIDTH/3, 0)->getY() > (VIEW_HEIGHT / 3))){
                    chosen_lane = 1;
                    zombieCabX = ROAD_CENTER;
                    zombieCabY = SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() + randInt(2, 4);
                    break;
                }
                if (getClosestCAW(LEFT_EDGE + ROAD_WIDTH/3, RIGHT_EDGE - ROAD_WIDTH/3, 255) == nullptr || (getClosestCAW(LEFT_EDGE + ROAD_WIDTH/3, RIGHT_EDGE - ROAD_WIDTH/3, 255)->getY() < (VIEW_HEIGHT*2/ 3))){
                    chosen_lane = 1;
                    zombieCabX = ROAD_CENTER;
                    zombieCabY = VIEW_HEIGHT - SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() - randInt(2, 4);
                    break;
                }
                if (getClosestCAW(RIGHT_EDGE - ROAD_WIDTH/3, RIGHT_EDGE, 0) == nullptr || (getClosestCAW(RIGHT_EDGE - ROAD_WIDTH/3, RIGHT_EDGE, 0)->getY() > (VIEW_HEIGHT / 3))){
                    chosen_lane = 2;
                    zombieCabX = ROAD_CENTER + ROAD_WIDTH/3;
                    zombieCabY = SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() + randInt(2, 4);
                    break;
                }
                if (getClosestCAW(RIGHT_EDGE - ROAD_WIDTH/3, RIGHT_EDGE, 255) == nullptr || (getClosestCAW(RIGHT_EDGE - ROAD_WIDTH/3, RIGHT_EDGE, 255)->getY() < (VIEW_HEIGHT*2/ 3))){
                    chosen_lane = 2;
                    zombieCabX = ROAD_CENTER + ROAD_WIDTH/3;
                    zombieCabY = VIEW_HEIGHT - SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() - randInt(2, 4);
                    break;
                }
                break;
            case 1:  //check middle lane first->right->left lanes
                if (getClosestCAW(LEFT_EDGE + ROAD_WIDTH/3, RIGHT_EDGE - ROAD_WIDTH/3, 0) == nullptr || (getClosestCAW(LEFT_EDGE + ROAD_WIDTH/3, RIGHT_EDGE - ROAD_WIDTH/3, 0)->getY() > (VIEW_HEIGHT / 3))){
                    chosen_lane = 1;
                    zombieCabX = ROAD_CENTER;
                    zombieCabY = SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() + randInt(2, 4);
                    break;
                }
                if (getClosestCAW(LEFT_EDGE + ROAD_WIDTH/3, RIGHT_EDGE - ROAD_WIDTH/3, 255) == nullptr || (getClosestCAW(LEFT_EDGE + ROAD_WIDTH/3, RIGHT_EDGE - ROAD_WIDTH/3, 255)->getY() < (VIEW_HEIGHT*2/ 3))){
                    chosen_lane = 1;
                    zombieCabX = ROAD_CENTER;
                    zombieCabY = VIEW_HEIGHT - SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() - randInt(2, 4);
                    break;
                }
                if (getClosestCAW(RIGHT_EDGE - ROAD_WIDTH/3, RIGHT_EDGE, 0) == nullptr || (getClosestCAW(RIGHT_EDGE - ROAD_WIDTH/3, RIGHT_EDGE, 0)->getY() > (VIEW_HEIGHT / 3))){
                    chosen_lane = 2;
                    zombieCabX = ROAD_CENTER + ROAD_WIDTH/3;
                    zombieCabY = SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() + randInt(2, 4);
                    break;
                }
                if (getClosestCAW(RIGHT_EDGE - ROAD_WIDTH/3, RIGHT_EDGE, 255) == nullptr || (getClosestCAW(RIGHT_EDGE - ROAD_WIDTH/3, RIGHT_EDGE, 255)->getY() < (VIEW_HEIGHT*2/ 3))){
                    chosen_lane = 2;
                    zombieCabX = ROAD_CENTER + ROAD_WIDTH/3;
                    zombieCabY = VIEW_HEIGHT - SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() - randInt(2, 4);
                    break;
                }
                if (getClosestCAW(LEFT_EDGE, LEFT_EDGE + ROAD_WIDTH/3, 0) == nullptr || (getClosestCAW(LEFT_EDGE, LEFT_EDGE + ROAD_WIDTH/3, 0)->getY() > (VIEW_HEIGHT / 3))){
                    chosen_lane = 0;
                    zombieCabX = ROAD_CENTER - ROAD_WIDTH/3;
                    zombieCabY = SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() + randInt(2, 4);
                    break;
                }
                if (getClosestCAW(LEFT_EDGE, LEFT_EDGE + ROAD_WIDTH/3, 255) == nullptr || (getClosestCAW(LEFT_EDGE, LEFT_EDGE + ROAD_WIDTH/3, 255)->getY() < (VIEW_HEIGHT*2/ 3))){
                    chosen_lane = 0;
                    zombieCabX = ROAD_CENTER - ROAD_WIDTH/3;
                    zombieCabY = VIEW_HEIGHT - SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() - randInt(2, 4);
                    break;
                }
                break;
            case 2:     //check right lane first->left->middle lanes
                if (getClosestCAW(RIGHT_EDGE - ROAD_WIDTH/3, RIGHT_EDGE, 0) == nullptr || (getClosestCAW(RIGHT_EDGE - ROAD_WIDTH/3, RIGHT_EDGE, 0)->getY() > (VIEW_HEIGHT / 3))){
                    chosen_lane = 2;
                    zombieCabX = ROAD_CENTER + ROAD_WIDTH/3;
                    zombieCabY = SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() + randInt(2, 4);
                    break;
                }
                if (getClosestCAW(RIGHT_EDGE - ROAD_WIDTH/3, RIGHT_EDGE, 255) == nullptr || (getClosestCAW(RIGHT_EDGE - ROAD_WIDTH/3, RIGHT_EDGE, 255)->getY() < (VIEW_HEIGHT*2/ 3))){
                    chosen_lane = 2;
                    zombieCabX = ROAD_CENTER + ROAD_WIDTH/3;
                    zombieCabY = VIEW_HEIGHT - SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() - randInt(2, 4);
                    break;
                }
                if (getClosestCAW(LEFT_EDGE, LEFT_EDGE + ROAD_WIDTH/3, 0) == nullptr || (getClosestCAW(LEFT_EDGE, LEFT_EDGE + ROAD_WIDTH/3, 0)->getY() > (VIEW_HEIGHT / 3))){
                    chosen_lane = 0;
                    zombieCabX = ROAD_CENTER - ROAD_WIDTH/3;
                    zombieCabY = SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() + randInt(2, 4);
                    break;
                }
                if (getClosestCAW(LEFT_EDGE, LEFT_EDGE + ROAD_WIDTH/3, 255) == nullptr || (getClosestCAW(LEFT_EDGE, LEFT_EDGE + ROAD_WIDTH/3, 255)->getY() < (VIEW_HEIGHT*2/ 3))){
                    chosen_lane = 0;
                    zombieCabX = ROAD_CENTER - ROAD_WIDTH/3;
                    zombieCabY = VIEW_HEIGHT - SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() - randInt(2, 4);
                    break;
                }
                if (getClosestCAW(LEFT_EDGE + ROAD_WIDTH/3, RIGHT_EDGE - ROAD_WIDTH/3, 0) == nullptr || (getClosestCAW(LEFT_EDGE + ROAD_WIDTH/3, RIGHT_EDGE - ROAD_WIDTH/3, 0)->getY() > (VIEW_HEIGHT / 3))){
                    chosen_lane = 1;
                    zombieCabX = ROAD_CENTER;
                    zombieCabY = SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() + randInt(2, 4);
                    break;
                }
                if (getClosestCAW(LEFT_EDGE + ROAD_WIDTH/3, RIGHT_EDGE - ROAD_WIDTH/3, 255) == nullptr || (getClosestCAW(LEFT_EDGE + ROAD_WIDTH/3, RIGHT_EDGE - ROAD_WIDTH/3, 255)->getY() < (VIEW_HEIGHT*2/ 3))){
                    chosen_lane = 1;
                    zombieCabX = ROAD_CENTER;
                    zombieCabY = VIEW_HEIGHT - SPRITE_HEIGHT/2;
                    zombieCabvSpeed = getPointerToGhostRacer()->getVertSpeed() - randInt(2, 4);
                    break;
                }
                break;
        }
        if (chosen_lane != -1){
            addActor(new ZombieCab(IID_ZOMBIE_CAB, zombieCabX, zombieCabY, 90, 4.0, this, 3, zombieCabvSpeed));
        }
    }
    
    //OilSlick
    int chanceOilSlick = max(150 - getLevel()*10, 40);
    int oilSize = randInt(2, 5);
    int oilX = randInt(LEFT_EDGE, RIGHT_EDGE);
    if (randInt(0, chanceOilSlick-1) == 0){
        addActor(new OilSlick(IID_OIL_SLICK, oilX, VIEW_HEIGHT, 0, oilSize, this));
    }
    
    //ZombiePed
    int chanceZombiePed = max(100 - getLevel()*10, 20);
    int zombiePedX = randInt(0, VIEW_WIDTH);
    if (randInt(0, chanceZombiePed-1) == 0){
        addActor(new ZombiePedestrian(IID_ZOMBIE_PED, zombiePedX, VIEW_HEIGHT, 0, 3.0, this, 2));
    }
    
    //HumanPed
    int chanceHumanPed = max(200 - getLevel()*10, 30);
    int humanPedX = randInt(0, VIEW_WIDTH);
    if (randInt(0, chanceHumanPed-1) == 0){
        addActor(new HumanPedestrian(IID_HUMAN_PED, humanPedX, VIEW_HEIGHT, 0, 2.0, this, 2));
    }
    
    //HolyWaterGoodie
    int chanceOfHolyWater = 100 + 10*getLevel();
    int holyWaterX = randInt(LEFT_EDGE, RIGHT_EDGE);
    if (randInt(0, chanceOfHolyWater-1) == 0){
        addActor(new HolyWaterGoodie(IID_HOLY_WATER_GOODIE, holyWaterX, VIEW_HEIGHT, 90, 2.0, this));
    }
    
    //SoulGoodie
    int chanceOfLostSoul = 100;
    int soulGoodieX = randInt(LEFT_EDGE, RIGHT_EDGE);
    if (randInt(0, chanceOfLostSoul-1) == 0){
        addActor(new SoulGoodie(IID_SOUL_GOODIE, soulGoodieX, VIEW_HEIGHT, 0, 4.0, this));
    }
    
    // Update the Game Status Line
    ostringstream status;
    status << "Score: " << getScore() << "  Lvl: " << getLevel() << "  Souls2Save: " << m_numSoulsToSave << "  Lives: " << getLives() << "  Health: " << getPointerToGhostRacer()->getHitPoints() << "  Sprays: " << getPointerToGhostRacer()->getNumSprays() << "  Bonus: " << m_bonus;
    setGameStatText(status.str());
    
    // the player hasn’t completed the current level and hasn’t died, so
    // continue playing the current level
    m_bonus--;
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //delete GhostRacer
    delete m_pointerToGhostRacer;
    
    //iterates through the list container, deleting each Actor
    for (list<Actor*>::iterator p = m_listOfActors.begin(); p != m_listOfActors.end(); p++){
        delete (*p);
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

// Add an actor to the world (works for all actors except Ghost Rider and White BLs).
void StudentWorld::addActor(Actor* a){
    m_listOfActors.push_back(a);    //push all actors to back of list EXCEPT FOR WHITE BORDER LINES (push those manually to front!)
}

// Record that a soul was saved.
void StudentWorld::recordSoulSaved(){
    m_numSoulsToSave--;
}

// If actor a overlaps some live actor that is affected by a holy water
// projectile, inflict a holy water spray on that actor and return true;
// otherwise, return false.  (See Actor::beSprayedIfAppropriate.)
bool StudentWorld::sprayFirstAppropriateActor(Actor* a){
    for (list<Actor*>::iterator p = m_listOfActors.begin(); p != m_listOfActors.end(); p++){
        if (overlaps(a, (*p)) && ((*p)->beSprayedIfAppropriate())){
            return true;
        }
    }
    return false;
}

// Return true if actor a1 overlaps actor a2, otherwise false.
bool StudentWorld::overlaps(const Actor* a1, const Actor* a2) const {
    double delta_x = abs(a1->getX() - a2->getX());
    double delta_y = abs(a1->getY() - a2->getY());
    double radius_sum = a1->getRadius() + a2->getRadius();
    if (delta_x < radius_sum*.25 && delta_y < radius_sum*.6){
        return true;
    }
    return false;
}

// If actor a overlaps this world's GhostRacer, return a pointer to the
// GhostRacer; otherwise, return nullptr
GhostRacer* StudentWorld::getOverlappingGhostRacer(Actor* a) const {
    if (overlaps(a, getPointerToGhostRacer())){
        return getPointerToGhostRacer();
    }
    return nullptr;
}

//Returns pointer to the closest collision a-w actor to y input (in lane specified by [leftX, rightX)); returns nullptr if no such actor
Actor* StudentWorld::getClosestCAW(int leftX, int rightX, int y){
    Actor* closestActor = nullptr;
    //check GhostRacer first
    if (getPointerToGhostRacer()->getX() >= leftX && getPointerToGhostRacer()->getX() < rightX){
        closestActor = getPointerToGhostRacer();
    }
    //check rest of actors
    for (list<Actor*>::iterator p = m_listOfActors.begin(); p != m_listOfActors.end(); p++){
        if ((*p)->isCollisionAW() && (*p)->getX() >= leftX && (*p)->getX() < rightX){
            if (closestActor == nullptr){
                closestActor = (*p);
            } else {
                int curDeltaY = abs((*p)->getY() - y);
                int lastDeltaY = abs(closestActor->getY() - y);
                if (curDeltaY < lastDeltaY){
                    closestActor = (*p);
                }
            }
        }
    }
    return closestActor;
}

//Returns pointer to CAW actor in specified lane in FRONT of/BEHIND and closest to specified y, nullptr if none
Actor* StudentWorld::getClosestCAWinFrontOrBehind(int leftX, int rightX, int y, bool front){
    Actor* closestActor = nullptr;
    if (front){
        //check GhostRacer first
        if (getPointerToGhostRacer()->getX() >= leftX && getPointerToGhostRacer()->getX() < rightX && getPointerToGhostRacer()->getY() > y){
            closestActor = getPointerToGhostRacer();
        }
        //check rest of actors
        for (list<Actor*>::iterator p = m_listOfActors.begin(); p != m_listOfActors.end(); p++){
            if ((*p)->isCollisionAW() && (*p)->getX() >= leftX && (*p)->getX() < rightX && (*p)->getY() > y){
                if (closestActor == nullptr){
                    closestActor = (*p);
                } else {
                    int curDeltaY = abs((*p)->getY() - y);
                    int lastDeltaY = abs(closestActor->getY() - y);
                    if (curDeltaY < lastDeltaY){
                        closestActor = (*p);
                    }
                }
            }
        }
        return closestActor;
    } else {    //check behind
        //check GhostRacer first
        if (getPointerToGhostRacer()->getX() >= leftX && getPointerToGhostRacer()->getX() < rightX && getPointerToGhostRacer()->getY() < y){
            closestActor = getPointerToGhostRacer();
        }
        //check rest of actors
        for (list<Actor*>::iterator p = m_listOfActors.begin(); p != m_listOfActors.end(); p++){
            if ((*p)->isCollisionAW() && (*p)->getX() >= leftX && (*p)->getX() < rightX && (*p)->getY() < y){
                if (closestActor == nullptr){
                    closestActor = (*p);
                } else {
                    int curDeltaY = abs((*p)->getY() - y);
                    int lastDeltaY = abs(closestActor->getY() - y);
                    if (curDeltaY < lastDeltaY){
                        closestActor = (*p);
                    }
                }
            }
        }
        return closestActor;
    }
}
