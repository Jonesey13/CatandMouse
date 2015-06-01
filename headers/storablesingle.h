
#ifndef STORABLESINGLE_H
#define STORABLESINGLE_H


template <typename T>
class StorableSingle : public Storable {
public:
    ~StorableSingle()=default;
    StorableSingle()=default;
    StorableSingle(string NewSig,T &NewTarget): Storable(NewSig), Target(&NewTarget){};
    T *Target;
    void ReadFromStream(ifstream &inputstream) override;
    void WriteToStream(ofstream &outputstream) override;

};


template <typename T>
void StorableSingle<T>::ReadFromStream(ifstream &inputstream){
    inputstream>>*Target;
}

template <typename T>
void StorableSingle<T>::WriteToStream(ofstream &outputstream){
    outputstream<<Sig<<" "<<*Target<<"\n";
}


#endif //STORABLESINGLE_H



