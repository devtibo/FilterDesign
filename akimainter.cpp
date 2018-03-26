#include "akimainter.h"

AkimaInterp::AkimaInterp()
{

}


void AkimaInterp::setData(QVector<double> Xvec, QVector<double> Yvec)
{

    size = Xvec.length();

    X = (double*)malloc((size+1) * sizeof(double));
    Y = (double*)malloc((size+1) * sizeof(double));
    XM = (double*)malloc((size+4) * sizeof(double));
    Z= (double*)malloc((size+1) * sizeof(double));

    for (int i=0; i<size ;i++)
    {
        X[i+1] = Xvec.at(i);
        Y[i+1] = Yvec.at(i);

    }
}


/*******************************************************
*   Program to demonstrate the Akima spline fitting    *
*       of Function SIN(X) in double precision         *
* ---------------------------------------------------- *
*   Reference: BASIC Scientific Subroutines, Vol. II   *
*   By F.R. Ruckdeschel, BYTE/McGRAWW-HILL, 1981 [1].  *
*                                                      *
*                C++ Version by J.-P. Moreau, Paris.   *
*                       (www.jpmoreau.fr)              *
* ---------------------------------------------------- *
* SAMPLE RUN:                                          *
*                                                      *
* Akima spline fitting of SIN(X):                      *
*                                                      *
*   X   SIN(X) HANDBOOK   AKIMA INTERPOLATION   ERROR  *
* ---------------------------------------------------- *
* 0.00    0.0000000          0.0000000       0.0000000 *
* 0.05    0.0499792          0.0500402      -0.0000610 *
* 0.10    0.0998334          0.0998435      -0.0000101 *
* 0.15    0.1494381          0.1494310       0.0000072 *
* 0.20    0.1986693          0.1986459       0.0000235 *
* 0.25    0.2474040          0.2474157      -0.0000118 *
* 0.30    0.2955202          0.2955218      -0.0000016 *
* 0.35    0.3428978          0.3428916       0.0000062 *
* 0.40    0.3894183          0.3894265      -0.0000081 *
* 0.45    0.4349655          0.4349655       0.0000000 *
* 0.50    0.4794255          0.4794204       0.0000051 *
* 0.55    0.5226872          0.5226894      -0.0000021 *
* 0.60    0.5646425          0.5646493      -0.0000068 *
* 0.65    0.6051864          0.6051821       0.0000043 *
* 0.70    0.6442177          0.6442141       0.0000035 *
* 0.75    0.6816388          0.6816405      -0.0000017 *
* ---------------------------------------------------- *
*                                                      *
*******************************************************/




/*******************************************************
*          Akima spline fitting subroutine             *
* ---------------------------------------------------- *
* The input table is X(i), Y(i), where Y(i) is the     *
* dependant variable. The interpolation point is xx,   *
* which is assumed to be in the interval of the table  *
* with at least one table value to the left, and three *
* to the right. The interpolated returned value is yy. *
* n is returned as an error check (n=0 implies error). *
* It is also assumed that the X(i) are in ascending    *
* order.                                               *
*******************************************************/
double AkimaInterp::interpol_Akima(double xx)  {


    double  n,a,b,yy;

    //Labels: 100,200,300
    int iv = size;
    int i;
    n=1;
    //special case xx=0
    if (xx==0.0) {
        yy=0.0; return Y[1];
    }
    //Check to see if interpolation point is correct
    if (xx<X[1]) {
        n=0 ; return Y[1];
    }

    if (xx>=X[iv]) {
        n=0 ; return Y[iv];
    }

    X[0]=2.0*X[1]-X[2];
    //Calculate Akima coefficients, a and b
    for (i=1; i<iv; i++)
        //Shift i to i+2
        XM[i+2]=(Y[i+1]-Y[i])/(X[i+1]-X[i]);
    XM[iv+2]=2.0*XM[iv+1]-XM[iv];
    XM[iv+3]=2.0*XM[iv+2]-XM[iv+1];
    XM[2]=2.0*XM[3]-XM[4];
    XM[1]=2.0*XM[2]-XM[3];
    for (i=1; i<iv+1; i++)  {
        a=fabs(XM[i+3]-XM[i+2]);
        b=fabs(XM[i+1]-XM[i]);
        if (a+b!=0) goto e100;
        Z[i]=(XM[i+2]+XM[i+1])/2.0;
        goto e200;
e100: Z[i]=(a*XM[i+1]+b*XM[i+2])/(a+b);
e200: ;}
    //Find relevant table interval
    i=0;
e300: i++;
    if (xx>X[i]) goto e300;
    i--;
    //Begin interpolation
    b=X[i+1]-X[i];
    a=xx-X[i];
    yy=Y[i]+Z[i]*a+(3.0*XM[i+2]-2.0*Z[i]-Z[i+1])*a*a/b;
    yy=yy+(Z[i]+Z[i+1]-2.0*XM[i+2])*a*a*a/(b*b);
    return yy;
}
/*
void main()  {
  iv=14;  // Number of pooints in table
  printf("\n Akima spline fitting of SIN(X):\n");
  /* Input sine table
  -----------------------------------------------------------------
   Sine table values from  Handbook of mathematical functions
   by M. Abramowitz and I.A. Stegun, NBS, june 1964
  -----------------------------------------------------------------
  X[1]=0.000; Y[1]=0.00000000; X[2]=0.125; Y[2]=0.12467473;
  X[3]=0.217; Y[3]=0.21530095; X[4]=0.299; Y[4]=0.29456472;
  X[5]=0.376; Y[5]=0.36720285; X[6]=0.450; Y[6]=0.43496553;
  X[7]=0.520; Y[7]=0.49688014; X[8]=0.589; Y[8]=0.55552980;
  X[9]=0.656; Y[9]=0.60995199; X[10]=0.721; Y[10]=0.66013615;
  X[11]=0.7853981634; Y[11]=0.7071067812;
  X[12]=0.849; Y[12]=0.75062005; X[13]=0.911; Y[13]=0.79011709;
  X[14]=0.972; Y[14]=0.82601466;
  //----------------------------------------------------------------
  // print header
  printf("\n X   SIN(X) HANDBOOK   AKIMA INTERPOLATION   ERROR  \n");
  printf("----------------------------------------------------\n");
  // main loop
  xx=0.0;
  for (i=1; i<17; i++) {
    Interpol_Akima();
    printf("%4.2f    %9.7f          %9.7f      %10.7f\n",xx,sin(xx),yy,(sin(xx)-yy));
    xx += 0.05;
  }
  //print footer
  printf("----------------------------------------------------\n");
}

// End of file Akima.cpp
*/
