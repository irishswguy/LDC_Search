#ifndef SEARCH_H
#define SEARCH_H

#include <QObject>
#include <QDebug>
#include <QVector>
#include <QThread>
#include <QPoint>
#include <QtMath>
#include <distortion.h>
#include <QDateTime>


#define PROBLEM_DIM 3


struct SEARCH_STATUS
{
    QVector <double> error;
    QVector <double> K[3];
    QVector <double> Angle;
    QPoint  Centre;
    bool Finished=false;
    QString qsStatus;
};

class Search : public QObject
{
    Q_OBJECT
public:
    explicit Search(QObject *parent = nullptr);
    ~Search();
    void Start();

    bool Cancel = false;
    struct SEARCH_STATUS SearchStatus;
    DISTORTION_VARS DV;
    DISTORTION_VARS DV2;


    //void S(void);
    void S(double bounds[PROBLEM_DIM],int maxEvaluations);
    void BruteForceSearch(void);
    void toro(double (&x)[PROBLEM_DIM], double bounds[]);
    int fix(double x);
    double drand(double lower,double upper);



public slots:
    void process();
signals:
    void finished();
    void updateStatus(void);
    void signalUpdateTextStatus(QString);
private:
    
    Distortion LDC;

public slots:
};

#endif // SEARCH_H
