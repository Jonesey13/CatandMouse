#ifndef TRAP_H
#define TRAP_H



#include "car.h"

class Trap{
public:
    vector<Vector2u> SwitchPositions;
    vector<Vector2u> TrapPositions;
    vector<Vector2u> TrapOrientations;
    vector<Vector2u> TrapTypesOn;
    vector<Vector2u> TrapTypesOff;
    set<unsigned> SwitchPlayers;
    bool MainSwitch=0;

    void PlayerInteract(Car& Player, unsigned PlayerNumber);
};

#endif
