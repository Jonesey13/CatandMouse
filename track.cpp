#include "track.h"
#include "functions.h"


Track::Track(){
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
    vector<set<string>> DetectionList;
    DetectionList.resize(6);
    DetectionList[0]={"FALL"};
    DetectionList[1]={"NORMAL_TRACTION"};
    DetectionList[2]={"WALL"};
    DetectionList[3]={"SLIDING_TRACTION"};
    DetectionList[4]={"NORMAL_TRACTION","FINISH"};
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
    vector<unsigned> InputInt(10);
    ifstream Input("Tracks.txt",ios::binary);
    bool TrackFound=0;
    while (TrackFound==0 && !Input.eof())
    {
        Input>>InputString;
        if (InputString.compare("--StartTrack--")==0)
        {
            Input>>InputString;
            Input>>InputInt[0];
            if(InputInt[0]==TrackNumber)
            {
                TrackFound=1;
            }
        }
    }
    if (TrackFound==0)
    {
        cout<<"Failed to Find Track!"<<endl;
    }
    else{
        Input>>InputString;
        Input>>InputInt[0];
        Input>>InputString;
        Input>>InputInt[1];
        Dim=sf::Vector2u(InputInt[0],InputInt[1]);
        Input>>InputString;
        Input>>InputInt[0];
        FinishDirection=InputInt[0];
        Input>>InputString;
        Input>>InputInt[0];
        TotalStarting=InputInt[0];
        Input>>InputString;
        Input>>InputInt[0];
        Input>>InputInt[1];
        StartingPositions.clear();
        StartingPositions.push_back(Vector2u(InputInt[0],InputInt[1]));
        bool KeepGoing=1;
        streampos onestepback=Input.tellg();
        Input>>InputString;
        if (InputString.compare("Tiles:")==0)
        {
            KeepGoing=0;
        }
        else{
            Input.seekg(onestepback);
        }
        while (KeepGoing)
        {
            Input>>InputInt[0];
            Input>>InputInt[1];
            StartingPositions.push_back(Vector2u(InputInt[0],InputInt[1]));
            onestepback=Input.tellg();
            Input>>InputString;
            if (InputString.compare("Tiles:")==0)
            {
                KeepGoing=0;
            }
            else{
                Input.seekg(onestepback);
            }
        }


        Tile tile;
        vector<Tile> VectorTile(Dim.y,tile);
        vector<vector<Tile>> MatrixTile(Dim.x,VectorTile);
        unsigned index=0;
        while (index< Dim.x*Dim.y)
        {
            unsigned i=index%Dim.x;
            unsigned j=index/Dim.x;
            Input>>InputString;
            if(InputString.compare(0,1, "[" )==0)
            {
                MatrixTile[i][j].Orientation=1;
            }
            else{
                MatrixTile[i][j].Orientation=0;
            }
            InputString.erase(0,1);
            istringstream (InputString) >> InputInt[0];

            Input>>InputString;
            InputString.erase(1,1);
            istringstream (InputString) >> InputInt[1];

            MatrixTile[i][j].isSquare= InputInt[0]==InputInt[1];

            MatrixTile[i][j].Types=sf::Vector2u(InputInt[0], InputInt[1]);
            index++;
        }
        Tiles=MatrixTile;
        TrackReady=1;
    }
}



void Track::WriteTrack(unsigned TrackNumber){
    ofstream Input("Tracks.txt",ios::app);
    Input<<"\n\n\n--StartTrack--\n\n"<<"TrackNumber: "<< TrackNumber << "\n";
    Input<<"Width: "<<Dim.x<<" Height: "<<Dim.y<<"\n";
    Input<<"FinishingDirection: "<<FinishDirection<<"\n";
    Input<<"TotalStartingPlayers: "<<TotalStarting<<"\n";
    Input<<"StartingPositions: ";
    for(unsigned i=0; i<StartingPositions.size(); i++)
    {
        Input<<StartingPositions[i].x<<" "<<StartingPositions[i].y<<"\n";
    }
    Input<<"\n";
    Input<<"Tiles: ";
    for(unsigned index=0; index<Dim.x*Dim.y; index++)
    {
        unsigned i=index%Dim.x;
        unsigned j=index/Dim.x;
        if (Tiles[i][j].Orientation==1)
        {
            Input<<"[";
        }
        else
        {
            Input<<"(";
        }
        Input<<Tiles[i][j].Types.x<<" "<<Tiles[i][j].Types.y<<") ";
        if (i==Dim.x-1)
        {
            Input<<"\n";
        }
    }
    Input<<"\n"<<"--EndTrack--";
    Input.close(); //close it
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
    unsigned index=0;
    while (index< Dim.x*Dim.y)
    {
        unsigned i=index%Dim.x;
        unsigned j=index/Dim.x;
        MatrixTile[i][j].Orientation=0;
        MatrixTile[i][j].isSquare=1;
        MatrixTile[i][j].Types=sf::Vector2u(1,1);
        index++;
    }
    Tiles=MatrixTile;
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
