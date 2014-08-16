#include "header.h"


Track::Track(){
}

void Track::init(int TrackNumber)
{
    Texture.loadFromFile("TrackSprites.png");
    Size=32;
    ReadTrack(TrackNumber);

    for (unsigned int i=0; i<FinishLine.size();i++)
    {
        Tiles[FinishLine[i].x][FinishLine[i].y].Types=sf::Vector2u(5,5);
        Tiles[FinishLine[i].x][FinishLine[i].y].isSquare=1;
    }
}


sf::Vector2u Track::getDim(){
    return Dim;
}

int Track::getSize(){
    return Size;
}


Tile* Track::getTile(int i, int j){
return &Tiles[i][j];
}

sf::Vector2u Track::getStartingPosition(){
    return StartingPosition;
}

void Track::setStartingPosition(sf::Vector2u NewStarting){
    StartingPosition=NewStarting;
}

vector<sf::Vector2u> Track::getFinishLine(){
    return FinishLine;
}

void Track::setFinishLine(vector<sf::Vector2u> NewFinishLine){
    FinishLine=NewFinishLine;
}

sf::Texture* Track::getTexture(){
return &Texture;
}

void Track::ReadTrack(int TrackNumber){
    string InputString;
    vector<int> InputInt(10);
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
        Input>>InputInt[1];
        StartingPosition=sf::Vector2u(InputInt[0],InputInt[1]);
        Input>>InputString;
        bool FinishGoing=1;
        streampos onestepback=Input.tellg();
        Input>>InputString;
        if (InputString.compare("Tiles:")==0)
        {
            FinishGoing=0;
        }
        else{
            Input.seekg(onestepback);
        }
        while (FinishGoing && !Input.eof())
        {
            Input>>InputInt[0];
            Input>>InputInt[1];
            FinishLine.push_back(sf::Vector2u(InputInt[0],InputInt[1]));
            onestepback=Input.tellg();
            Input>>InputString;
            if (InputString.compare("Tiles:")==0)
            {
                FinishGoing=0;
            }
            else{
                Input.seekg(onestepback);
            }
        }
        Tile tile;
        vector<Tile> VectorTile(Dim.y,tile);
        vector<vector<Tile>> MatrixTile(Dim.x,VectorTile);
        unsigned int index=0;
        while (index< Dim.x*Dim.y)
        {
            unsigned int i=index%Dim.x;
            unsigned int j=index/Dim.x;
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



void Track::WriteTrack(int TrackNumber){
    ofstream Input("Tracks.txt",ios::app);
    Input<<"\n\n\n--StartTrack--\n\n"<<"TrackNumber: "<< TrackNumber << "\n";
    Input<<"Width: "<<Dim.x<<" Height: "<<Dim.y<<"\n";
    Input<<"StartingPosition: "<<StartingPosition.x<<" "<<StartingPosition.y<<"\n";
    Input<<"FinishLine: ";
    for(unsigned int i=0; i<FinishLine.size(); i++)
    {
        Input<<FinishLine[i].x<<" "<<FinishLine[i].y<<"\n";
    }
    Input<<"\n";
    Input<<"Tiles: ";
    for(unsigned int index=0; index<Dim.x*Dim.y; index++)
    {
        unsigned int i=index%Dim.x;
        unsigned int j=index/Dim.x;
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

void Track::FlushTrack(int TrackNumber){
    ifstream Input("Tracks.txt",ios::binary);
    string InputString;
    int InputInt;
    vector<int> TrackNumberList;
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
        cout<<"Here!"<<endl;
        for (unsigned int i=0; i< TrackNumberList.size();i++)
        {
            Tracks.push_back(EmptyTrack);
            Tracks[i].ReadTrack(TrackNumberList[i]);
        }
        ofstream EmptyData("Tracks.txt");
        EmptyData.close();
        for (unsigned int i=0; i< TrackNumberList.size();i++)
        {
            if (TrackNumber!=TrackNumberList[i])
            {
                Tracks[i].WriteTrack(TrackNumberList[i]);
            }
        }
    }
}

void Track::SetBlank(sf::Vector2u NewDim){
    Texture.loadFromFile("TrackSprites.png");
    Size=32;
    Dim=NewDim;
    StartingPosition=sf::Vector2u();
    Tile tile;
    vector<Tile> VectorTile(Dim.y,tile);
    vector<vector<Tile>> MatrixTile(Dim.x,VectorTile);
    unsigned int index=0;
    while (index< Dim.x*Dim.y)
    {
        unsigned int i=index%Dim.x;
        unsigned int j=index/Dim.x;
        MatrixTile[i][j].Orientation=0;
        MatrixTile[i][j].isSquare=1;
        MatrixTile[i][j].Types=sf::Vector2u(1,1);
        index++;
    }
    Tiles=MatrixTile;
    TrackReady=1;
}
