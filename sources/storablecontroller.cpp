#include "storable.h"


void StorableController::ReadFromStream(ifstream &inputstream){
    for (unsigned j=0; j<8; j++)
    {
        (*JoyId)[j]=-1;
    }
    ButtonsToNumbers->resize(8);
    for (auto &index: (*ButtonsToNumbers))
    {
        index.clear();
    }

    unsigned InputUnsigned, ButtonNumber, ActionNumber;
    string InputString;
    for (unsigned i=0; i<8; i++)
    {
        ostringstream convert;
        inputstream>>InputString;
        convert<<"Player"<<i+1<<":";
        if (InputString!=convert.str())
        {
            throw runtime_error("A Player is Missing!");
        }
        inputstream>>InputString;
        if(InputString!="NoJoystick:")
        {
            inputstream>>InputUnsigned;
            (*JoyId)[InputUnsigned]=i;
        }
        while(!inputstream.eof())
        {
            streampos StepBack=inputstream.tellg();
            inputstream>>InputString;
            if (InputString=="----")
            {
                break;
            }
            inputstream.seekg(StepBack);
            inputstream>>ButtonNumber>>ActionNumber;
            (*ButtonsToNumbers)[i][ButtonNumber]=ActionNumber;
        }
    }
}

void StorableController::WriteToStream(ofstream &outputstream){
    outputstream<<Sig<<" ";
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


        for (auto index: (*ButtonsToNumbers)[i])
        {
            outputstream<<index.first<<"   "<<index.second<<"\n";
        }
        outputstream<<"----"<<"\n";
    }
    outputstream<<"\n";
}

