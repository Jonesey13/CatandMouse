#include "trap.h"

void Trap::PlayerInteract(Car& Player, unsigned PlayerNumber){
    Vector2u CurrentSquare=Player.getGridPosition();

    // Notice this format allows multiple cars to run over multiple switches
    if (any_of(SwitchPositions.begin(),SwitchPositions.end(), [&](Vector2u k){return k==CurrentSquare;})
    && none_of(SwitchPlayers.begin(),SwitchPlayers.end(), [&](unsigned k){return k==PlayerNumber;}))
    {
        SwitchPlayers.insert(PlayerNumber);
        MainSwitch=!MainSwitch;
    }
    if (none_of(SwitchPositions.begin(),SwitchPositions.end(), [&](Vector2u k){return k==CurrentSquare;})
    && any_of(SwitchPlayers.begin(),SwitchPlayers.end(), [&](unsigned k){return k==PlayerNumber;}))
    {
        SwitchPlayers.erase(PlayerNumber);
    }
}

