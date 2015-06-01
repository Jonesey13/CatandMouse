#ifndef CAR_H
#define CAR_H

#include "functions.h"



class Car
{
public:
    constexpr static double Wall_Elasticity=0.6;
    constexpr static double Elasticity=1;
    constexpr static double AngularElasticity=0.5;
    constexpr static double DeathDuration=1;
    vector<Vector2d> Bounding={Vector2d(-0.375,-0.25),Vector2d(-0.375,0.25),Vector2d(0.375,0.25), Vector2d(0.375,-0.25)};
    double DrivingForce=10;
    double Braking=5;
    double Reversing=2;
    double RollingFriction=1;
    double SlidingFriction=5;
    double AngularFriction=5;

    Vector2d Velocity=Vector2d();
    double Rotation=0;
    Vector2d Position=Vector2d();
    double Angular=0;

    double TurningRate=0.5;
    double Traction=5;
    bool VelocitySwitch=false;
    bool BrakeSwitch=false;
    bool RPlusSwitch=false;
    bool RMinusSwitch=false;

    bool DeathSwitch=false;
    double DeathTime=0;
    Vector2d PositionBeforeDeath=Vector2d(0,0);
    bool RequestReset=0;

    void Update(double DeltaTime);
    void DeathUpdate(double Deltatime);
    void NormalTraction();
    void SlidingTraction();
    Vector2u getGridPosition(){return Vector2u(Position.x,Position.y);};

    int LapNumber=0;
    int LapFlag=0;
};

#endif // CAR_H
