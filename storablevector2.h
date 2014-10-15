
#ifndef STORABLEVECTOR2_H
#define STORABLEVECTOR2_H

template <typename T>
using Vector2=sf::Vector2<T>;


template <typename T>
class StorableVector2 : public Storable {
public:
    ~StorableVector2()=default;
    StorableVector2()=default;
    StorableVector2(string NewSig, Vector2<T> &NewTarget): Storable(NewSig), Target(&NewTarget){};
    Vector2<T> *Target;
    void ReadFromStream(ifstream &inputstream) override;
    void WriteToStream(ofstream &outputstream) override;

};


template <typename T>
void StorableVector2<T>::ReadFromStream(ifstream &inputstream){
    inputstream>>Target->x>>Target->y;
}

template <typename T>
void StorableVector2<T>::WriteToStream(ofstream &outputstream){
    outputstream<<Sig<<" "<<Target->x<<" "<<Target->y<<"\n";
}


#endif //STORABLEVECTOR2_H




