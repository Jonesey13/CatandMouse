#ifndef TRAP_H
#define TRAP_H



#include "car.h"

class TrapData{
public:
    Vector2u Square;
    bool Half;
    Vector2u Types;
};

class Trap{
public:
    vector<Vector2u> SwitchPositions;
    vector<TrapData> trapData;
    set<unsigned> SwitchPlayers;
    bool MainSwitch=0;
    vector<pair<Vector2d,Vector2d>> Wiring;
    void PlayerInteract(Car& Player, unsigned PlayerNumber);
};

#endif
