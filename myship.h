#ifndef MYSHIP_H
#define MYSHIP_H
#include<QString>
#include<QDebug>
#include<qmath.h>
#define M_PI 3.14159
#define DEG2RAD(x)  (x)*M_PI/180


struct Static_Information
{
    QString Ship_Name;
    QString Call_Sign;
    QString MMSI;
    float Ship_Length;
    float Ship_Breadth;
    float Draft;
    float Displacement;
};

struct Dynamic_information
{
    float Speed;
    float Course;
    double Longitude;
    double Latitude;
    double longitude_real;
    double Latitude_real;
};
class MyShip
{
public:
    MyShip();
    MyShip(const MyShip & ship);
    void ShipPosition_ShipSpeed_ShipCurse(Dynamic_information & dyi);
    void ShipShape();
    void ShipShape_Move();
    double Distance(double lat, double longit);
    Static_Information STI;
    Dynamic_information DYI;
    double Ship_Shape_x1, Ship_Shape_y1;
    double Ship_Shape_x2, Ship_Shape_y2;
    double Ship_Shape_x3, Ship_Shape_y3;

};

#endif // MYSHIP_H
