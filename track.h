#ifndef TRACK_H
#define TRACK_H


#include <iostream>
#include <fstream>
#include <sstream>

#include "tile.h"
#include "storable.h"



class Track
{
    friend class TrackPairUnsigned;
    friend class TrackTileMap;
    vector<vector<Tile>> Tiles;
    Vector2u Dim;

public:
    Track();
    void init(unsigned TrackNumber);
    vector<Vector2u> StartingPositions=vector<Vector2u>(8,Vector2u(0,0));
    Vector2u getDim();
    Tile *getTile(unsigned i, unsigned j);
    bool TrackReady=0;
    unsigned FinishDirection=0;
    unsigned TotalStarting=4;

    vector<Vector2d> getTileBounding(unsigned i, unsigned j, bool TriangleHalf=0);

    void SetBlank(sf::Vector2u NewDim);
    void ReadTrack(unsigned TrackNumber);
    void WriteTrack(unsigned TrackNumber);
    void FlushTrack(unsigned TrackNumber);
    void RefreshDetection();
    bool getFinishSquareHalf(Vector2d Position);
};





#endif // TRACK_H




