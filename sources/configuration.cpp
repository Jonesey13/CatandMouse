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
    ifstream InputStream("ConfigData.txt",ios::binary);
    string InputString;
    StorableSingle<unsigned> LapData("NumberOfLaps:",NumberOfLaps);
    vector<map<unsigned,unsigned>> ButtonsToNumbers;
    StorableController ControllerData("ControllerSettings:",JoyId,ButtonsToNumbers);

    while(InputStream>>InputString){
        Storable *Item=nullptr;
        if (InputString=="NumberOfLaps:")
        {
            Item=&LapData;
        }
        if (InputString=="ControllerSettings:")
        {
            Item=&ControllerData;
        }
        if(Item){
            Item->ReadFromStream(InputStream);
        }
    }

    for (unsigned i=0; i<8;i++)
    {
        for(auto index : ButtonsToNumbers[i])
            ButtonsToActions[i][index.first]=static_cast<Action>(index.second);
    }

}



void Configuration::Save(){
    ofstream OutputStream("ConfigData.txt");
    vector<Storable*> storables;
    StorableSingle<unsigned> LapData("NumberOfLaps:", NumberOfLaps);
    storables.push_back(&LapData);

    vector<map<unsigned,unsigned>> ButtonsToNumbers;
    ButtonsToNumbers.resize(8);
    for (unsigned i=0; i<8;i++)
    {
        for(auto index : ButtonsToActions[i])
            ButtonsToNumbers[i][index.first]=index.second;
    }
    StorableController ControllerData("ControllerSettings:",JoyId,ButtonsToNumbers);
    storables.push_back(&ControllerData);

    for(auto index: storables)
    {
        index->WriteToStream(OutputStream);
    }

}
