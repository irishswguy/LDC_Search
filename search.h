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
#include <algorithm>
#include <statistics.h>


#define PROBLEM_DIM 6


struct SEARCH_STATUS
{
    QVector <double> error;
    QVector <double> K[3];
    QVector <double> Angle;
    QPoint  Centre;
    bool Finished=false;
    QString qsSearchStatus;
    QVector <QString> ResultsStatus;
    int SearchProgress=0;
    int CurrentTestCycle=0;
};

struct PARTICLE
{
    double K[3];
    double Angle;
    QPoint Center;
    double BestError;
    bool operator<(const PARTICLE &other) const {
      return (BestError < other.BestError);
    }
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

    bool S_Search=true;
    bool LR_Search=false;
    bool GP_Search=false;
    int TestCycles=1;

     QVector <double> StatisticalData;

    void S(double bounds[PROBLEM_DIM],int maxEvaluations);
    void S_New(QVector <double>,int maxEvaluations);

    //---------------------------------------------------------------------
    void LRSearch(void);
    void LRSearchInit(void);
    void LRSearchCopyParticle(int p);
    void LRSearchMutate(QVector <PARTICLE> SelectedParticles);





    //---------------------------------------------------------------------
    void BruteForceSearch(void);
    void toro(double (&x)[PROBLEM_DIM-3], double bounds[]);
    void toro(double (&variables)[PROBLEM_DIM]  , QVector<double> &bounds);
 //   void toro(double (&x)[PROBLEM_DIM-3], QVector<double> &bounds);
    void toro(QVector<double> &bounds);


    int fix(double x);
    double drand(double lower,double upper);
    bool randChoice(void);

public slots:
    void process();
signals:
    void finished();
    void updateStatus(void);
//    void signalUpdateSearchStatus(QString);
//    void signalUpdateResultsStatus(QString);
//    void signalUpdateProgressStatus(QString);


private:
    
    Distortion LDC;
    PARTICLE_SEARCH PS;
    Statistics statistics;

public slots:
};

#endif // SEARCH_H
