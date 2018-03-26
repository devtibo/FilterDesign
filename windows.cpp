#include "windows.h"

Windows::Windows(int winName, int N)
{
    winAvg = 0.0;
    switch(winName)
    {

    case Windows::rectangle :
        for (int i=0; i <N;i++)
            data.append(1);
        winAvg = 1;
        break;


    case Windows::hanning :
        for (int i=0; i <N;i++)
        {
            data.append(0.5 *(1.0 - cos( (2.0*M_PI * i)/(N-1.0))));
            winAvg += data.at(i)/N;
        }
        break;


    case Windows::hamming :
        for (int i=0; i <N;i++)
        {
            data.append(0.54 - 0.46*cos( (2.0*M_PI * i)/(N-1.0)));
            winAvg += data.at(i)/N;
        }
        break;

    }
}

QVector<double> Windows::getWin()
{
    return data;
}

