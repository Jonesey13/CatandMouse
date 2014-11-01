#include "storable.h"


void StorableTrapList::ReadFromStream(ifstream &inputstream){
    string InputString;
    Target->clear();

    while (!inputstream.eof())
    {
        streampos StepBack=inputstream.tellg();
        inputstream>>InputString;
        if(InputString=="--EndTraps--")
        {
            break;
        }
        inputstream.seekg(StepBack);
        inputstream>>InputString>>InputString;
        Trap trap;
        StorableSingle<bool> MainSwitch("MainSwitch:",trap.MainSwitch);
        StorableVectorVector2<unsigned> SwitchPos("SwitchPositions:",trap.SwitchPositions);
        while (InputString!="--EndTrap--")
        {
            inputstream>>InputString;
            if (InputString=="MainSwitch:")
                MainSwitch.ReadFromStream(inputstream);
            if (InputString=="SwitchPositions:")
                SwitchPos.ReadFromStream(inputstream);


            if (InputString=="TrapData:")
            {
                while (!inputstream.eof())
                {
                    TrapData trapData;
                    StepBack=inputstream.tellg();
                    inputstream>>InputString;
                    if(InputString=="----")
                    {
                        break;
                    }
                    inputstream.seekg(StepBack);
                    inputstream>>trapData.Square.x>>trapData.Square.y>>InputString
                                >>trapData.Half>>InputString>>trapData.Types.x>>trapData.Types.y;
                    trap.trapData.push_back(trapData);
                }
            }

            if (InputString=="Wiring:")
            {
                while (!inputstream.eof())
                {
                    pair<Vector2d,Vector2d> wires;
                    StepBack=inputstream.tellg();
                    inputstream>>InputString;
                    if(InputString=="----")
                    {
                        break;
                    }
                    inputstream.seekg(StepBack);
                    inputstream>>wires.first.x>>wires.first.y>>InputString
                                >>wires.second.x>>wires.second.y;
                    trap.Wiring.push_back(wires);
                }
            }

        }
        Target->push_back(trap);
    }
}

void StorableTrapList::WriteToStream(ofstream &outputstream){
    outputstream<<Sig<<" ";
    unsigned TrapNumber=1;
    for (Trap trap: *Target)
    {
        outputstream<<"Trap "<<TrapNumber<<": "<<"\n";
        StorableSingle<bool> MainSwitch("MainSwitch:",trap.MainSwitch);
        StorableVectorVector2<unsigned> SwitchPos("SwitchPositions:",trap.SwitchPositions);
        MainSwitch.WriteToStream(outputstream);
        SwitchPos.WriteToStream(outputstream);

        outputstream<<"TrapData: ";
        for (unsigned i=0; i<trap.trapData.size();i++)
        {
            outputstream<<trap.trapData[i].Square.x<<" "<<trap.trapData[i].Square.y
            <<" : "<<trap.trapData[i].Half<<" : "<<trap.trapData[i].Types.x<<" "<<trap.trapData[i].Types.y<<" \n";
        }
        outputstream<<"----"<<"\n"<<"\n";

        outputstream<<"Wiring: ";
        for (unsigned i=0; i<trap.Wiring.size();i++)
        {
            outputstream<<trap.Wiring[i].first.x<<" "<<trap.Wiring[i].first.y
            <<" : "<<trap.Wiring[i].second.x<<" "<<trap.Wiring[i].second.y<<" \n";
        }
        outputstream<<"----"<<"\n"<<"\n";

        outputstream<<"--EndTrap--"<<"\n"<<"\n";
        TrapNumber++;
    }
    outputstream<<"--EndTraps--"<<"\n"<<"\n";
}
