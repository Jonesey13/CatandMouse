#ifndef STORABLETILEMAP_H
#define STORABLETILEMAP_H

#include "tile.h"

class StorableTileMap : public Storable {
public:
    ~StorableTileMap()=default;
    StorableTileMap()=default;
    StorableTileMap(string NewSig,vector<vector<Tile>> &NewTarget): Storable(NewSig), Target(&NewTarget){};
    vector<vector<Tile>> *Target;
    void ReadFromStream(ifstream &inputstream) override;
    void WriteToStream(ofstream &outputstream) override;

};


#endif //STORABLETILEMAP_H
