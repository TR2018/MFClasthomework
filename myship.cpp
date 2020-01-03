#include "myship.h"
#include<QString>
MyShip::MyShip()
{
    STI.Ship_Name = "XXX";
    STI.Call_Sign="BL4J";
    STI.MMSI="987654123";
    STI.Ship_Breadth = 50.00;
    STI.Ship_Length = 100.00;
    STI.Draft = 50.00;
    STI.Displacement = 50.00;
    DYI.Course = 0.0;
    DYI.Latitude = 0.000;
    DYI.Longitude = 0.000;
    DYI.Speed = 0.0;
    DYI.Latitude_real=0.000;
    DYI.longitude_real=0.000;

}



MyShip::MyShip(const MyShip & ship)
{
    this->STI = ship.STI;
    this->DYI = ship.DYI;
    this->Ship_Shape_x1 = ship.Ship_Shape_x1;
    this->Ship_Shape_x2 = ship.Ship_Shape_x2;
    this->Ship_Shape_x3 = ship.Ship_Shape_x3;

}


void MyShip::ShipPosition_ShipSpeed_ShipCurse(Dynamic_information & dyi)
{
    DYI.Course = dyi.Course;
    DYI.Speed = dyi.Speed;
    DYI.Latitude = dyi.Latitude;
    DYI.Longitude = dyi.Longitude;
}


void MyShip::ShipShape()
{
    if(DYI.Course==0&&DYI.Latitude==0&&DYI.Longitude==0&&DYI.Speed==0)
    {
        Ship_Shape_x1=0;
        Ship_Shape_y1=-STI.Ship_Length/2;
        Ship_Shape_x2=-STI.Ship_Breadth / 2;
        Ship_Shape_y2=STI.Ship_Length/2;
        Ship_Shape_x3=STI.Ship_Breadth / 2;
        Ship_Shape_y3=STI.Ship_Length/2;
    }
    else

    {
        Ship_Shape_x1=(DYI.Longitude+(STI.Ship_Length/2)*sin(DEG2RAD(DYI.Course)))/5;
        Ship_Shape_y1=(DYI.Latitude-(STI.Ship_Length/2)*cos(DEG2RAD(DYI.Course)))/5;
        Ship_Shape_x2=(DYI.Longitude-(STI.Ship_Length/2)*sin(DEG2RAD(DYI.Course))+(STI.Ship_Breadth/2)*sin(DEG2RAD(DYI.Course-90)))/5;
        Ship_Shape_y2=(DYI.Latitude+(STI.Ship_Length/2)*cos(DEG2RAD(DYI.Course))-(STI.Ship_Breadth/2)*cos(DEG2RAD(DYI.Course-90)))/5;
        Ship_Shape_x3=(DYI.Longitude-(STI.Ship_Length/2)*sin(DEG2RAD(DYI.Course))+(STI.Ship_Breadth/2)*sin(DEG2RAD(DYI.Course+90)))/5;
        Ship_Shape_y3=(DYI.Latitude+(STI.Ship_Length/2)*cos(DEG2RAD(DYI.Course))-(STI.Ship_Breadth/2)*cos(DEG2RAD(DYI.Course+90)))/5;
    }

}
void MyShip::ShipShape_Move()
{
    float speed_m_s = DYI.Speed *0.5* 1852 / 3600;
    DYI.longitude_real=DYI.longitude_real+DYI.Speed*0.1/3600*sin(DEG2RAD(DYI.Course));
    DYI.Latitude_real=DYI.Latitude_real-DYI.Speed*0.1/3600*cos(DEG2RAD(DYI.Course));
    DYI.Latitude = DYI.Latitude- speed_m_s * cos(DEG2RAD(DYI.Course));
    DYI.Longitude = DYI.Longitude+ speed_m_s * sin(DEG2RAD(DYI.Course));
    ShipShape();

}






double MyShip::Distance(double lat, double longit)
{
    static double Ea = 6378137;  // 赤道半径
    static double Eb = 6356725;  // 极半径

    double ec = Eb + (Ea - Eb) * (90.0 - ((DYI.Latitude + lat) / 2)) / 90.0;


    DYI.Latitude = DEG2RAD(DYI.Latitude);
    DYI.Longitude = DEG2RAD(DYI.Longitude);
    lat = DEG2RAD(lat);
    longit = DEG2RAD(longit);

    double dlat = DYI.Latitude - lat;
    double dlong = DYI.Longitude - longit;

    double AC;
    double CB;
    double AD;
    double d;
    double d1;
    double d2;
    double distance;
    AC = 2 * sin(dlat / 2);
    AD = 2 * sin(dlong / 2)*cos(DYI.Latitude);
    CB = 2 * sin(dlong / 2)*cos(lat);
    d = AC * AC + CB * AD;
    d1 = d / 4;
    d2 = 2 * atan2(sqrt(d1), sqrt(1 - d1));
    distance = 2 * d2*ec;
    return distance;
}
