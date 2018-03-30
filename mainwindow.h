#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QDebug>
#include <search.h>
//#include <QChartView>
#include <QtCharts>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void PlotError(void);
    void PlotConstants(void);

private slots:
    void errorString(QString);
    void updateStatus(void);
    void on_pbStartSearch_clicked();
    void on_pbCancleSearch_clicked();

private:
    Ui::MainWindow *ui;
    QThread* thread = new QThread;
    Search * search = new Search();
    QChartView *chartView;
    QChartView *chartViewConstants;

};

#endif // MAINWINDOW_H
