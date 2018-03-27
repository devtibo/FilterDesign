#include "mainwindow.h"

#include <QVBoxLayout>

#include "akimainter.h"


#include "windows.h"
#include <fftw3.h>

// NOTE  : Valide pour un nombre de points impair !


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    /* INIT DATA */
    /* ----------*/

    x1.append(0);
    x1.append(1/4.0 * 48000/2.0);
    x1.append(2/4.0 * 48000/2.0);
    x1.append(3/4.0 * 48000/2.0);
    x1.append(4/4.0 * 48000/2.0);


    y1.append(1);
    y1.append(1);
    y1.append(0);
    y1.append(0);
    y1.append(0);


    for (int i=0; i< 200;i++)
    {
        double tmp;
        tmp=x1.first() + (x1.last()-x1.first())/(200.0-1) * i;
        x.append(tmp);
    }


    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *panelLayout = new QHBoxLayout;

    /* CUSTOMPLOT */
    /*-----------*/
    customPlot = new QCustomPlot;
    // create and configure plottables:
    QCPGraph *graph1 = customPlot->addGraph();
    graph1->setData(x1, y1);
    graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    graph1->setPen(QPen(QColor(120, 120, 120), 2));

    graph1->setLineStyle(QCPGraph::lsStepCenter);


    QCPGraph *graph2 = customPlot->addGraph();
    graph2->setData(x1, y1);
    //graph2->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    graph2->setPen(QPen(QColor(120, 255, 255), 1, Qt::DashLine));

    QCPGraph *graph3 = customPlot->addGraph();
    graph3->setPen(QPen(QColor(127, 0, 255), 3));

    QCPGraph *graph4 = customPlot->addGraph();
    graph4->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, QPen(QColor(120, 255, 255), 1.5), QBrush(Qt::white), 12));
    graph4->setPen(Qt::PenStyle::NoPen);



    /*
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->legend->setSelectedFont(legendFont);
    ui->customPlot->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items

*/
    // move bars above graphs and grid below bars:
    customPlot->addLayer("abovemain", customPlot->layer("main"), QCustomPlot::limAbove);
    customPlot->addLayer("belowmain", customPlot->layer("main"), QCustomPlot::limBelow);
    graph1->setLayer("abovemain");
    customPlot->xAxis->grid()->setLayer("belowmain");
    customPlot->yAxis->grid()->setLayer("belowmain");

    // set some pens, brushes and backgrounds:
    customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    customPlot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    //axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(0, QColor(30, 30, 30));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    customPlot->axisRect()->setBackground(axisRectGradient);

    customPlot->xAxis->setLabel("Frequency (kHz)");
    customPlot->yAxis->setLabel("Magnitude (dB)");
    customPlot->xAxis->setLabelColor(Qt::white);
    customPlot->yAxis->setLabelColor(Qt::white);


    /** Legend **/
    customPlot->legend->setVisible(true);
    customPlot->legend->setFillOrder(QCPLegend::foColumnsFirst);
    graph1->setName("Non-Uniform Pattern");
    graph2->setName("Interp. Pattern");
    graph3->setName("Filter Response");
    graph4->setName("Uniform Pattern (for compute)");
    QCPLayoutGrid *subLayout = new QCPLayoutGrid;
    customPlot->plotLayout()->addElement(1, 0, subLayout);
    subLayout->setMargins(QMargins(5, 0, 5, 5));
    subLayout->addElement(0, 0, customPlot->legend);
    // set legend's row stretch factor very small so it ends up with minimum height:
    customPlot->plotLayout()->setRowStretchFactor(1, 0.001);
    customPlot->legend->setBrush(QBrush(QColor(80, 80, 80)));
    customPlot->legend->setTextColor(Qt::white);



    customPlot->rescaleAxes();
    customPlot->yAxis->setRange(-1, 1.5);
    customPlot->xAxis->setRange(0, fs/2);
    customPlot->setMinimumHeight(300);
    customPlot->setMinimumWidth(500);

    connect(customPlot,SIGNAL(mousePress(QMouseEvent*)),this,SLOT(onMousePress(QMouseEvent*)));
    connect(customPlot,SIGNAL(mouseRelease(QMouseEvent*)),this,SLOT(onMouseRelease(QMouseEvent*)));

    phaseTracer = new QCPItemTracer(customPlot);
    this->phaseTracer->setInterpolating(false);
    this->phaseTracer->setStyle(QCPItemTracer::tsSquare);
    this->phaseTracer->setPen(QPen(Qt::yellow));
    this->phaseTracer->setBrush(Qt::yellow);
    this->phaseTracer->setSize(10);

    //phaseTracer->setVisible(false);

    phaseTracer->setGraph(customPlot->graph(0));
    reInterp();

    customPlot->replot();


    /* LABEL FOR NOTES */
    /*----------------*/
    QLabel *mHelp = new QLabel();
    QString mHelpStr;
    mHelpStr  = "* Left Mouse Button  : Drag points \n";
    mHelpStr += "* Right Mouse Button : Add a point\n";
    mHelpStr += "* Right Mouse Button + [CTRL] : Remove point";
    mHelp->setText(mHelpStr);
    mHelp->setMaximumHeight(100);


    /* TEXT EDIT FOR OUPUT */
    /*--------------------*/
    QTabWidget *mTabWidget = new QTabWidget;
mTabWidget->setMaximumWidth(220);
mTabWidget->setMinimumWidth(220);

    mTextEditCoef = new QTextEdit;
    mTextEditCoef->setReadOnly(true);
    mTextEditCoef->setMaximumWidth(220);

    mTextEditPts = new QTextEdit;
    mTextEditPts->setReadOnly(true);
    mTextEditPts->setMaximumWidth(220);

    mTabWidget->addTab(mTextEditCoef,"Coefficients");
    mTabWidget->addTab(mTextEditPts,"Template Points");


    /* SPINEBOX */
    /*-----------*/
    nbrePts  =new QSpinBox;
    nbrePts->setRange(0,500);
    nbrePts->setValue(5);
    nbrePts->setSingleStep(2);

    connect(nbrePts,SIGNAL(valueChanged(int)),this,SLOT(onValueChanged(int)));


    QCheckBox *interpCurveCheckBox = new QCheckBox("Show Interpolated Curve");
    QCheckBox *interpPatternCheckBox = new QCheckBox("Show Interpolated Pattern");
    interpCurveCheckBox->setChecked(true);
    interpPatternCheckBox->setChecked(true);
    connect(interpCurveCheckBox,SIGNAL(clicked(bool)),this,SLOT(onInterpCurveCheckBox(bool)));
    connect(interpPatternCheckBox,SIGNAL(clicked(bool)),this,SLOT(onInterpPatternCheckBox(bool)));

    /* FILL WINDOWS */
    /*--------------*/
    QFrame *mFrame = new QFrame;
    panelLayout->addLayout(mainLayout);
    panelLayout->addWidget(mTabWidget);
    mainLayout->addWidget(customPlot);
    mainLayout->addWidget(mHelp);
    mainLayout->addWidget(nbrePts);
    mainLayout->addWidget(interpCurveCheckBox);
    mainLayout->addWidget(interpPatternCheckBox);
    mFrame->setLayout(panelLayout);

    setCentralWidget(mFrame);

    generateFilter();
}

void MainWindow::generateFilter()
{

    QVector<double> yUniform,xUniform;
    int L=y1.size()*2-1;
    int nbrePointsOnGraph;
    if(y1.size()%2==0)
        nbrePointsOnGraph= y1.size()+1;
    else
        nbrePointsOnGraph= y1.size();

    nbrePts->setRange(nbrePointsOnGraph,500);
    nbrePointsOnGraph = qMax(nbrePts->value(),nbrePointsOnGraph);



    int Nfft = nbrePointsOnGraph*2-1;

    QString ptsStr;
    /* Select uniform points*/
    for (float i=0;i<nbrePointsOnGraph;i++)
    {
        float indx;
        indx = (1.0*i)/ (nbrePointsOnGraph-1);
        yUniform.append(y.at(round( indx* (1.0*y.size()-1.0))));
        xUniform.append(x.at(round( indx* (1.0*x.size()-1.0))));
        qDebug("%f %f",xUniform.at(i)/(fs/2),yUniform.at(i));
        ptsStr +=  QString::number(yUniform.at(i)) + "\n";
    }

    mTextEditPts->setText(ptsStr);

    // Plot uniform set of points
    fftw_complex f[Nfft];
    double t[Nfft]={0};

    f[0][0] = yUniform.at(0);
    f[0][1] = 0;
    for (int i=1;i<Nfft/2;i++)
    {
        f[i][0] = yUniform.at(i);
        f[i][1] = 0;

        f[Nfft-i][0] = yUniform.at(i);
        f[Nfft-i][1] = 0;
    }


    for (int i=0;i<Nfft;i++)
    {
        qDebug("- %f",f[i][0]);

    }

    fftw_plan mfft =  fftw_plan_dft_c2r_1d(Nfft,f,t, FFTW_ESTIMATE);
    Windows *mWin = new Windows(Windows::hamming,Nfft);

    fftw_execute(mfft);
    for (int i=0;i<Nfft;i++)
    {
        qDebug("ifft res %d :%f (%f) : %f",i,t[i]/Nfft,mWin->data.at(i),f[i][0]);
        t[i] = (t[i]/Nfft);
    }



    QVector<double> ri;

    for (int i =floor(Nfft/2)+1;i<Nfft;i++)
        ri.append(t[i]);

    for (int i=0;i<floor(Nfft/2)+1;i++)
        ri.append(t[i]);


    double gain=0;
    for (int i=0;i<Nfft;i++)
    {
        ri.replace(i,ri.at(i) *  mWin->data.at(i));
        qDebug("RI %d: %f",i,ri.at(i));
        gain += ri.at(i);
    }


    QString coefStr;
    for (int i=0;i<Nfft;i++)
    {
        ri.replace(i,ri.at(i));// *  1/gain);
        coefStr += QString::number(ri.at(i)) + "\n";
    }
    mTextEditCoef->setText(coefStr);




    // Zero padding RI
    int n=ri.length();
    for (int i =0; i<(1024-n); i++)
        ri.append(0);

    fftw_complex fRi[1024];
    double in[1024];

    for (int i=0;i<1024;i++)
        in[i]=ri.at(i);

    fftw_plan mfft2 =  fftw_plan_dft_r2c_1d(1024,in,fRi,FFTW_ESTIMATE);
    fftw_execute(mfft2);

    QVector<double> riFreq;
    QVector<double> riFreq2;
    for (int i=0;i<1024/2;i++)
    {
        riFreq.append(qSqrt(fRi[i][0]*fRi[i][0] + fRi[i][1]*fRi[i][1]));
        //riFreq.append(fRi[i][0]);

        riFreq2.append(1.0/(1024.0-1.0) * i * fs);
        //       qDebug("%d : %f %f",i,riFreq2.at(i),riFreq.at(i));
    }


    // Update Plot
    customPlot->graph(3)->setData(xUniform,yUniform);
    customPlot->graph(2)->setData(riFreq2, riFreq);
    customPlot->replot();
}



void MainWindow::reInterp()
{
    // clear y vector
    y.clear();

    //make interpolation
    AkimaInterp *mInterp = new AkimaInterp;
    mInterp->setData(x1,y1);
    for (int i=0;i<x.length();i++)
        y.append(mInterp->interpol_Akima(x.at(i)));

    // Update Plot
    customPlot->graph(1)->setData(x,y);

}
MainWindow::~MainWindow()
{

}

/** [PUBILC SLOTS] **/
void MainWindow::onMousePress(QMouseEvent* m)
{
    if(m->button()==Qt::LeftButton)
        connect(customPlot,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(onMouseMove(QMouseEvent*)));

    if(m->button()==Qt::RightButton)
    {
        float xCoord = customPlot->xAxis->pixelToCoord(m->pos().x());
        float yCoord = customPlot->yAxis->pixelToCoord(m->pos().y());
        qint64  it = customPlot->graph(0)->data().data()->findBegin(xCoord,false) - customPlot->graph(0)->data().data()->constBegin();

        if (m->modifiers() == Qt::ControlModifier) // Remove Point
        {
            // find closest point
            // qint64  it = customPlot->graph(0)->data().data()->findBegin(xCoord,false) - customPlot->graph(0)->data().data()->constBegin(); // Other Solution but do not trake the nearest point
            float tmp, tmpSave;
            tmpSave =x1.at(x1.length()-1);
            for (int i=0;i<x1.length();i++)
            {
                tmp = qAbs(x1.at(i)-xCoord);
                if (tmp<tmpSave)
                {
                    it =i;
                    tmpSave=tmp;
                }
            }
            x1.remove(it);
            y1.remove(it);
        }
        else // add point
        {
            /*
            if(yCoord>1)
                yCoord=1;*/
            if(yCoord<0)
                yCoord=0;
            x1.insert(it,xCoord);
            y1.insert(it,yCoord);
        }

        // Update plot
        customPlot->graph(0)->setData(x1,y1);
        reInterp();
        customPlot->replot();
    }
}

void MainWindow::onMouseRelease(QMouseEvent* m)
{
    // Disconnect mouse Move slot
    disconnect(customPlot,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(onMouseMove(QMouseEvent*)));

    // Update Filter
    generateFilter();
}


void MainWindow::onMouseMove(QMouseEvent* m)
{

    //get Mouse pointer coordinates
    float xCoord = customPlot->xAxis->pixelToCoord(m->pos().x());
    float yCoord = customPlot->yAxis->pixelToCoord(m->pos().y());

    // Update Traver
    phaseTracer->setGraph(customPlot->graph(0));
    phaseTracer->setGraphKey(this->customPlot->xAxis->pixelToCoord(m->pos().x()));
    customPlot->replot(); // TO update phaseTracer Value

    // get coordinate of the selected point
    QPointF ptsCoord = this->phaseTracer->position->coords();

    // Find closest point
    qint64  it = customPlot->graph(0)->data().data()->findBegin(ptsCoord.x(),false) - customPlot->graph(0)->data().data()->constBegin(); // Other Solution which take the point
    // Other solution to fin the closest point
    /*float tmp, tmpSave;
    tmpSave =x1.at(x1.length()-1);
    for (int i=0;i<x1.length();i++)
    {
        tmp = qAbs(x1.at(i)-ptsCoord.x());
        if (tmp<tmpSave)
        {
            it =i;
            tmpSave=tmp;
        }
    }*/

    // Limit excursion
    /*  if (yCoord > 1)
        yCoord=1;*/
    if (yCoord <0)
        yCoord =0;

    //Move point
    y1.replace(it,yCoord);

    // Update plot
    customPlot->graph(0)->setData(x1,y1);
    reInterp();
    customPlot->replot();
}

void MainWindow::onValueChanged(int val)
{
    generateFilter();
    customPlot->replot();
}


void MainWindow::onInterpCurveCheckBox(bool isChecked)
{
    customPlot->graph(1)->setVisible(isChecked);
    customPlot->replot();
}

void MainWindow::onInterpPatternCheckBox(bool isChecked)
{
    customPlot->graph(3)->setVisible(isChecked);
    customPlot->replot();
}

/** [FUNCTIONS] **/
int pow2roundup (int x)
{
    if (x < 0)
        return 0;
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x+1;
}
