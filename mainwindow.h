#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot/qcustomplot.h"
int pow2roundup(int x);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void reInterp();

    QVector<double> x1,y1,x,y;
    float fs=48000;

    QCustomPlot *customPlot ;
    QCPItemTracer *phaseTracer;
    QTextEdit *mTextEdit;
      QSpinBox *nbrePts;

    void generateFilter();

public slots:
    void onMousePress(QMouseEvent* m);
    void onMouseRelease(QMouseEvent* m);
    void onMouseMove(QMouseEvent* m);
    void onValueChanged(int val);

    void onInterpCurveCheckBox(bool isChecked);
    void onInterpPatternCheckBox(bool isChecked);
};

#endif // MAINWINDOW_H
