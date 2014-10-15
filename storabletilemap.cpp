#include "storable.h"

void StorableTileMap::ReadFromStream(ifstream &inputstream){
    Tile tile;
    vector<Tile> VectorTile;
    Vector2u InputInt;
    string InputString;
    vector<vector<Tile>> &Tiles=*Target;
    Tiles.clear();
    Tiles.push_back(VectorTile);
    unsigned i=0;
    while (!inputstream.eof())
    {
        inputstream>>InputString;
        if(InputString.compare(0,1, "[" )==0)
        {
            tile.Orientation=1;
        }
        else{
            tile.Orientation=0;
        }
        InputString.erase(0,1);
        istringstream (InputString) >> InputInt.x;

        inputstream>>InputString;
        InputString.erase(1,1);
        istringstream (InputString) >> InputInt.y;

        tile.isSquare= InputInt.x==InputInt.y;
        tile.Types=InputInt;
        Tiles[i].push_back(tile);

        streampos StepBack=inputstream.tellg();
        inputstream>>InputString;
        if(InputString=="-")
        {
            i++;
            Tiles.push_back(VectorTile);
        }
        else{
            inputstream.seekg(StepBack);
        }

        StepBack=inputstream.tellg();
        inputstream>>InputString;
        if(InputString=="----")
        {
            Tiles.pop_back();
            break;
        }
        inputstream.seekg(StepBack);
    }
}

void StorableTileMap::WriteToStream(ofstream &outputstream){
    outputstream<<Sig<<" ";
    vector<vector<Tile>> &Tiles=*Target;
    Vector2u Dim=Vector2u(Tiles.size(),Tiles[0].size());
    for(unsigned index=0; index<Dim.x*Dim.y; index++)
    {
        unsigned i=index%Dim.x;
        unsigned j=index/Dim.x;
        if (Tiles[i][j].Orientation==1)
        {
            outputstream<<"[";
        }
        else
        {
            outputstream<<"(";
        }
        outputstream<<Tiles[i][j].Types.x<<" "<<Tiles[i][j].Types.y<<") ";
        if (i==Dim.x-1)
        {
            outputstream<<"- "<<"\n";
        }
    }
    outputstream<<"----"<<"\n";
}
