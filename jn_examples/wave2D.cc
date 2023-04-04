//  g++ -std=c++11 -o wave2d wave2D.c
//  ./wave2d 45

#include <iostream>
#include <fstream>
#include <math.h>
#include <cstdlib>   // for rand() on archer.
#include <iomanip>   // for setw() in cout, 
#include <cstdlib>   // for rand() on archer and atoi()

using namespace std;

const int nx=101, ny=101, nt=3; //  define the size of array.

// ===================================================================
void write2DWave(ofstream & ofs, int time, double u[nx][ny][nt]) {

     int kt=time;

     // if(kt>0) {
     //  ofs<<"  "<<endl;   // two empty lines for annimation.
     //   ofs<<"  "<<endl;
     // }

     if(kt>2) kt=2;
     for (int j=0; j<ny; j=j+2 ) {
        for(int i=0; i<nx; i=i+2 ) {
           ofs<<"  "<<time<<","<<i<<","<<j<<","<<u[i][j][kt]<<endl;
        }   //for gnuplot animation
     }

   return;
}

// ===================================================================
//   create a wave at t0
// ===================================================================
void wave2Dinitial(double u[nx][ny][nt],double & ratio){
   double pi=acos(-1.0);
   cout<<"  pi="<<pi<<endl;
   double amplitude=1.0;
   double tension=180.0;
   double density=390.0;
   double dx=pi/100.0;
   double dy=pi/100.0;
   double c=sqrt(tension/density);  //  propagation speed
   double cprime=c;
   double r=c/cprime;
   ratio=0.5*r*r;   //  0.5 for stability

   // create intitail wave with double sine curves...
   double y=0.0;
   for (int j=0; j<ny; j++) {
      double x=0.0;
      for (int i=0; i<nx; i++) {
         u[i][j][0]=amplitude*sin(2.0*x)*sin(y);
         x=x+dx;
      }
      y=y+dy;
   }   // end of loop over y.
}

// ===================================================================
//   propagate a wave from t+2 based on the wave at t1 and t0...
// ===================================================================
void wave2Dpropagate(double u[nx][ny][nt],double ratio,int tmax){


   ofstream outfileA;
   outfileA.open("wave2Dfull.csv");

   ofstream outfileB;
   outfileB.open("wave2Dfirst.csv");

   for (int j=1; j<(ny-1); j++ )  {                     // First time step
     for (int i=1; i<(nx-1); i++ )  {
       u[i][j][1] = u[i][j][0] +0.5*ratio*(u[i+1][j][0]+u[i-1][j][0]
            +u[i][j+1][0]+u[i][j-1][0]-4.0*u[i][j][0]) ;
     }
   }  // end of loop for the first time step

     //     === write out the wave at t=0 and t=1 ===
     outfileB<<"t,x,y,z"<<endl;       //  write one line headder in csv.
     outfileA<<"t,x,y,z"<<endl;       //  write one line headder in csv.

     write2DWave(outfileB,0,u);
     write2DWave(outfileA,0,u);
     write2DWave(outfileA,1,u);

   //   loop over time steps...
   for (int k=2; k<=tmax; k++ )   {                       // Later times
     //   propagate wave to t2 based on the wave at t1 and t0.
     //   four outer boudaries (j=0, j=ny, i=0, i=nx) are fixed, i.e. no update.
     for (int j=1; j<(ny-1); j++ )  {
       for (int i=1; i<(nx-1); i++ )  {
         u[i][j][2] = 2.*u[i][j][1] - u[i][j][0]+ratio*(u[i+1][j][1]
         + u[i-1][j][1] + u[i][j+1][1]+u[i][j-1][1] - 4.*u[i][j][1]);
       }
     }  // end of j, i, loop..

     write2DWave(outfileA,k,u);

     //   preparation for next time step...
     for (int j=0; j<nx; j++ ){
       for(int i=0; i<ny; i++ ){
         u[i][j][0] = u[i][j][1];                        // New past
         u[i][j][1] = u[i][j][2];                     // New present
       }
     }
   }  //  end of loop over time steps. 
   
   outfileA.close();
   outfileB.close();
}

// ===================================================================
int main(int argc, char* argv[])
{
   // check the input argument.
   if(argc<2) {
      cerr<<"Usage:  "<<argv[0]<<"  n "<<endl;
      cerr<<"where n is the number of time steps, e.g. 45 "<<endl;
      return 1;
   }  

   int tmax=atoi(argv[1]);
   cout<<"The number of steps is "<<tmax;

   // const int nx=11, ny=11, nt=3;
   double u[nx][ny][nt]={0};
   double ratio;

   wave2Dinitial(u,ratio);
   cout<<"  ratio="<<ratio<<endl;

   wave2Dpropagate(u,ratio,tmax);

   return 0;
}
