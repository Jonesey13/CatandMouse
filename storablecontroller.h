
#ifndef STORABLECONTROLLER_H
#define STORABLECONTROLLER_H



class StorableController: public Storable
{
public:
    ~StorableController()=default;
    StorableController()=default;
    StorableController(string NewSig, vector<int> &NewJoyId, vector<map<unsigned,unsigned>> &NewButtonsToNumbers)
    :Storable(NewSig), JoyId(&NewJoyId), ButtonsToNumbers(&NewButtonsToNumbers){};
    vector<int> *JoyId;
    vector<map<unsigned,unsigned>> *ButtonsToNumbers;

    void ReadFromStream(ifstream &inputstream) override;
    void WriteToStream(ofstream &outputstream) override;
};


#endif //STORABLECONTROLLER_H


