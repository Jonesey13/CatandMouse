#include "functions.h"

// Convention: X-Axis left to right, Y-Axis top to bottom
// Rotation is clockwise
Vector2d RotateVector(Vector2d input, double rotation)
{
    Vector2d output;
    output.x=input.x*cos(2*M_PI*rotation)-input.y*sin(2*M_PI*rotation);
    output.y=input.y*cos(2*M_PI*rotation)+input.x*sin(2*M_PI*rotation);
    return output;
}

double DotProduct(Vector2d vector1, Vector2d vector2)
{
    double product= vector1.x*vector2.x+vector1.y*vector2.y;
    return product;
}

Vector2f Double2Float(Vector2d input)
{
    return Vector2f(input.x, input.y);
}


Vector2d ScalePosition(Vector2d position, double Scaling, bool paddingdim, unsigned padding){
    position=Scaling*position;
    if (paddingdim==0)
    {
        position =position+Vector2d(padding,0);
    }
    else{
        position =position+Vector2d(0,padding);
    }
    return position;
}

Vector2f ScalePosition(Vector2f position, double Scaling, bool paddingdim, unsigned padding){
    position=static_cast<float>(Scaling)*position;
    if (paddingdim==0)
    {
        position =position+Vector2f(padding,0);
    }
    else{
        position =position+Vector2f(0,padding);
    }
    return position;
}

bool InPolygon(Vector2d Point, vector<Vector2d> Polygon){
    vector<Vector2d> PointPoly={Point};
    return InPolygon(PointPoly,Polygon);
}

bool InPolygon(vector<Vector2d> Polygon1, vector<Vector2d> Polygon2){
    bool CornerPoly=0;
    Vector2u CornerSide=Vector2u(0,0);
    Vector2d Direction=Vector2d(0,0);
    double Overlap=0;
    bool DoubleFlag=0;
    return InPolygon(Polygon1, Polygon2, CornerPoly, CornerSide, Direction, Overlap, DoubleFlag);
}



bool InPolygon(vector<Vector2d> Polygon1, vector<Vector2d> Polygon2, bool &CornerPoly,
               Vector2u &CornerSide, Vector2d &Direction, double &Overlap, bool &DoubleFlag){
    double GreatestOverlap=-1000;
    bool FoundOverlap=0;
    vector<Vector2d> Polygon;
    Vector2d Point;
    unsigned j1=0;
    bool CurrentCornerPoly=0;
    bool SmallestCornerPoly=0;
    for(unsigned j=0; j<Polygon1.size()+Polygon2.size();j++)
    {
        if (j<Polygon1.size())
        {
            j1=j;
            Point=Polygon1[j];
            Polygon=Polygon2;
            CurrentCornerPoly=0;
        }
        else{
            if (Polygon1.size()==1)
            {
                continue;
            }
            j1=j-Polygon1.size();
            Point=Polygon2[j1];
            Polygon=Polygon1;
            CurrentCornerPoly=1;
        }
        Polygon.push_back(Polygon[0]);
        double SmallestCrossover=1000;
        Vector2d SmallestNormal=Vector2d();
        Vector2u SmallestCornerSide=Vector2u();
//        if (j==Polygon1.size() && GreatestOverlap>0)
//            break;
        for (unsigned i=0; i< Polygon.size()-1; i++)
        {
            Vector2d Side=Polygon[i+1]-Polygon[i];
            Side=Side/(sqrt(DotProduct(Side,Side)));
            Vector2d Normal=RotateVector(Side,-0.25);
            Vector2d Relative=Point-Polygon[i];
            double Product=DotProduct(Relative,Normal);
            if (Product<SmallestCrossover)
            {
                SmallestCrossover=Product;
                SmallestCornerSide=Vector2u(j1,i);
                SmallestCornerPoly=CurrentCornerPoly;
                SmallestNormal=Normal;
            }
        }
        if (SmallestCrossover>0)
        {
            if (FoundOverlap==0)
                FoundOverlap=1;
            else
                DoubleFlag=1;
        }
        if (SmallestCrossover>GreatestOverlap)
        {
            GreatestOverlap=SmallestCrossover;
            Direction=SmallestNormal;
            CornerPoly=SmallestCornerPoly;
            CornerSide=SmallestCornerSide;
        }
    }
    Overlap=GreatestOverlap;
    return FoundOverlap;
}

bool getTriangleHalf(Vector2d Position, bool Orientation){
    Vector2u TrackPosition=Vector2u(Position.x,Position.y);
    double component;
    if (Orientation==0){
        Vector2d Position1=Vector2d(TrackPosition.x+1,TrackPosition.y);
        Vector2d Position2=Vector2d(TrackPosition.x,TrackPosition.y+1);
        Vector2d Position3=Vector2d(TrackPosition.x,TrackPosition.y);
        Vector2d Normal=Position1-Position2;
        component=DotProduct(Position-Position3,Normal);
    }
    else{
        Vector2d Position1=Vector2d(TrackPosition.x,TrackPosition.y);
        Vector2d Position2=Vector2d(TrackPosition.x+1,TrackPosition.y+1);
        Vector2d Position3=Vector2d(TrackPosition.x,TrackPosition.y+1);
        Vector2d Normal=Position1-Position2;
        component=DotProduct(Position-Position3,Normal);
    }
    return (component>0);
}
