
#ifndef CONFIGCONTROLLER_H
#define CONFIGCONTROLLER_H
class Configuration;

class ConfigController: public Storable
{
public:
    ~ConfigController()=default;
    ConfigController()=default;
    ConfigController(string NewSig,Configuration &NewConfig);
    vector<map<unsigned,Action>> *ButtonsToActions;
    vector<int> *JoyId;
    Configuration *Config;
    void ReadFromStream(ifstream &inputstream) override;
    void WriteToStream(ofstream &outputstream) override;
};


#endif //CONFIGCONTROLLER_H

