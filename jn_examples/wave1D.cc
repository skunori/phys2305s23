//  g++ -std=c++11 -o wave1d wave1D.cc
//  ./wave1d  200  1
//  (1st argument:  number of time steps)
//  (2nd argument:  1 for string wave, 2 for sine wave)

#include <iostream>
#include <fstream>
#include <math.h>
#include <cstdlib>   // for rand() on archer.
#include <iomanip>   // for setw() in cout, 

using namespace std;

const int nx=101, nt=3; //  define the size of array.


// ====================================================================
void writeWave(ofstream & ofs, int time, double xi[nx][nt]) {
      int kt=time;

      if(kt>2) kt=2;
      for(int i=0; i<nx; i++) {
         float tval=time;
         float xval=i;    //  100 cm long stirng
         float yval=xi[i][kt];       //  string displacement
         ofs<<tval<<","<<xval<<","<<yval<<endl;
      }

   return;
}

// ====================================================================
//   intial candition for a wave on a string at t=0.
// ====================================================================
void wave1Dstringwave(double xi[nx][nt],double & ratio){

   double xplunk=80.;   //  plunk position x
   double yplunk=0.1;   //  plunk distance y

   double rho=0.01;      // density
   double tension=40.0;  // tension
   double c=sqrt(tension/rho);    // propagation speed
   double c1=c;
   double r=(c*c)/(c1*c1);
   ratio=r*1.0;

   double slope1=yplunk/xplunk;   // slope of the string on the left side.
   double slope2=yplunk/(100.0-xplunk);  // slope on the right side.
   cout<<"   slope1="<<slope1<<"   slope2="<<slope2<<endl;

   for(int i=0; i<nx; i++) {
     int jplunk=xplunk;     // convert to integer.
     if(i<jplunk) {xi[i][0]=slope1*i;}
     else {xi[i][0]=yplunk-slope2*(i-jplunk);}
   }
}

// ====================================================================
//   initial sine wave at t=0.
// ====================================================================
void wave1Dsinewave(double xi[nx][nt],double & ratio){
   double amplitude=0.1;
   double pi=acos(-1.0);   // calculate pi using arccosine
   for(int i=0; i<nx; i++) {
      double theta=(4.0*pi/100.)*i;
      xi[i][0]=amplitude*sin(theta);
   }
   ratio=1.0;
}

// ====================================================================
//   propagate wave
// ====================================================================
void wave1Dpropagate(double xi[nx][nt],double ratio,int tmax){

   // condition for t=1.
   // note that both end of the string is fixed. So, no update for i=0 and i=nx.
   for(int i=1; i<(nx-1); i++) {
       xi[i][1] = xi[i][0] + 0.5*ratio*(xi[i+1][0]+xi[i-1][0]-2.0*xi[i][0]);
   }

   //   calculate through time steps

   ofstream outfileA;
   outfileA.open("wave1Dfull.csv");

   ofstream outfileB;
   outfileB.open("wave1Dfirst.csv");

   outfileB<<"t,x,y"<<endl;       // one line header in csv file
   outfileA<<"t,x,y"<<endl;       // one line header in csv file

   writeWave(outfileB, 0, xi);
   writeWave(outfileA, 0, xi);
   writeWave(outfileA, 1, xi);

   //   step through time steps.
   for(int t=2; t<tmax; t++) {

      //   update the wave for next time (t=2).
      //   note that both ends (t=0 and t=nx) are fixed, i.e. no update.
      for(int i=1; i<(nx-1); i++) {
         xi[i][2] = 2.*xi[i][1] - xi[i][0] + ratio*(xi[i+1][1]+xi[i-1][1]-2*xi[i][1]);
      }

      //   write x, y...
      int noutFrequency=1.0/ratio;
      if(noutFrequency<0) noutFrequency=1;
      if((t%noutFrequency)==0) {
          writeWave(outfileA, t, xi);
            //   write two empty lines to separate points for each time step
      }
      //   for animation.
      // recycle array
      for(int i=0; i<nx; i++) {
         xi[i][0]=xi[i][1];
         xi[i][1]=xi[i][2];
      }

   }  //  end of loop over t (time steps).

   outfileA.close();
   outfileB.close();
}

// ====================================================================
int main(int argc, char* argv[])
{
   // check the input argument.
   if(argc<2) {
      cerr<<"Error... Error...  An input parameter is required..."<<endl;
      cerr<<"Usage:  "<<argv[0]<<"  n  k "<<endl;
      cerr<<"where n is the number of time steps, e.g. 200 "<<endl;
      cerr<<"and k=1 for string wave and k=2 for sine wave "<<endl;
      return 1;
   } 

   int tmax=atoi(argv[1]);
   cout<<"total time steps is "<<tmax<<endl;

   int iswitch=atoi(argv[2]);
   if (iswitch==1) {cout<<"String wave will be generated"<<endl;}
   else if (iswitch==2) {cout<<"String wave will be generated"<<endl;}
   else     {cout<<"Error:   second input argument should be 1 or 2."<<endl; return 1;}

   // const int nx=11, ny=11, nt=3;
   double xi[nx][nt]={0};
   double ratio;

   //  define the intial condition of string for string-wave

   cout<<"  iswitch "<<iswitch<<endl;
   switch(iswitch)
   {
      case 1 : wave1Dstringwave(xi,ratio); break;  //  sting wave
      case 2 : wave1Dsinewave(xi,ratio); break;    //  sine wave
      default: { cout<<"Error for switch"<<endl; return 1;};
   }

   cout<<"  ratio="<<ratio<<endl;

   wave1Dpropagate(xi,ratio,tmax);

  return 0;
}
