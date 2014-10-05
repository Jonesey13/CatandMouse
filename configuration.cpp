#include "configuration.h"

Configuration::Configuration(){
    ButtonsToActions.resize(8);
    for (unsigned i=0;i<8;i++)
    {
        ButtonsToActions[i][0]=ACCELERATE;
        ButtonsToActions[i][1]=BRAKE;
    }
}

void Configuration::Load(){
    ifstream inputstream("ConfigData.txt",ios::binary);
    string Current;
    while(inputstream>>Current){
        ConfigStorable *Item=nullptr;
        ConfigUnsigned UnsignedData;
        ConfigController ControllerData;
        if (Current=="NumberOfLaps:")
        {
            UnsignedData=ConfigUnsigned("NumberOfLaps",*this);
            Item=&UnsignedData;
        }
        if (Current=="ControllerSettings:")
        {
            ControllerData=ConfigController("Controller",*this);
            Item=&ControllerData;
        }
        if(Item){
            Item->ReadFromStream(inputstream);
        }
    }
}



void Configuration::Save(){
    ofstream outputstream("ConfigData.txt");
    ConfigUnsigned UnsignedData=ConfigUnsigned("NumberOfLaps",*this);
    UnsignedData.WriteToStream(outputstream);
    ConfigController ControllerData=ConfigController("Controller",*this);
    ControllerData.WriteToStream(outputstream);
}
