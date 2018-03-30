#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QDebug>
#include <search.h>
#include <QImage>
#include <QPixmap>
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
    void ShowImage();
    void ShowSearchStatus();


private slots:
    void updateStatus(void);
    void on_pbStartSearch_clicked();
    void on_pbCancleSearch_clicked();
    void on_pbExit_clicked();
    void on_pbLoadData_clicked();
    void slotUpdateTextStatus(QString);

private:
    Ui::MainWindow *ui;
    QThread* thread = new QThread;
    Search * search = new Search();
    QChartView *chartView;
    QChartView *chartViewConstants;
    QVector <QPoint> referencePoints;
    QVector <QPoint> distortedPoints;
    QPixmap Image;
};

#endif // MAINWINDOW_H
