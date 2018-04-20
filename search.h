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
struct PSO_PARTICLE
{
    double K[3];
    double Angle;
    QPoint Center;
    double BestError;
    double theta2;
    double theta3;
    double V;
    bool operator<(const PARTICLE &other) const {
      return (BestError < other.BestError);
    }
};

struct EV_PARTICLE
{
    double K[3];
    double Angle;
    QPoint Center;
    double movement[6];
    double Error;
    bool operator<(const EV_PARTICLE &other) const {
      return (Error < other.Error);
    }
};

struct PSO_GLOBAL
{
    QVector <PSO_PARTICLE> P;
    double theta1;
    double BestError;
};

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
    QVector <PARTICLE> BestParticle;

    double BestError;
//    double BestK[3];
//    double BestAngle;
//    QPoint BestCentre;

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

    void UpdateResults(QString Data);

     QVector <double> StatisticalData;

    void S(double bounds[PROBLEM_DIM],int maxEvaluations);
    void S_New(QVector <double>,int maxEvaluations);

    //------------------ Evolution Search ------------------------------
    QVector <EV_PARTICLE> EV_Swarm;
    void EV_Search(int maxEvaluations);
    void EV_Init();
    double EV_Function(EV_PARTICLE);
    void EV_Evolve(void);
    void EV_SortAndSelect(void);


    // --------------------------------------------------------------------
    struct PSO_GLOBAL pso;
    void PSO(int maxEvaluations);
    void PSOInit(void);
    double PSOFunction(int Particle);
    double PSOFunction(PSO_PARTICLE Particle);
    PSO_PARTICLE PSOPerturb(PSO_PARTICLE );




    //---------------------------------------------------------------------
    void LRSearch(void);
    void LRSearchInit(void);
    void LRSearchCopyParticle(int p);
    void LRSearchMutate(QVector <PARTICLE> SelectedParticles);
    void LRSearchAddParticles(int n);





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
