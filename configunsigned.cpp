#include "configuration.h"

ConfigUnsigned::ConfigUnsigned(string NewSig,Configuration &NewConfig): ConfigStorable(NewSig,NewConfig){
    if (Sig=="NumberOfLaps")
    {
        Target=&Config->NumberOfLaps;
    }
}

void ConfigUnsigned::ReadFromStream(ifstream &inputstream){
    if (Target)
    {
        inputstream>>*Target;
    }
}

void ConfigUnsigned::WriteToStream(ofstream &outputstream){
    if (Sig=="NumberOfLaps")
    {
        outputstream<<"NumberOfLaps: "<<*Target<<"\n";
    }
}

