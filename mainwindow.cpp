#include "mainwindow.h"
#include "ui_mainwindow.h"


using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    search->moveToThread(thread);
    connect(search, SIGNAL (error(QString)), this, SLOT (errorString(QString)));
    connect(search, SIGNAL (updateStatus()), this, SLOT (updateStatus()));

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
    axisY->setMax(2.0);
    axisY->setMin(0.0);


    // We set the graph in the view
    chartView->setChart(chart);

}

void MainWindow::PlotConstants()
{
    int seed=1;

    QLineSeries *series = new QLineSeries();
    int k = 0;
    while (k <= 200)
    {
        *series << QPointF(k,sin((seed+ k)*0.1));
        ++k;
    }

    QLineSeries *series2 = new QLineSeries();
    k = 0;
    while (k <= 100)
    {
        *series2 << QPointF(k,sin((seed+ k)*0.1)+.21);
        ++k;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->addSeries(series2);

    chart->legend()->hide();
    chart->setTitle("Search Error");

    // Adjusting the Axes of the Graph
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Samples");
    axisX->setLabelFormat("%i");
    axisX->setTickCount(1);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Error Value");
    axisY->setLabelFormat("%g");
    axisY->setTickCount(5);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    axisY->setMax(2.0);
    axisY->setMin(-2.0);


    QValueAxis *axisY2 = new QValueAxis();
    axisY2->setTitleText("Error Value");
    axisY2->setLabelFormat("%g");
    axisY2->setTickCount(5);
    chart->addAxis(axisY, Qt::AlignRight);
    series2->attachAxis(axisY);

    // We set the graph in the view
    chartViewConstants->setChart(chart);

}

void MainWindow::errorString(QString err)
{
    qDebug() << "Error String:"<< err;
}

void MainWindow::updateStatus()
{
    qDebug() << "Status";
    PlotError();

    if(search->SearchStatus.Finished==true)
    {
        ui->pbStartSearch->setEnabled(true);
        ui->pbCancleSearch->setEnabled(false);
    }
}

void MainWindow::on_pbStartSearch_clicked()
{
     search->Start();
     thread->start();
     PlotError();
     PlotConstants();
     ui->pbStartSearch->setEnabled(false);
     ui->pbCancleSearch->setEnabled(true);
}

void MainWindow::on_pbCancleSearch_clicked()
{
    search->Cancel=true;
    ui->pbStartSearch->setEnabled(true);
    ui->pbCancleSearch->setEnabled(false);

}
