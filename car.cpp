#include "header.h"

Car::Car(){
    DrivingForce=200;
    Braking=50;
    Reversing=20;
    Velocity=Vector2d();
    Rotation=0;
    Position=Vector2d();
    TurningRate=0.5;
    VelocitySwitch=false;
    BrakeSwitch=false;
    RPlusSwitch=false;
    RMinusSwitch=false;
}

sf::Texture Car::Texture=sf::Texture();
unsigned Car::Size=0;

void Car::init(){
    Texture.loadFromFile("CarSprites.png");
    Size=32;
}


sf::Texture *Car::getTexture(){
    return &Texture;
}

unsigned Car::getSize(){
    return Size;
}

void Car::Update(double DeltaTime){
    Vector2d RollingDirection=RotateVector(Vector2d(1,0), Rotation);
    //sf::Vector2f SlidingDirection=RotateVector(sf::Vector2f(0,1), Rotation);

    Position=Position+Velocity*DeltaTime;

    // Update variables according to button presses
    if (VelocitySwitch==1)
    {
        Velocity=Velocity+DrivingForce*DeltaTime*RollingDirection;
    }
    if (BrakeSwitch==1)
    {
        double Rolling=DotProduct(Velocity,RollingDirection);
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
    if (RMinusSwitch==1)
    {
        Rotation=Rotation-TurningRate*DeltaTime;
    }
    if (RPlusSwitch==1)
    {
        Rotation=Rotation+TurningRate*DeltaTime;
    }
}

