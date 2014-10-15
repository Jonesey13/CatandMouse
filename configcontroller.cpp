
#include "configuration.h"

ConfigController::ConfigController(string NewSig,Configuration &NewConfig): Storable(NewSig){
    Config=&NewConfig;
    ButtonsToActions=&Config->ButtonsToActions;
    JoyId=&Config->JoyId;
}

void ConfigController::ReadFromStream(ifstream &inputstream){
    unsigned i=0;
    unsigned registered=0;
    unsigned total=2;
    unsigned number;
    for (unsigned j=0; j<8; j++)
    {
        (*JoyId)[j]=-1;
    }

    string text;
    inputstream>>text;
    inputstream>>text;
    if(text!="NoJoystick")
    {
        inputstream>>text;
        istringstream (text)>>number;
        (*JoyId)[number]=i;
    }
    for (auto &index: (*ButtonsToActions))
    {
        index.clear();
    }


    while(i<8)
    {
        bool found=0;
        ostringstream convert;
        inputstream>>text;
        istringstream (text)>>number;
        inputstream>>text;
        if (text=="Accelerate")
        {
            (*ButtonsToActions)[i][number]=Action::ACCELERATE;
            registered++;
            found=1;
        }
        if (text=="Brake")
        {
            (*ButtonsToActions)[i][number]=Action::BRAKE;
            registered++;
            found=1;
        }
        if(found==0)
        {
            throw runtime_error("Missing Full Set of Buttons!");
        }
        if(registered==total)
        {
            registered=0;
            i++;
            if(i!=8)
            {
                inputstream>>text;
                convert<<"Player"<<i+1<<":";
                if (text!=convert.str())
                {
                    throw runtime_error("A Player is Missing!");
                }
                inputstream>>text;
                if(text!="NoJoystick:")
                {
                    inputstream>>text;
                    istringstream (text)>>number;
                    (*JoyId)[number]=i;
                }
            }
        }
    }

}

void ConfigController::WriteToStream(ofstream &outputstream){
    outputstream<<"ControllerSettings: ";
    for (unsigned i=0; i<8; i++)
    {
        bool found=0;
        for (unsigned j=0; j<JoyId->size();j++)
        {
            if ((*JoyId)[j]==static_cast<int>(i))
            {
                outputstream<<"Player"<<i+1<<": "<<"Joystick: "<<j<<"\n";
                found=1;
            }
        }
        if (found==0){
            outputstream<<"Player"<<i+1<<": "<<"NoJoystick: "<<"\n";
        }


        for (auto index: (*ButtonsToActions)[i])
        {
            switch (index.second)
            {
            case Action::ACCELERATE:
            {
               outputstream<<index.first<<"   "<<"Accelerate"<<"\n";
               break;
            }
            case Action::BRAKE:
            {
               outputstream<<index.first<<"   "<<"Brake"<<"\n";
               break;
            }
            default:
                break;
            }
        }
    }
}

