#ifndef RACE_H
#define RACE_H

#include "car.h"
#include "track.h"
#include "configuration.h"

class Race
{
public:
    Configuration Config;
    Track track;
    vector<Car> Player;
    Race();
    bool Victory=0;
    unsigned VictorNumber=0;

    void init(Configuration &NewConfig);
    void Update(double DeltaTime);
    void HandleWallCollisions();
    void HandleDeaths();
    Vector2u getNearestValidSquare(unsigned PlayerNumber);
    void ResetCar(unsigned PlayerNumber);
    void HandleSingleWallCollision(Car &Racer, Vector2d Direction, double Overlap,
                                   unsigned Attempts, Vector2d AltVelocityChange=Vector2d(0,0));
    void HandlePlayerOnPlayerCollisions();
    void HandleSinglePlayerCollision(vector<Car*> Cars, vector<vector<Vector2d>> Bounding, bool CornerCar, Vector2d CollisionPos,
                                     Vector2d Direction, double Overlap, bool DoubleFlag, unsigned Attempts);
    void UpdateTraction();
    void UpdateLaps();
};










#endif // RACE_H
