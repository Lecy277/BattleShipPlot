#ifndef CARRIERGROUP_H_INCLUDED
#define CARRIERGROUP_H_INCLUDED
using namespace std;
class Ship
{
public:
    ///default constructor and destructor to set initialization values
     Ship(){AirCarrierX=0; AirCarrierY=0; Surface_Ship_Z =-50; submarineZ = -400; supply_State = 100.00;}
    ~Ship(){}
    double Z (){return submarineZ;}
    double Ship_Z(){return Surface_Ship_Z;}
    double Supply(){return supply_State;}
    double CarrierX(){return AirCarrierX;}
    double CarrierY(){return AirCarrierY;}

protected:
    double supply_State;//0 to 100 70 means refuel
    double AirCarrierX, AirCarrierY, GuidMisCruiserX, GuidMisCruiserY , DDG_LampsX, DDG_LampsY;
    double DDG_NonLampsX, DDG_NonLampsY, FrigateX, FrigateY, ResupShipX, ResupShipY;
    double Surface_Ship_Z;
    double submarineX, submarineY, submarineZ;

};
/// series of derived classes to initialize ship and sub positions
class GuidMisCruiser : public Ship
{
public:
      double ShipGuidMisCruiserX(double X){
            GuidMisCruiserX = X - 450;
            return GuidMisCruiserX;}

      double ShipGuidMisCruiserY(double Y){

            GuidMisCruiserY = Y + 50;
            return GuidMisCruiserY;}
private:
};

class Frigate : public Ship
{
public:
      double ShipFrigateX(double X){
            FrigateX = X +450;
            return FrigateX;}

      double ShipFrigateY(double Y){
            FrigateY = Y + 50;
            return FrigateY;}
private:
};
class DDG_Lamps : public Ship
{
public:
      double ShipLampsX(double X){
            DDG_LampsX = X + 325;
            return DDG_LampsX;}

      double ShipLampsY(double Y){
            DDG_LampsY = Y - 225;
            return DDG_LampsY;}
private:
};

class DDG_nonLamps : public Ship
{
public:
      double ShipDDG_NonLampsX(double X){
            DDG_NonLampsX = X - 325;
            return DDG_NonLampsX;}

      double ShipDDG_NonLampsY(double Y){

            DDG_NonLampsY = Y + 225;
            return DDG_NonLampsY;}
private:
};

class ResupShip : public Ship
{
public:
      double ShipResupShipX(double X){
            ResupShipX = X + 0;
            return ResupShipX;}

      double ShipResupShipY(double Y){
            ResupShipY = Y - 250;
            return ResupShipY;}
private:
};

class Sub : public Ship
{
public:
      double ShipsubmarineX(double X){
            submarineX = X -50;
            return submarineX;}

      double ShipsubmarineY(double Y){
            submarineY = Y - 270;
            return submarineY;}
private:
};




#endif // CARRIERGROUP_H_INCLUDED
