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

struct PARTICLE
{
    double K[3];
    double Angle;
    QPoint Center;
    double BestError;
};

struct PARTICLE_SEARCH
{
    QVector <PARTICLE> particle;
    double GlobalBestError;
    double GlobalBestK[3];
    double GlobalBestAngle;
    QPoint GlobalBestCenter;
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

    void S(double bounds[PROBLEM_DIM],int maxEvaluations);
    //---------------------------------------------------------------------
    void LRSearch(void);
    void LRSearchInit(void);
    void LRSearchCopyParticle(int p);




    //---------------------------------------------------------------------
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
    PARTICLE_SEARCH PS;

public slots:
};

#endif // SEARCH_H
