

#ifndef STORABLETRAPLIST_H
#define STORABLETRAPLIST_H

#include <vector>
#include "trap.h"

class StorableTrapList : public Storable {
public:
    ~StorableTrapList()=default;
    StorableTrapList()=default;
    StorableTrapList(string NewSig,vector<Trap> &NewTarget): Storable(NewSig), Target(&NewTarget){};
    vector<Trap> *Target;
    void ReadFromStream(ifstream &inputstream) override;
    void WriteToStream(ofstream &outputstream) override;
};


#endif //STORABLETRAPLIST_H





