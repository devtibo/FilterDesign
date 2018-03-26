#ifndef WINDOWS_H
#define WINDOWS_H

#include <QString>
#include <QVector>

class Windows
{    
public:
    enum { rectangle =3, hamming =2, hanning = 1 , other = 0 };
    Windows(int,int);
    QVector<double> data;
    QVector<double> getWin();
    double winAvg;
private :

};

#endif // WINDOWS_H
