

#ifndef STORABLEVECTORVECTOR2_H
#define STORABLEVECTORVECTOR2_H

#include <vector>

template <typename T>
using Vector2=sf::Vector2<T>;

template <typename T>
class StorableVectorVector2 : public Storable {
public:
    ~StorableVectorVector2()=default;
    StorableVectorVector2()=default;
    StorableVectorVector2(string NewSig,vector<Vector2<T>> &NewTarget): Storable(NewSig), Target(&NewTarget){};
    vector<Vector2<T>> *Target;
    void ReadFromStream(ifstream &inputstream) override;
    void WriteToStream(ofstream &outputstream) override;
};

template <typename T>
void StorableVectorVector2<T>::ReadFromStream(ifstream &inputstream){
    string InputString;
    Target->clear();
    Vector2<T> InputT;
    while (!inputstream.eof())
    {
        streampos StepBack=inputstream.tellg();
        inputstream>>InputString;
        if(InputString=="----")
        {
            break;
        }
        inputstream.seekg(StepBack);
        inputstream>>InputT.x>>InputT.y;
        Target->push_back(InputT);
    }
}

template <typename T>
void StorableVectorVector2<T>::WriteToStream(ofstream &outputstream){
    outputstream<<Sig<<" ";
    for (auto index: *Target)
    {
        outputstream<<index.x<<" "<<index.y<<"\n";
    }
    outputstream<<"----"<<"\n"<<"\n";
}

#endif //STORABLEVECTOR_H




