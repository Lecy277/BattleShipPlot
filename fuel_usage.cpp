#include <list>
#include <iostream>
#include <fstream>
#include <random>
#include <ctime>
#include <cmath>
#include <string>
#include <sstream>
#include "CarrierGroup.h"
using namespace std;
/// in other options, compiler settings -std=gnu++11
///only necessary if your C++ 11 does not find to_string
string to_string(int i)
{
    stringstream ss;
    ss<<i;
    return ss.str();
}
/// built into c++ 11 as to_string()
int main()
{
    double cv_x, cv_y;  ///x,y location of ships and resupply ship
    double bearing =270;
    double speed, dbearing, drange;  ///relative bearing
    double heading=0.0;
    double bearing_act, range_act, bowangle;
    double range = 10000.0/2025;
    double deltar = .05;
    double PI = 3.14156;
    double dx, dy;
    double theta = 0.0; ///course correction variable
    double alpha = 0.0; /// course correction angle
    double f=0.0;///amount of fuel spent per time step
    double Supply_total=0.0;///amount of fuel
    int choice;/// int while loop menu choice
    int t=1;///variables for my for loops later on
    int n=0;/// int's for loops below
    int j,y,c;/// int's for loops below
    /// objects for my classes
    Ship ZAxis;
    DDG_Lamps shippy;
    DDG_nonLamps shippyNon;
    GuidMisCruiser shippyG;
    Frigate shippyF;
    ResupShip shippyRe;
    Sub shippySub;
    /// setting value for my carrier
    cv_x=ZAxis.CarrierX();
    cv_y=ZAxis.CarrierY();
    /// declaring my two lists
    list<double>The_ship;
    list<double>::iterator sitr;
    list<double>The_ship1;
    list<double>::iterator sitr1;

    ///create the heterogeneous linked list
    The_ship.push_back(ZAxis.CarrierX());
    The_ship.push_back(shippy.ShipLampsX(cv_x));
    The_ship.push_back(shippyNon.ShipDDG_NonLampsX(cv_x));
    The_ship.push_back(shippyG.ShipGuidMisCruiserX(cv_x));
    The_ship.push_back(shippyF.ShipFrigateX(cv_x));
    The_ship.push_back(shippySub.ShipsubmarineX(cv_x));
    The_ship.push_back(shippyRe.ShipResupShipX(cv_x));
    The_ship.push_back(shippyRe.ShipResupShipX(cv_x)+85);

    The_ship1.push_back(ZAxis.CarrierY());
    The_ship1.push_back(shippy.ShipLampsY(cv_y));
    The_ship1.push_back(shippyNon.ShipDDG_NonLampsY(cv_y));
    The_ship1.push_back(shippyG.ShipGuidMisCruiserY(cv_y));
    The_ship1.push_back(shippyF.ShipFrigateY(cv_y));
    The_ship1.push_back(shippySub.ShipsubmarineY(cv_y));
    The_ship1.push_back(shippyRe.ShipResupShipY(cv_y));
    The_ship1.push_back(shippyRe.ShipResupShipY(cv_y)-10);

    /// generate random modification to bearing and range (dbearing, drange)
    /// max bearing variance = +-5 degrees
    /// max range variance = +- 5% of ideal range
    /// consider 3:2:1 rule

    ///a random engine generates a number between 0.0 and 1.0
    default_random_engine gen(time(NULL));
    ///time(NULL) as arg to gen() replaces the old srand()

    ///now we fit the 0 to 1.0 value into a distribution
    normal_distribution<double> bdist(0,5.0/2.0);
    normal_distribution<double> rdist(0,deltar*range/2);

    string fname, fSub, fCarrier, fResup, outfile;
    fstream fout, fout2, foutSub,foutResup ,foutCarrier;

    ///this cpp file will CREATE the command file it will access.
    fout2.open("command.txt",ios::out);
    fout2<<"set view (80 , 300 , 50 , 60 )"<<endl;
    fout2<<"set yrange [-1300:1300]"<<endl;
    fout2<<"set xrange [-1300:1300]"<<endl;
    fout2<<"set zrange [-600:600]"<<endl;
    fout2<<"set xlabel \"X Axis\""<<endl;
    fout2<<"set ylabel \"Y Axis\""<<endl;
    fout2<<"set zlabel \"Z Axis\""<<endl;
    fout2<<"set mapping cartesian"<<endl;
    fout2<<"set angles degrees"<<endl;
    fout2<<"set terminal png"<<endl;

    cout<<"initial position for carrier:"<<endl;
    cout<<"carrier (cvn) ("<<ZAxis.CarrierX()<<", "<<ZAxis.CarrierY()<<")"<<endl;
    cout<<"\nEnter speed between 10 to 30 knots carrier knots: ";
    cin>>speed;
    while (speed<10 || speed >30)
    {
        cout<<"INCORRECT SPEED INPUT TRY AGAIN RANGE BETWEEN 10 to 30 knots :";
        cin>>speed;
    }
    ///if statements for fuel levels left based on speed input
    if(speed >= 25)
    {
        f= 20;
    }
    else if(speed >= 20 && speed < 25)
    {
        f= 15;
    }
    else if(speed >= 15 && speed < 20)
    {
        f= 10;
    }
    else if(speed >= 10 && speed < 15)
    {
        f= 5;
    }
  ///while loop to go through menu
  do{
    cout<<"Enter 1 to continue 0=exit:  ";
    cin >> choice;
    if (choice==1)
    {
    ///input course correction
    cout<<endl<<"input course correction (-360)to 360 :";
    cin>>theta;

    while (theta<-360 || theta >360)
    {   ///while loop if course correct incorrect input
        cout<<"INCORRECT COURSE INPUT TRY AGAIN RANGE BETWEEN 0 to 360 DEGREES :";
        cin>>theta;
    }
        heading = heading + theta;
        ///converts navy heading trig ref angle
        alpha = 360 - bearing - theta;
        theta = 360-theta;
        dx = speed*cos(theta*PI/180.0);
        dy = speed*sin(theta*PI/180.0);

        ///initial heading after input
        for (sitr=The_ship.begin(); sitr!=The_ship.end(); ++sitr){
            *sitr=*sitr +dx;
        }
        for (sitr1=The_ship1.begin(); sitr1!=The_ship1.end(); ++sitr1){
            *sitr1=*sitr1 +dy;///added this in afterwards
        }

        dbearing = bdist(gen);
        drange = rdist(gen);
        cout<<endl<<"modifying position"<<endl;
        cout<<"delta bearing: "<<dbearing<<endl;
        bearing_act=bearing+dbearing;
        range_act = speed + drange;
        bowangle = (PI/180)*(90-abs(dbearing));
        dx = cos(bowangle)*range_act;
        dy = sin(bowangle)*range_act;

        /// for loops to put in the coarse correct for the lists based on bearing
        if(dbearing<0){
            for (sitr=The_ship.begin(); sitr!=The_ship.end(); ++sitr){
                *sitr=*sitr - (dx/2025);
            }
        }
        else{
            for (sitr=The_ship.begin(); sitr!=The_ship.end(); ++sitr){
                *sitr=*sitr + (dx/2025);
            }
        }
        for (sitr1=The_ship1.begin(); sitr1!=The_ship1.end(); ++sitr1){
            *sitr1=*sitr1 +dy;
        }

        ///modifying from nautical miles and yards
        cout<<"new bearing actual: "<< bearing_act<<endl;
        cout<<"new range actual: "<<range_act<<endl;
        ///modifying from nautical miles and yards
        cout<<"positions after "<<t <<  " time steps"<<endl;

        ///sets total fuel supply based on speed and time steps
        if(t<9){
            Supply_total=ZAxis.Supply() -(f*t);
        }
        else if (t>9)
        {
            Supply_total=ZAxis.Supply() -(f*(t-c));
        }
        ///coarse of the fleet not refueling
        if (Supply_total>=70.00)
        {   fname="position_Ship"+to_string(t)+".dat";
            fSub="position_Sub"+to_string(t)+".dat";
            fCarrier="position_Carrier"+to_string(t)+".dat";
            fResup="position_Resupply"+to_string(t)+".dat";
            outfile="position_"+to_string(t)+".png";
            ///we take the string, turn it into a char []
            ///so that we can use it as a filename for output
            fout.open(fname.c_str(),ios::out);
            foutSub.open(fSub.c_str(),ios::out);
            foutCarrier.open(fCarrier.c_str(),ios::out);
            foutResup.open(fResup.c_str(),ios::out);
            fout2<<"set output \'"<<outfile<<"\'"<<endl;
            ///counts through list to output ships to correct .dat files while refueling
            sitr++;
            sitr1++;
            foutCarrier<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
            sitr++;
            sitr1++;
            fout<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
            sitr++;
            sitr1++;
            fout<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
            sitr++;
            sitr1++;
            fout<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
            sitr++;
            sitr1++;
            fout<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
            sitr++;
            sitr1++;
            foutSub<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Z()<<endl;
            sitr++;
            sitr1++;
            foutResup<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
            sitr++;
            sitr1++;
            fout2<<"splot \'"<<fCarrier<<"\' lt 6 lw 9 lc rgb \"green\",\'"<<fResup<<"\' lt 6 lw 9 lc rgb \"red\",\'"
            <<fSub<<"\' lt 6 lw 9 lc rgb \"yellow\",\'"<<fname<<"\' lt 6 lw 9 lc rgb \"blue\""<<endl;
            fout.close();
            foutSub.close();
            foutCarrier.close();
            foutResup.close();
        }
        ///coarse of the fleet when fuel refueling
        else if (Supply_total<70.00)
        {   if(t>9){
                n=0;
            }
            /// for loops that sets coarse while refueling and out put to dat files and command for 2 time steps
            for(n=n+t;n<2+t; n++)
                { ///name, number and open all out files
                fname="position_Ship"+to_string(n)+".dat";
                fSub="position_Sub"+to_string(n)+".dat";
                fCarrier="position_Carrier"+to_string(n)+".dat";
                fResup="position_Resupply"+to_string(n)+".dat";
                outfile="position_"+to_string(n)+".png";
                fout.open(fname.c_str(),ios::out);
                foutSub.open(fSub.c_str(),ios::out);
                foutCarrier.open(fCarrier.c_str(),ios::out);
                foutResup.open(fResup.c_str(),ios::out);
                fout2<<"set output \'"<<outfile<<"\'"<<endl;
                for (sitr=The_ship.begin(); sitr!=The_ship.end(); ++sitr){
                    *sitr=*sitr +13;
                }
                for (sitr1=The_ship1.begin(); sitr1!=The_ship1.end(); ++sitr1){
                    *sitr1=*sitr1 +13;
                }
                ///counts through list to output ships to correct .dat files while refueling
                sitr++;
                sitr1++;
                foutCarrier<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                sitr++;
                sitr1++;
                fout<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                fout<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                fout<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                foutSub<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Z()<<endl;
                sitr++;
                sitr1++;
                foutResup<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                foutResup<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                ///close all out files
                fout.close();
                foutSub.close();
                foutCarrier.close();
                foutResup.close();
                ///output to command file to set color, shape and size for .dat files to create png's
                fout2<<"splot \'"<<fCarrier<<"\' lt 6 lw 9 lc rgb \"green\",\'"<<fResup<<"\' lt 6 lw 9 lc rgb \"red\",\'"
                <<fSub<<"\' lt 6 lw 9 lc rgb \"yellow\",\'"<<fname<<"\' lt 6 lw 9 lc rgb \"blue\""<<endl;
             }
             /// for loops that sets coarse while refueling and out put to dat files and command for 2 time steps
            for(j=n; j<n+2; j++)
             {  ///name, number and open all out files
                fname="position_Ship"+to_string(j)+".dat";
                fSub="position_Sub"+to_string(j)+".dat";
                fCarrier="position_Carrier"+to_string(j)+".dat";
                fResup="position_Resupply"+to_string(j)+".dat";
                outfile="position_"+to_string(j)+".png";
                fout.open(fname.c_str(),ios::out);
                foutSub.open(fSub.c_str(),ios::out);
                foutCarrier.open(fCarrier.c_str(),ios::out);
                foutResup.open(fResup.c_str(),ios::out);
                fout2<<"set output \'"<<outfile<<"\'"<<endl;
                ///sets list to go straight and maintain 13 knots while refueling
                for (sitr=The_ship.begin(); sitr!=The_ship.end(); ++sitr){
                    *sitr=*sitr +13;
                }
                for (sitr1=The_ship1.begin(); sitr1!=The_ship1.end(); ++sitr1){
                    *sitr1=*sitr1 +13;
                }
                ///counts through list to output ships to correct .dat files while refueling
                sitr++;
                sitr1++;
                foutCarrier<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                fout<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                fout<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                fout<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                sitr++;
                sitr1++;
                foutSub<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Z()<<endl;
                sitr++;
                sitr1++;
                /// moves ship next to supply ship to refuel and changes ship from blue to red
                foutResup<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                foutResup<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                ///close all out files
                fout.close();
                foutSub.close();
                foutCarrier.close();
                foutResup.close();
                ///output to command file to set color, shape and size for .dat files to create png's
                fout2<<"splot \'"<<fCarrier<<"\' lt 6 lw 9 lc rgb \"green\",\'"<<fResup<<"\' lt 6 lw 9 lc rgb \"red\",\'"
                <<fSub<<"\' lt 6 lw 9 lc rgb \"yellow\",\'"<<fname<<"\' lt 6 lw 9 lc rgb \"blue\""<<endl;
                }
            /// for loops that sets coarse while refueling and output to dat files and command for 2 time steps
            for(y=j; y<j+2; y++)
             {  ///name, number and open all out files
                fname="position_Ship"+to_string(y)+".dat";
                fSub="position_Sub"+to_string(y)+".dat";
                fCarrier="position_Carrier"+to_string(y)+".dat";
                fResup="position_Resupply"+to_string(y)+".dat";
                outfile="position_"+to_string(y)+".png";
                fout.open(fname.c_str(),ios::out);
                foutSub.open(fSub.c_str(),ios::out);
                foutCarrier.open(fCarrier.c_str(),ios::out);
                foutResup.open(fResup.c_str(),ios::out);
                fout2<<"set output \'"<<outfile<<"\'"<<endl;
                ///sets list to go straight and maintain 13 knots while refueling
                for (sitr=The_ship.begin(); sitr!=The_ship.end(); ++sitr){
                    *sitr=*sitr +13;
                }
                for (sitr1=The_ship1.begin(); sitr1!=The_ship1.end(); ++sitr1){
                    *sitr1=*sitr1 +13;
                }
                ///counts through list to output ships to correct .dat files while refueling
                sitr++;
                sitr1++;
                foutCarrier<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                fout<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                fout<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                sitr++;
                sitr1++;
                fout<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                foutSub<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Z()<<endl;
                sitr++;
                sitr1++;
                /// moves ship next to supply ship to refuel and changes ship from blue to red
                foutResup<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                foutResup<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                ///close all out files
                fout.close();
                foutSub.close();
                foutCarrier.close();
                foutResup.close();
                ///output to command file to set color, shape and size for .dat files to create png's
                fout2<<"splot \'"<<fCarrier<<"\' lt 6 lw 9 lc rgb \"green\",\'"<<fResup<<"\' lt 6 lw 9 lc rgb \"red\",\'"
                <<fSub<<"\' lt 6 lw 9 lc rgb \"yellow\",\'"<<fname<<"\' lt 6 lw 9 lc rgb \"blue\""<<endl;
             }
             /// for loops that sets coarse while refueling and out put to dat files and command for 2 time steps
             for(c=y; c<y+2; c++)
             {  ///name, number and open all out files
                fname="position_Ship"+to_string(c)+".dat";
                fSub="position_Sub"+to_string(c)+".dat";
                fCarrier="position_Carrier"+to_string(c)+".dat";
                fResup="position_Resupply"+to_string(c)+".dat";
                outfile="position_"+to_string(c)+".png";
                fout.open(fname.c_str(),ios::out);
                foutSub.open(fSub.c_str(),ios::out);
                foutCarrier.open(fCarrier.c_str(),ios::out);
                foutResup.open(fResup.c_str(),ios::out);
                fout2<<"set output \'"<<outfile<<"\'"<<endl;
                ///sets list to go straight and maintain 13 knots while refueling
                for (sitr=The_ship.begin(); sitr!=The_ship.end(); ++sitr){
                    *sitr=*sitr +13;
                }
                for (sitr1=The_ship1.begin(); sitr1!=The_ship1.end(); ++sitr1){
                    *sitr1=*sitr1 +13;
                }
                ///counts through list to output ships to correct .dat files while refueling
                sitr++;
                sitr1++;
                foutCarrier<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                fout<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                sitr++;
                sitr1++;
                fout<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                fout<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                foutSub<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Z()<<endl;
                sitr++;
                sitr1++;
                /// moves ship next to supply ship to refuel and changes ship from blue to red
                foutResup<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                foutResup<<*sitr<<" "<<*sitr1<<" "<<ZAxis.Ship_Z()<<endl;
                sitr++;
                sitr1++;
                ///close all out files
                fout.close();
                foutSub.close();
                foutCarrier.close();
                foutResup.close();
                ///output to command file to set color, shape and size for .dat files to create png's
                fout2<<"splot \'"<<fCarrier<<"\' lt 6 lw 9 lc rgb \"green\",\'"<<fResup<<"\' lt 6 lw 9 lc rgb \"red\",\'"
                <<fSub<<"\' lt 6 lw 9 lc rgb \"yellow\",\'"<<fname<<"\' lt 6 lw 9 lc rgb \"blue\""<<endl;
             }
            ///reset fuel totals back to 100 after all ships refueled
            Supply_total=ZAxis.Supply();
            t=c-1;
        }
        t++;
    }

   }while(choice !=0);///end duel while loop
    fout2.close();
    ///output commands to command file and create png files
    cout<<"command.txt complete"<<endl;
    system("gnuplot command.txt");
    return 0;

}
