#include "header.h"


Car::Car(){
    DrivingForce=200;
    Braking=50;
    Reversing=20;
    Velocity=sf::Vector2f();
    Rotation=0;
    Position=sf::Vector2f();
    TurningRate=0.5;
    VelocitySwitch=false;
    BrakeSwitch=false;
    RPlusSwitch=false;
    RMinusSwitch=false;
}

sf::Vector2f Car::getVelocity(){
    return Velocity;
}


void Car::setVelocity(sf::Vector2f NewVelocity){
    Velocity=NewVelocity;
}


float Car::getRotation(){
    return Rotation;
}

void Car::setRotation(float NewRotation){
    Rotation=NewRotation;
    }

sf::Vector2f Car::getPosition(){
    return Position;
}

void Car::setPosition(sf::Vector2f NewPosition){
    Position=NewPosition;
}

sf::Texture Car::Texture=sf::Texture();
int Car::Size=0;

void Car::initTexture(){
    Texture.loadFromFile("CarSprites.png");
    Size=32;
}


sf::Texture* Car::getTexture(){
return &Texture;
}

int Car::getSize(){
return Size;
}

void Car::Update(float DeltaTime){
    sf::Vector2f RollingDirection=RotateVector(sf::Vector2f(1,0), Rotation);
    //sf::Vector2f SlidingDirection=RotateVector(sf::Vector2f(0,1), Rotation);

    Position=Position+Velocity*DeltaTime;

    // Update variables according to button presses
    if (VelocitySwitch==1)
    {
        Velocity=Velocity+(float)(DrivingForce*DeltaTime)*RollingDirection;
    }
    if (BrakeSwitch==1)
    {
        float Rolling=DotProduct(Velocity,RollingDirection);
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
    setVelocity(Velocity);
}


