
#ifndef CONFIGUNSIGNED_H
#define CONFIGUNSIGNED_H

#include <string>

class ConfigUnsigned : public ConfigStorable {
public:
    ~ConfigUnsigned()=default;
    ConfigUnsigned()=default;
    ConfigUnsigned(string NewSig,Configuration &NewConfig);
    unsigned *Target;
    void ReadFromStream(ifstream &inputstream) override;
    void WriteToStream(ofstream &outputstream) override;




};


#endif //CONFIGUNSIGNED_H
