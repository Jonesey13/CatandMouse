#include "race.h"

Race::Race(){
}


void Race::init(Configuration &NewConfig){
    Config=NewConfig;
    track.ReadTrack(Config.TrackNumber);
    Car car;
    for(unsigned i=0; i< Config.NumberOfPlayers; i++){
        Player.push_back(car);
        Player[i].Position=Vector2d(track.StartingPositions[i].x,track.StartingPositions[i].y) +Vector2d(0.5,0.5);
        Player[i].Rotation=0.25*track.FinishDirection;
    }
    for (auto trap : track.Traps)
    {
        trap.MainSwitch=0;
    }
}


void Race::Update(double DeltaTime){
    track.RefreshDetection();
    for (unsigned i=0; i<Player.size() ; i++)
    {
        if (Player[i].DeathSwitch==0)
        {
            Player[i].Update(DeltaTime);
        }
        else{
            Player[i].DeathUpdate(DeltaTime);
        }
        if (Player[i].RequestReset)
        {
            Player[i].RequestReset=0;
            ResetCar(i);
        }
    }
    UpdateTraction();
    UpdateLaps();
    HandleTraps();
    HandlePlayerOnPlayerCollisions();
    HandleWallCollisions();
    HandleDeaths();
}

void Race::UpdateLaps(){
    for (unsigned i=0; i<Player.size();i++)
    {
        Tile* CurrentTile=track.getTile(Player[i].Position.x, Player[i].Position.y);
        Detect Detection=CurrentTile->Detection;
        if (Detection.x.count(FINISH))
        {
            if (track.getFinishSquareHalf(Player[i].Position)==0)
            {
                if(Player[i].LapFlag==1)
                {
                    Player[i].LapNumber--;
                }
                Player[i].LapFlag=-1;
            }
            else
            {
                if(Player[i].LapFlag==-1)
                {
                    Player[i].LapNumber++;
                }
                Player[i].LapFlag=1;
            }
        }
        else
        {
            Player[i].LapFlag=0;
        }
        int TotalLaps=Config.NumberOfLaps+1;
        if (Player[i].LapNumber==TotalLaps)
        {
            Victory=1;
            VictorNumber=i+1;
        }
    }
}

void Race::UpdateTraction(){
    for (unsigned i=0; i<Player.size();i++)
    {
        Tile* CurrentTile=track.getTile(Player[i].Position.x, Player[i].Position.y);
        Detect Detection=CurrentTile->Detection;
        bool Orientation=CurrentTile->Orientation;
        if (CurrentTile->isSquare || getTriangleHalf(Player[i].Position,Orientation)==0)
        {
            if (Detection.x.count(NORMAL_TRACTION))
            {
                Player[i].NormalTraction();
            }
            if (Detection.x.count(SLIDING_TRACTION))
            {
                Player[i].SlidingTraction();
            }
        }
        else{
            if (Detection.y.count(NORMAL_TRACTION))
            {
                Player[i].NormalTraction();
            }
            if (Detection.y.count(SLIDING_TRACTION))
            {
                Player[i].SlidingTraction();
            }
        }
    }
}

void Race::HandleDeaths(){
    for (unsigned i=0; i<Player.size();i++)
    {
        if (Player[i].DeathSwitch==1)
        {
            continue;
        }
        Vector2u CurrentSquare=Player[i].getGridPosition();
        Tile* CurrentTile=track.getTile(CurrentSquare.x,CurrentSquare.y);
        Detect Detection=CurrentTile->Detection;
        vector<Vector2d> Bounding;
        if (CurrentTile->isSquare)
        {
            if( Detection.x.find(FALL)!=Detection.x.end())
            {
                Player[i].DeathSwitch=1;
                Bounding=track.getTileBounding(CurrentSquare.x,CurrentSquare.y);
            }
        }
        else{
            bool Orientation=CurrentTile->Orientation;
            if (!getTriangleHalf(Player[i].Position,Orientation) && Detection.x.find(FALL)!=Detection.x.end())
            {
                Player[i].DeathSwitch=1;
                Bounding=track.getTileBounding(CurrentSquare.x,CurrentSquare.y,0);
            }
            if(getTriangleHalf(Player[i].Position,Orientation) && Detection.y.find(FALL)!=Detection.y.end())
            {
                Player[i].DeathSwitch=1;
                Bounding=track.getTileBounding(CurrentSquare.x,CurrentSquare.y,1);
            }
        }
        if (Player[i].DeathSwitch==1)
        {
            Vector2d Average=accumulate(Bounding.begin(),Bounding.end(),Vector2d(0,0))/static_cast<double>(Bounding.size());
            Vector2d Difference=Average-Player[i].Position;
            Player[i].Velocity=Difference/Car::DeathDuration;
            Player[i].PositionBeforeDeath=Player[i].Position;
        }
    }
}

void Race::HandleWallCollisions(){
    int trackwidth=track.getDim().x, trackheight=track.getDim().y;
    for (unsigned i=0; i<Player.size();i++)
    {
        bool StillCorrecting=1;
        unsigned Attempts=0;
        while (StillCorrecting){
            StillCorrecting=0;
            vector<Vector2d> Bounding=Player[i].Bounding;
            for (unsigned j=0; j<4;j++)
            {
                Bounding[j]=RotateVector(Bounding[j],Player[i].Rotation);
                Bounding[j]+=Player[i].Position;
            }
            Vector2u CurrentSquare=Player[i].getGridPosition();
            for (int j=0; j<9;j++)
            {
                bool TestCollision=0;
                vector<Vector2d> BoundingTile;
                int index1=(j%3)-1, index2=(j/3)-1;

                index1+=CurrentSquare.x; index2+=CurrentSquare.y;
                if (index1>=0 && index1<trackwidth && index2>=0 && index2<trackheight)
                {
                    Tile* CurrentTile=track.getTile(index1,index2);
                    Detect Detection=CurrentTile->Detection;
                    if(CurrentTile->isSquare && Detection.x.find(WALL)!=Detection.x.end())
                    {
                        BoundingTile={Vector2d(index1,index2),Vector2d(index1,index2+1),
                        Vector2d(index1+1,index2+1),Vector2d(index1+1,index2)};
                        TestCollision=1;
                    }
                    if(CurrentTile->isSquare==0 && Detection.y.find(WALL)!=Detection.y.end())
                    {
                        if(CurrentTile->Orientation==1)
                        {
                            BoundingTile={Vector2d(index1,index2),Vector2d(index1,index2+1),Vector2d(index1+1,index2)};
                        }
                        else{
                            BoundingTile={Vector2d(index1,index2),Vector2d(index1+1,index2+1),Vector2d(index1+1,index2)};
                        }
                        TestCollision=1;
                    }
                    if(CurrentTile->isSquare==0 && Detection.x.find(WALL)!=Detection.x.end())
                    {
                        if(CurrentTile->Orientation==1)
                        {
                            BoundingTile={Vector2d(index1,index2+1),Vector2d(index1+1,index2+1),Vector2d(index1+1,index2)};
                        }
                        else{
                            BoundingTile={Vector2d(index1,index2),Vector2d(index1,index2+1),Vector2d(index1+1,index2+1)};
                        }
                        TestCollision=1;
                    }
                }
                else{
                    BoundingTile={Vector2d(index1,index2),Vector2d(index1,index2+1),
                    Vector2d(index1+1,index2+1),Vector2d(index1+1,index2)};
                    TestCollision=1;
                }
                if(TestCollision==1)
                {
                    bool CornerPoly=0;
                    Vector2u CornerSide=Vector2u();
                    Vector2d Direction=Vector2d();
                    double Overlap=0;
                    bool DoubleFlag=0;
                    if(InPolygon(BoundingTile, Bounding, CornerPoly, CornerSide, Direction,Overlap,DoubleFlag))
                    {
                        if(CornerPoly==1)
                        {
                            Direction=Vector2d(-Direction.x,-Direction.y);
                        }
                        if (DoubleFlag==1)
                        {
                            Vector2d TileAverage=accumulate(BoundingTile.begin(),BoundingTile.end(),Vector2d(0,0));
                            TileAverage=TileAverage/static_cast<double>(BoundingTile.size());
                            Vector2d Average=accumulate(Bounding.begin(),Bounding.end(),Vector2d(0,0));
                            Average=Average/static_cast<double>(Bounding.size());
                            Vector2d AltVelocity=Average-TileAverage;
                            AltVelocity=AltVelocity/(sqrt(DotProduct(AltVelocity,AltVelocity)));
                            HandleSingleWallCollision(Player[i], Direction, Overlap,Attempts,AltVelocity);
                        }
                        else{
                            HandleSingleWallCollision(Player[i], Direction, Overlap,Attempts);
                        }
                        StillCorrecting=1;
                        Attempts++;
                    }
                }
                if(Attempts>=20)
                {
                    Player[i].DeathSwitch=1;
                    StillCorrecting=0;
                }
            }
        }
    }
}

void Race::HandleSingleWallCollision(Car &Racer, Vector2d Direction, double Overlap, unsigned Attempts, Vector2d AltVelocityChange){
    if (Overlap<0.0001)
    {
        Overlap=0.0001;
    }
    Racer.Position+=2*Overlap*Direction;
    if (Attempts==0){
        if (DotProduct(AltVelocityChange,AltVelocityChange)>0.0001)
        {
            Direction=AltVelocityChange;
        }
        Racer.Velocity=Car::Wall_Elasticity*Racer.Velocity;
        Vector2d CarLinear=Direction*DotProduct(Direction,Racer.Velocity);
        Racer.Velocity=Racer.Velocity-2.0*CarLinear;
    }
}

void Race::HandlePlayerOnPlayerCollisions(){
    for (unsigned i=0; i<Player.size()-1;i++)
    {
        for (unsigned j=i+1; j<Player.size();j++)
        {
            if (Player[i].DeathSwitch==1 || Player[j].DeathSwitch==1)
                continue;
            bool StillCorrecting=1;
            unsigned Attempts=0;
            while (StillCorrecting){
                StillCorrecting=0;
                vector<Vector2d> Bounding1=Player[i].Bounding;
                for (unsigned k=0; k<Bounding1.size();k++)
                {
                    Bounding1[k]=RotateVector(Bounding1[k],Player[i].Rotation);
                    Bounding1[k]+=Player[i].Position;
                }
                vector<Vector2d> Bounding2=Player[j].Bounding;
                for (unsigned k=0; k<Bounding2.size();k++)
                {
                    Bounding2[k]=RotateVector(Bounding2[k],Player[j].Rotation);
                    Bounding2[k]+=Player[j].Position;
                }
                bool CornerPoly=0;
                Vector2u CornerSide=Vector2u();
                Vector2d Direction=Vector2d();
                double Overlap=0;
                bool DoubleFlag=0;
                if(InPolygon(Bounding1, Bounding2, CornerPoly, CornerSide, Direction,Overlap,DoubleFlag))
                {
                    vector<vector<Vector2d>> Bounding={Bounding1,Bounding2};
                    Vector2d CollisionPos=Bounding[CornerPoly][CornerSide.x];
                    HandleSinglePlayerCollision({&Player[i],&Player[j]},Bounding,
                                                CornerPoly,CollisionPos,Direction,Overlap,DoubleFlag,Attempts);
                    Attempts++;
                    StillCorrecting=1;
                }
                if(Attempts>=5)
                {
                    //Player[i].DeathSwitch=1;
                    StillCorrecting=0;
                }
            }
        }
    }
}

void Race::HandleSinglePlayerCollision(vector<Car*> Cars, vector<vector<Vector2d>> Bounding, bool CornerCar, Vector2d CollisionPos,
                                     Vector2d Direction, double Overlap, bool DoubleFlag, unsigned Attempts ){
    if (Overlap<0.0001)
    {
        Overlap=0.0001;
    }
    Cars[CornerCar]->Position-=Direction*Overlap;
    Cars[!CornerCar]->Position+=Direction*Overlap;

    if (DoubleFlag==0)
    {
        for (unsigned CarIndex=0; CarIndex<2 ; CarIndex++)
        {
            Vector2d TorqueVector=CollisionPos-Cars[(CornerCar+CarIndex)%2]->Position;
            TorqueVector=RotateVector(TorqueVector,-0.25);
            Vector2d CarLinear=Direction*DotProduct(Direction,Cars[(CornerCar+CarIndex)%2]->Velocity
                                                    -Cars[(CornerCar+true+CarIndex)%2]->Velocity);
            Cars[(CornerCar+CarIndex)%2]->Angular=Car::AngularElasticity*DotProduct(TorqueVector,CarLinear);
        }
    }

    if (Attempts==0)
    {
        Vector2d VelocityDifference=Cars[0]->Velocity-Cars[1]->Velocity;
        Vector2d VelocityChange=Car::Elasticity*VelocityDifference;
        Cars[0]->Velocity-=VelocityChange;
        Cars[1]->Velocity+=VelocityChange;
    }
}

void Race::HandleTraps(){
    for (unsigned i=0;i<Player.size();i++)
    {
        for (unsigned j=0; j<track.Traps.size();j++)
        {
            bool before=track.Traps[j].MainSwitch;
            track.Traps[j].PlayerInteract(Player[i],i);
            if (before!=track.Traps[j].MainSwitch)
            {
                track.FlipTrap(j,track.Traps[j].MainSwitch);
            }

        }
    }



}


Vector2u Race::getNearestValidSquare(unsigned PlayerNumber){
    Vector2u Dim=track.getDim();
    vector<double> norm(Dim.x*Dim.y,0);
    Vector2d Position=Player[PlayerNumber].PositionBeforeDeath;

    for (unsigned k=0; k<Dim.x*Dim.y;++k)
    {
        unsigned k1= k % Dim.x;
        unsigned k2= k / Dim.x;
        norm[k]=sqrt(pow(k1+0.5-Position.x,2)+pow(k2+0.5-Position.y,2));
    }
    vector<int> index(norm.size(), 0);
    for (unsigned i = 0 ; i != index.size() ; i++) {
        index[i] = i;
    }

    sort(index.begin(), index.end(),[&](const int& a, const int& b) {return (norm[a] < norm[b]);});

    unsigned j=0;
    bool SquareValid=false;
    bool CarCollision=false;
    vector<PROPERTIES> InvalidTiles={FALL,WALL};
    while (SquareValid==false || CarCollision==true)
    {
        CarCollision=false;
        SquareValid=true;
        unsigned k1=index[j]%Dim.x;
        unsigned k2=index[j]/Dim.x;
        Tile* CurrentTile=track.getTile(k1,k2);
        Detect Detection=CurrentTile->Detection;
        for (unsigned i=0; i<InvalidTiles.size(); i++)
        {
            if (Detection.x.count(InvalidTiles[i])==1)
            {
                SquareValid=0;
            }
        }
        if(CurrentTile->isSquare==0)
        {
            SquareValid=0;
        }

        Player[PlayerNumber].Position=Vector2d(k1+0.5, k2+0.5);
        for (unsigned i=0; i< Player.size();++i)
        {
            Car *Car1,*Car2;
            if(i==PlayerNumber)
            {
                continue;
            }
            Car1=&Player[PlayerNumber];
            Car2=&Player[i];

            vector<Vector2d> Bounding1=Car1->Bounding;
            for (unsigned k=0; k<Bounding1.size();k++)
            {
                Bounding1[k]=RotateVector(Bounding1[k],Player[PlayerNumber].Rotation);
                Bounding1[k]+=Player[PlayerNumber].Position;
            }
            Vector2d Center1=accumulate(Bounding1.begin(),Bounding1.end(),Vector2d(0,0))/static_cast<double>(Bounding1.size());

            vector<Vector2d> Bounding2=Car2->Bounding;
            for (unsigned k=0; k<Bounding2.size();k++)
            {
                Bounding2[k]=RotateVector(Bounding2[k],Player[i].Rotation);
                Bounding2[k]+=Player[i].Position;
            }
            Vector2d Center2=accumulate(Bounding2.begin(),Bounding2.end(),Vector2d(0,0))/static_cast<double>(Bounding2.size());

            if( InPolygon(Bounding1,Bounding2) || InPolygon(Center1,Bounding2) || InPolygon(Center2,Bounding1) )
            {
                CarCollision=1;
            }
        }
        j++;
        if (j==Dim.x*Dim.y)
        {
            cerr<<"Failed to Find Valid Square!!"<<endl;
        }
    }
    return Vector2u(index[j-1]%Dim.x,index[j-1]/Dim.x);
}

void Race::ResetCar(unsigned PlayerNumber){
    Vector2u NewPosition=getNearestValidSquare(PlayerNumber);
    Player[PlayerNumber].Position=Vector2d(NewPosition.x+0.5,NewPosition.y+0.5);
    Player[PlayerNumber].Velocity=Vector2d(0,0);
}
