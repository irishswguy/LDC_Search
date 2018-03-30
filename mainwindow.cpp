#include "mainwindow.h"
#include "ui_mainwindow.h"


using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    search->moveToThread(thread);
    connect(search, SIGNAL (updateStatus()), this, SLOT (updateStatus()));
    connect(search, SIGNAL (signalUpdateTextStatus(QString)), this, SLOT (slotUpdateTextStatus(QString)));

    connect(thread, SIGNAL (started()), search, SLOT (process()));
    connect(search, SIGNAL (finished()), thread, SLOT (quit()));
    connect(search, SIGNAL (finished()),  this, SLOT (updateStatus()));


    chartView = new QChartView(this);
    QGridLayout* gridLayout;
    gridLayout = new QGridLayout(ui->myWidget);
    gridLayout->addWidget(chartView,0,0);

    chartViewConstants= new QChartView(this);
    QGridLayout* gridLayoutConstants;
    gridLayoutConstants = new QGridLayout(ui->constPlot);
    gridLayoutConstants->addWidget(chartViewConstants,0,0);

    chartView->setFrameShape(QFrame::Box);
    chartViewConstants->setFrameShape(QFrame::Box);
    PlotConstants();
    PlotError();

    ui->pbLoadData->setEnabled(true);
    ui->pbStartSearch->setEnabled(false);
    ui->pbCancleSearch->setEnabled(false);
    ui->pbExit->setEnabled(true);

    Image= QPixmap(1920,1080);

}

MainWindow::~MainWindow()
{
    delete ui;
}

#define STEP 25
void MainWindow::PlotError()
{

    int NumberOfSamples=search->SearchStatus.error.size();

    QLineSeries *series = new QLineSeries();
    for(int i=0;i<NumberOfSamples;i++){
            qDebug() << search->SearchStatus.error.at(i);
            *series << QPointF(i,search->SearchStatus.error.at(i));

            }

    QChart *chart = new QChart();
    chart->addSeries(series);

    chart->legend()->hide();
    chart->setTitle("Search Error");

    // Adjusting the Axes of the Graph
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Samples");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(1);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    if(NumberOfSamples<STEP)
        axisX->setMax(STEP);
    else
        axisX->setMax(NumberOfSamples+(STEP/4));
    axisX->setMin(0.0);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Error Value");
    axisY->setLabelFormat("%g");
    axisY->setTickCount(5);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    axisY->setMax(10.0);
    axisY->setMin(0.0);

    // We set the graph in the view
    chartView->setChart(chart);
}

void MainWindow::PlotConstants()
{

    int NumberOfSamples=search->SearchStatus.error.size();

    QLineSeries *K0series = new QLineSeries();
    QLineSeries *K1series = new QLineSeries();
    QLineSeries *K2series = new QLineSeries();
    QLineSeries *Angleseries = new QLineSeries();

    K0series->setName(QString("K0"));
    K1series->setName(QString("K1"));
    K2series->setName(QString("K2"));
    Angleseries->setName(QString("Angle"));

    for(int i=0;i<NumberOfSamples;i++){
        *K0series << QPointF(i,search->SearchStatus.K[0].at(i));
        *K1series << QPointF(i,search->SearchStatus.K[1].at(i));
        *K2series << QPointF(i,search->SearchStatus.K[2].at(i));
        *Angleseries << QPointF(i,search->SearchStatus.Angle.at(i));
        }

    QChart *chart = new QChart();
    chart->addSeries(K0series);
    chart->addSeries(K1series);
    chart->addSeries(K2series);
    chart->addSeries(Angleseries);


    chart->legend()->hide();
    chart->setTitle("Constants Chart");

    // Adjusting the Axes of the Graph
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Samples");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(1);
    chart->addAxis(axisX, Qt::AlignBottom);
    K0series->attachAxis(axisX);
    K1series->attachAxis(axisX);
    K2series->attachAxis(axisX);
    Angleseries->attachAxis(axisX);
    if(NumberOfSamples<STEP)
        axisX->setMax(STEP);
    else
        axisX->setMax(NumberOfSamples+(STEP/4));
    axisX->setMin(0.0);


    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Constants");
    axisY->setLabelFormat("%g");
    axisY->setTickCount(5);
    chart->addAxis(axisY, Qt::AlignLeft);
    K0series->attachAxis(axisY);
    axisY->setMax(2.0);
    axisY->setMin(-2.0);


    K1series->attachAxis(axisY);
    K2series->attachAxis(axisY);
    Angleseries->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartViewConstants->setChart(chart);

}
//-------------------------------------------------------------------------------------------------------------
//
//  This function displates images of the image points and saves these images to files.
//
//-------------------------------------------------------------------------------------------------------------
void MainWindow::ShowImage()
{
    Image.fill();

    QVector <QPoint> Points;

    QPainter *paint = new QPainter(&Image);
    QPainterPath path_reference;

    paint->setRenderHint(QPainter::Antialiasing);
    paint->setRenderHint(QPainter::HighQualityAntialiasing);

    Points = distortedPoints;
    if(Points.size()==0)
        return;

    paint->setPen(Qt::green);
    paint->setBrush(Qt::green);
    paint->drawEllipse(search->SearchStatus.Centre,10,10);

    paint->setPen(Qt::blue);
     paint->setBrush(Qt::blue);
    paint->fillPath(path_reference, Qt::blue);

    for(int i=0;i<Points.size();i++)
        {
        QPointF fP(Points.at(i).x(),Points.at(i).y());
        path_reference.addEllipse(fP,7,7);
        }

    paint->fillPath(path_reference, Qt::blue);
    paint->drawPath(path_reference);

    for(int y=0;y<19;y++)
        for(int x=0;x<18;x++)
        {
            QPoint P1 = Points.at(y+(19*x));
            QPoint P2 = Points.at(y+(19*x)+19);
            paint->drawLine(P1,P2);
        }

    for(int x=0;x<19;x++)
        for(int y=0;y<18;y++)
        {
            int Index1 = y+(19*x);
            int Index2 = y+(19*x)+1;
            QPoint P1 = Points.at(Index1);
            QPoint P2 = Points.at(Index2);
            paint->drawLine(P1,P2);
        }

    Points = referencePoints;
    path_reference = QPainterPath();
    paint->setPen(Qt::red);
    paint->setBrush(Qt::red);
    paint->fillPath(path_reference, Qt::red);
    for(int i=0;i<Points.size();i++)
        {
        QPointF fP(Points.at(i).x(),Points.at(i).y());
        path_reference.addEllipse(fP,7,7);
        }

    paint->fillPath(path_reference, Qt::red);
    paint->drawPath(path_reference);

    for(int y=0;y<19;y++)
        for(int x=0;x<18;x++)
        {
            QPoint P1 = Points.at(y+(19*x));
            QPoint P2 = Points.at(y+(19*x)+19);
            paint->drawLine(P1,P2);
        }

    for(int x=0;x<19;x++)
        for(int y=0;y<18;y++)
        {
            int Index1 = y+(19*x);
            int Index2 = y+(19*x)+1;
            QPoint P1 = Points.at(Index1);
            QPoint P2 = Points.at(Index2);
            paint->drawLine(P1,P2);
        }



    delete paint;
    ui->lbImage->setPixmap(Image);


}

void MainWindow::ShowSearchStatus()
{
    if(search->SearchStatus.qsStatus!=""){
        ui->lwSearchStatus->addItem(search->SearchStatus.qsStatus);
        ui->lwSearchStatus->scrollToBottom();
        }
}



void MainWindow::updateStatus()
{
    qDebug() << "Status";
    PlotError();
    PlotConstants();
    ShowImage();
    ShowSearchStatus();

    if(search->SearchStatus.Finished==true)
    {
        ui->pbStartSearch->setEnabled(true);
        ui->pbCancleSearch->setEnabled(false);
        ui->pbExit->setEnabled(true);
    }
}

void MainWindow::on_pbStartSearch_clicked()
{
     search->Start();
     thread->start();
     PlotError();
     PlotConstants();

     ui->pbLoadData->setEnabled(false);
     ui->pbStartSearch->setEnabled(false);
     ui->pbCancleSearch->setEnabled(true);
     ui->pbExit->setEnabled(false);

}

void MainWindow::on_pbCancleSearch_clicked()
{
    search->Cancel=true;
    ui->pbLoadData->setEnabled(true);
    ui->pbStartSearch->setEnabled(true);
    ui->pbCancleSearch->setEnabled(false);
    ui->pbExit->setEnabled(true);


}

void MainWindow::on_pbExit_clicked()
{
    QApplication::quit();
}

void MainWindow::on_pbLoadData_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
             tr("Save Points File"), "/home/louis/Projects/temp", tr("TXT Files (*.*)"));

    fileName.chop(4);
    if(fileName!=NULL)
    {
    QFile fileRef(fileName+".ref");
    if(fileRef.open(QIODevice::ReadOnly))
      {
          QTextStream in(&fileRef);
             while (!in.atEnd())
             {
                QString line = in.readLine();
                qDebug() << line;
                QStringList tmpList = line.split(QRegExp(","));
                QString sX = tmpList[0];
                QString sY = tmpList[1];
                QPoint P(sX.toInt(),sY.toInt());
                referencePoints.push_back(P);
             }
          fileRef.close();
      }
    else
        qDebug() << "Unable to read file:" << fileName;

    QFile fileDis(fileName+".dis");
        if(fileDis.open(QIODevice::ReadOnly))
        {
            QTextStream in(&fileDis);
               while (!in.atEnd())
               {
                  QString line = in.readLine();
                  qDebug() << line;
                  QStringList tmpList = line.split(QRegExp(","));
                  QString sX = tmpList[0];
                  QString sY = tmpList[1];
                  QPoint P(sX.toInt(),sY.toInt());
                  distortedPoints.push_back(P);
               }
            fileDis.close();
        }
      else
          qDebug() << "Unable to read file:" << fileName;
        ui->pbLoadData->setEnabled(true);
        ui->pbStartSearch->setEnabled(true);
        ui->pbCancleSearch->setEnabled(false);
        ui->pbExit->setEnabled(true);
    }

}

void MainWindow::slotUpdateTextStatus(QString message)
{
    ui->lwSearchStatus->addItem(message);
    ui->lwSearchStatus->scrollToBottom();
}
