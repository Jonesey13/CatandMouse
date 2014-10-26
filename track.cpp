#include "track.h"
#include "functions.h"


Track::Track(){
    for (unsigned i=0; i<8;i++)
    {
        StartingPositions.push_back(Vector2u(i,0));
    }
    Traps.resize(10);
}

void Track::init(unsigned TrackNumber)
{
    ReadTrack(TrackNumber);
}


Vector2u Track::getDim(){
    return Dim;
}



Tile* Track::getTile(unsigned i, unsigned j){
    return &Tiles[i][j];
}


void Track::RefreshDetection(){
    vector<set<PROPERTIES>> DetectionList;
    DetectionList.resize(6);
    DetectionList[0]={FALL};
    DetectionList[1]={NORMAL_TRACTION};
    DetectionList[2]={WALL};
    DetectionList[3]={SLIDING_TRACTION};
    DetectionList[4]={NORMAL_TRACTION,FINISH};
    for (unsigned i=0;i<Dim.x;i++)
    {
        for (unsigned j=0;j<Dim.y;j++)
        {
            Tile *CurrentTile=getTile(i,j);
            CurrentTile->Detection.x=DetectionList[CurrentTile->Types.x];
            CurrentTile->Detection.y=DetectionList[CurrentTile->Types.y];
        }
    }
}

vector<Vector2d> Track::getTileBounding(unsigned i, unsigned j, bool TriangleHalf){
    Tile* tile=getTile(i,j);
    vector<Vector2d> Bounding;
    if (tile->isSquare)
    {
        Bounding={Vector2d(i,j),Vector2d(i,j+1),Vector2d(i+1,j+1),Vector2d(i+1,j)};
    }
    else{
        bool Orientation=tile->Orientation;
        if(Orientation==0)
        {
            if (TriangleHalf==0)
            {
                Bounding={Vector2d(i,j),Vector2d(i,j+1),Vector2d(i+1,j+1)};
            }
            else{
                Bounding={Vector2d(i,j),Vector2d(i+1,j+1),Vector2d(i+1,j)};
            }
        }
        else{
            if (TriangleHalf==0)
            {
                Bounding={Vector2d(i,j+1),Vector2d(i+1,j+1),Vector2d(i+1,j)};
            }
            else{
                Bounding={Vector2d(i,j),Vector2d(i,j+1),Vector2d(i+1,j)};
            }
        }
    }
    return Bounding;
 }

 void Track::ReadTrack(unsigned TrackNumber){
    string InputString;
    unsigned InputInt;
    ifstream InputStream("Tracks.txt",ios::binary);
    bool TrackFound=0;
    while (TrackFound==0 && !InputStream.eof())
    {
        InputStream>>InputString;
        if (InputString=="--StartTrack--")
        {
            InputStream>>InputString;
            InputStream>>InputInt;
            if(InputInt==TrackNumber)
            {
                TrackFound=1;
            }
        }
    }
    if (TrackFound==0)
    {
        throw runtime_error("Failed to Find Track!");
    }
    else{
        StorableVector2<unsigned> Dimensions("Dimensions:",Dim);
        StorableSingle<unsigned> FinishingDirection("FinishingDirection:",FinishDirection);
        StorableSingle<unsigned> TotalStartingPlayers("TotalStartingPlayers:",TotalStarting);
        StorableVectorVector2<unsigned> TrackStartingPositions("StartingPositions:",StartingPositions);
        StorableTileMap TileMap("Tiles:",Tiles);
        while (InputString!="--EndTrack--"){
            Storable* storable=nullptr;
            InputStream>>InputString;

            if (InputString=="Dimensions:")
                storable=&Dimensions;
            if (InputString=="FinishingDirection:")
                storable=&FinishingDirection;
            if (InputString=="TotalStartingPlayers:")
                storable=&TotalStartingPlayers;
            if (InputString=="StartingPositions:")
                storable=&TrackStartingPositions;
            if (InputString=="Tiles:")
                storable=&TileMap;


            if (storable)
                storable->ReadFromStream(InputStream);

        }
        TrackReady=1;
    }
    InputStream.close();
 }

void Track::WriteTrack(unsigned TrackNumber){
    ofstream OutputStream("Tracks.txt",ios::app);
    OutputStream<<"\n\n\n--StartTrack--\n\n"<<"TrackNumber: "<< TrackNumber << "\n";
    vector<Storable*> storables;
    StorableVector2<unsigned> Dimensions("Dimensions:",Dim);
    storables.push_back(&Dimensions);
    StorableSingle<unsigned> FinishingDirection("FinishingDirection:",FinishDirection);
    storables.push_back(&FinishingDirection);
    StorableSingle<unsigned> TotalStartingPlayers("TotalStartingPlayers:",TotalStarting);
    storables.push_back(&TotalStartingPlayers);
    StorableVectorVector2<unsigned> TrackStartingPositions("StartingPositions:",StartingPositions);
    storables.push_back(&TrackStartingPositions);
    StorableTileMap TileMap("Tiles:",Tiles);
    storables.push_back(&TileMap);


    for(auto index : storables)
    {
        index->WriteToStream(OutputStream);
    }

    OutputStream<<"\n"<<"--EndTrack--";
    OutputStream.close();
}

void Track::FlushTrack(unsigned TrackNumber){
    ifstream Input("Tracks.txt",ios::binary);
    string InputString;
    unsigned InputInt;
    vector<unsigned> TrackNumberList;
    bool TrackFound=0;
    while(!Input.eof())
    {
        Input>>InputString;
        if (InputString.compare("--StartTrack--")==0)
        {
            Input>>InputString;
            Input>>InputInt;
            if(InputInt==TrackNumber)
            {
                TrackFound=1;
            }
            TrackNumberList.push_back(InputInt);
        }
    }
    Input.close();
    vector<Track> Tracks;
    Track EmptyTrack;
    if (TrackFound==1){
        for (unsigned i=0; i< TrackNumberList.size();i++)
        {
            Tracks.push_back(EmptyTrack);
            Tracks[i].ReadTrack(TrackNumberList[i]);
        }
        ofstream EmptyData("Tracks.txt");
        EmptyData.close();
        for (unsigned i=0; i< TrackNumberList.size();i++)
        {
            if (TrackNumber!=TrackNumberList[i])
            {
                Tracks[i].WriteTrack(TrackNumberList[i]);
            }
        }
    }
}

void Track::SetBlank(Vector2u NewDim){
    Dim=NewDim;
    StartingPositions.clear();
    for (unsigned i=0; i<8; ++i)
    {
        StartingPositions.push_back(sf::Vector2u(i,0));
    }

    Tile tile;
    vector<Tile> VectorTile(Dim.y,tile);
    vector<vector<Tile>> MatrixTile(Dim.x,VectorTile);
    Tiles=MatrixTile;
}

void Track::Reshape(){
    Vector2u NewSize=Dim;
    vector<Tile> TileVector(NewSize.y,Tile());
    vector<vector<Tile>> NewTiles(NewSize.x,TileVector);
    Vector2u OldSize=Vector2u(Tiles.size(),Tiles[0].size());
    Vector2u MinSize=Vector2u(min(OldSize.x,NewSize.x),min(OldSize.y,NewSize.y));
    for (unsigned i=0; i<MinSize.x; i++)
    {
        for (unsigned j=0; j<MinSize.y;j++)
        {
            NewTiles[i][j]=Tiles[i][j];
        }
    }
    Tiles=NewTiles;
}

bool Track::getFinishSquareHalf(Vector2d Position){
    Vector2u TilePosition=Vector2u(Position.x,Position.y);
    bool Vertical=FinishDirection%2;
    bool Negative=FinishDirection/2;
    if(Vertical==0)
    {
        return ((1-2*Negative)*(Position.x-TilePosition.x-0.5)>0);
    }
    else
    {
        return ((1-2*Negative)*(Position.y-TilePosition.y-0.5)>0);
    }
}


void Track::FlipTrap(unsigned index, bool Mode){
    Trap* trap=&Traps[index];

    for (unsigned i=0; i< trap->TrapPositions.size(); i++)
    {
        Vector2u& Position=trap->TrapPositions[i];
        Vector2u& Orientation=trap->TrapOrientations[i];
        Vector2u* Types;
        if (Mode==1)
            Types=&trap->TrapTypesOn[i];
        else
            Types=&trap->TrapTypesOff[i];
        if (Orientation.y==0)
            Tiles[Position.x][Position.y].Types=*Types;
        if (Orientation.y==1)
            Tiles[Position.x][Position.y].Types.x=Types->x;
        if (Orientation.y==1)
            Tiles[Position.x][Position.y].Types.y=Types->y;
    }
}
