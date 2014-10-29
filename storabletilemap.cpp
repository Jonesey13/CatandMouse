#include "storable.h"

void StorableTileMap::ReadFromStream(ifstream &inputstream){
    Tile tile;
    vector<Tile> VectorTile;
    Vector2u InputInt;
    string InputString;
    vector<vector<Tile>> &Tiles=*Target;
    Tiles.clear();
    vector<vector<Tile>> SwappedTiles;
    SwappedTiles.push_back(VectorTile);
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
        if(InputString.substr(1,1)=="]")
            {
                tile.isSquare=1;
            }
        else
            {
                tile.isSquare=0;
            }

        InputString.erase(1,1);
        istringstream (InputString) >> InputInt.y;
        tile.Types=InputInt;
        SwappedTiles[i].push_back(tile);

        streampos StepBack=inputstream.tellg();
        inputstream>>InputString;
        if(InputString=="-")
        {
            i++;
            SwappedTiles.push_back(VectorTile);
        }
        else{
            inputstream.seekg(StepBack);
        }

        StepBack=inputstream.tellg();
        inputstream>>InputString;
        if(InputString=="----")
        {
            SwappedTiles.pop_back();
            break;
        }
        inputstream.seekg(StepBack);
    }
    Vector2u SwappedSize=Vector2u(SwappedTiles.size(),SwappedTiles[0].size());
    Vector2u Size=Vector2u(SwappedSize.y,SwappedSize.x);
    VectorTile=vector<Tile>(Size.y,tile);
    Tiles=vector<vector<Tile>>(Size.x,VectorTile);
    for (unsigned i1=0; i1<Size.x; i1++)
    {
        for (unsigned i2=0; i2<Size.y; i2++)
        {
            Tiles[i1][i2]=SwappedTiles[i2][i1];
        }
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
        outputstream<<Tiles[i][j].Types.x<<" "<<Tiles[i][j].Types.y;
        if(Tiles[i][j].isSquare)
            outputstream<<"] ";
        else
            outputstream<<") ";

        if (i==Dim.x-1)
        {
            outputstream<<"- "<<"\n";
        }
    }
    outputstream<<"----"<<"\n";
}
