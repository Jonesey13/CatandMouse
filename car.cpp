#include "car.h"



void Car::NormalTraction(){
    RollingFriction=1;
    SlidingFriction=5;
    Traction=0.5;
}

void Car::SlidingTraction(){
    RollingFriction=0.2;
    SlidingFriction=1;
    Traction=0.1;
}


void Car::Update(double DeltaTime){
    Vector2d RollingDirection=RotateVector(Vector2d(1,0), Rotation);
    Vector2d SlidingDirection=RotateVector(Vector2d(0,1), Rotation);
    double Rolling=DotProduct(Velocity,RollingDirection);
    double Sliding=DotProduct(Velocity,SlidingDirection);
    Rolling=Rolling-RollingFriction*Rolling*DeltaTime;
    if (abs(Sliding)>abs(SlidingFriction*DeltaTime))
    {
    Sliding=Sliding-SlidingFriction*copysign(1,Sliding)*DeltaTime;
    }
    else{
        Sliding=0;
    }
    Velocity=Rolling*RollingDirection+Sliding*SlidingDirection;

    if (abs(Angular)>abs(AngularFriction*DeltaTime))
    {
        Angular=Angular-AngularFriction*Angular*DeltaTime;
    }
    else{
        Angular=0;
    }
    Rotation=Rotation+Angular*DeltaTime;
    Position=Position+Velocity*DeltaTime;

    // Update variables according to button presses
    if (VelocitySwitch==1)
    {
        Velocity=Velocity+DrivingForce*DeltaTime*RollingDirection;
    }
    if (BrakeSwitch==1)
    {
        Rolling=DotProduct(Velocity,RollingDirection);
        Velocity=Velocity-Rolling*RollingDirection;
        if (Rolling>=0)
        {
            Rolling=Rolling-Braking*DeltaTime;
        }
        else
        {
            Rolling=Rolling-Reversing*DeltaTime;
        }
        Velocity=Velocity+Rolling*RollingDirection;
    }
    Rolling=DotProduct(Velocity,RollingDirection);
    Vector2d OldRolling=Rolling*RollingDirection;
    if (RMinusSwitch==1)
    {
        Rotation=Rotation-TurningRate*DeltaTime;
    }
    if (RPlusSwitch==1)
    {
        Rotation=Rotation+TurningRate*DeltaTime;
    }

    RollingDirection=RotateVector(Vector2d(1,0), Rotation);
    Vector2d NewRolling=Rolling*RollingDirection;
    Vector2d RollingDifference=NewRolling-OldRolling;
    double RollingChange=sqrt(DotProduct(RollingDifference,RollingDifference))/DeltaTime;
    if (RollingChange<Traction)
    {
        Velocity=Velocity-OldRolling;
        Velocity=Velocity+NewRolling;
    }
    else
    {
        Velocity=Velocity-Traction/RollingChange*OldRolling;
        Velocity=Velocity+Traction/RollingChange*NewRolling;
    }

}

void Car::DeathUpdate(double DeltaTime){
    Position+=DeltaTime*Velocity;
    if (abs(Angular)>abs(AngularFriction*DeltaTime))
    {
        Angular=Angular-AngularFriction*Angular*DeltaTime;
    }
    else{
        Angular=0;
    }
    Rotation=Rotation+Angular*DeltaTime;
    DeathTime+=DeltaTime;
    if(DeathTime>DeathDuration)
    {
        DeathSwitch=0;
        DeathTime=0;
        RequestReset=1;
    }
}



