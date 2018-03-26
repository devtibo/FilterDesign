#ifndef AKIMAINTERP_H
#define AKIMAINTERP_H
#include <stdio.h>
#include <math.h>
#include <QVector>

class AkimaInterp
{
public:
    AkimaInterp();
    void setData(QVector<double> Xvec, QVector<double> Yvec);
    double interpol_Akima(double xx);




    double *X, *Y, *XM, *Z;
    int size;
};

#endif // AKIMAINTERP_H
