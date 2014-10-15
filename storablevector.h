
#ifndef STORABLEVECTOR_H
#define STORABLEVECTOR_H

#include <vector>

template <typename T>
class StorableVector : public Storable {
public:
    ~StorableVector()=default;
    StorableVector()=default;
    StorableVector(string NewSig,vector<T> &NewTarget): Storable(NewSig), Target(&NewTarget){};
    vector<T> *Target;
    void ReadFromStream(ifstream &inputstream) override;
    void WriteToStream(ofstream &outputstream) override;
};

template <typename T>
void StorableVector<T>::ReadFromStream(ifstream &inputstream){
    string InputString;
    Target->empty();
    T InputT;
    while (!inputstream.eof())
    {
        streampos StepBack=inputstream.tellg();
        inputstream>>InputString;
        if(InputString=="----")
        {
            break;
        }
        inputstream.seekg(StepBack);
        inputstream>>InputT;
        Target->push_back(InputT);
    }
}

template <typename T>
void StorableVector<T>::WriteToStream(ofstream &outputstream){
    outputstream<<Sig<<" ";
    for (auto index: *Target)
    {
        outputstream<<index<<"\n";
    }
    outputstream<<"----"<<"\n"<<"\n";
}

#endif //STORABLEVECTOR_H




