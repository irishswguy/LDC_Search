#include "search.h"

Search::Search(QObject *parent) : QObject(parent)
{

}

Search::~Search() {
}

void Search::Start()
{
    SearchStatus.Finished=false;
    SearchStatus.K[0].clear();
    SearchStatus.K[1].clear();
    SearchStatus.K[2].clear();
    SearchStatus.error.clear();
    SearchStatus.Angle.clear();
    SearchStatus.qsSearchStatus = "";
    SearchStatus.SearchProgress=0;



    Cancel = false;
}

void Search::UpdateResults(QString Data)
{
    SearchStatus.ResultsStatus.push_back(Data);
    emit updateStatus();
    QThread::msleep(500);
     SearchStatus.ResultsStatus.clear();
}

void Search::process() {

    qDebug() << "Search Process Started";

    SearchStatus.ResultsStatus.clear();
    SearchStatus.BestParticle.clear();


    if( LR_Search == true){
        UpdateResults("Starting LR-Search Algrothim \n------------------------------------------\n");

            StatisticalData.clear();
            SearchStatus.BestError=std::numeric_limits<double>::max();

            for(int i=0;i<TestCycles;i++){
                Start();
                SearchStatus.BestParticle.clear();
                SearchStatus.CurrentTestCycle=i;
                LRSearch();
                UpdateResults("Test Cycle : "+QString::number(i+1)+" Error : " + QString::number(StatisticalData.back(),'f',2));
            }

    }

    if(S_Search==true){
        double bounds[]={-0.2,0.2};
        QVector <double> Bounds;
        Bounds.push_back(-0.2);     // K[0]
        Bounds.push_back(0.2);
        Bounds.push_back(-0.2);     // K[1]
        Bounds.push_back(0.2);
        Bounds.push_back(-0.2);     // K[2]
        Bounds.push_back(0.2);
        Bounds.push_back(-20);     // X-Offset
        Bounds.push_back(+20);
        Bounds.push_back(-20);     // Y-Offset
        Bounds.push_back(+20);
        Bounds.push_back(-10);      // Angle
        Bounds.push_back(10);

        UpdateResults("Starting S-Search Algrothim \n------------------------------------------\n");
        StatisticalData.clear();

        for(int i=0;i<TestCycles;i++){
            Start();
            SearchStatus.BestParticle.clear();
            SearchStatus.CurrentTestCycle=i;;
            S_New(Bounds,500);
            UpdateResults("Test Cycle : "+QString::number(i+1)+" Error : " + QString::number(StatisticalData.back(),'f',2));
//            SearchStatus.ResultsStatus.push_back("Test Cycle : "+QString::number(i+1)+" Error : " + QString::number(StatisticalData.back(),'f',2));
//            emit updateStatus();
//            QThread::msleep(500);
//            SearchStatus.ResultsStatus.clear();

            }

        }

    if(GP_Search==true)
        {
        PSO(500);
        }

    QVector <PARTICLE> tempParticle;

    tempParticle=SearchStatus.BestParticle;

    qSort(tempParticle);
    double Mean = statistics.getMean(StatisticalData);
    double StdDev = statistics.getStdDeviation(StatisticalData,Mean);

    QString Result = "\n---------Best Results-------" +
    QString("\n  Best Error :")+
    QString::number(tempParticle.at(0).BestError)+
    QString("\n  K1 :")+QString::number(tempParticle.at(0).K[0])+
    QString("\n  K1 :")+QString::number(tempParticle.at(0).K[1])+
    QString("\n  K2 :")+QString::number(tempParticle.at(0).K[2])+
    QString("\n  Angle :")+QString::number(tempParticle.at(0).Angle,'f',2)+
    QString("\n  Centre : ") +QString::number(tempParticle.at(0).Center.x())+","+QString::number(tempParticle.at(0).Center.y())+
    QString("\n ")+
    QString("\nMean   : ") + QString::number(Mean,'f',1)+
    QString("\nStdDev : ") + QString::number(StdDev,'f',1)+
    QString("\n ");
    SearchStatus.Finished=true;
    UpdateResults(Result);


//    SearchStatus.ResultsStatus.push_back("---------Best Results-------");
//    SearchStatus.ResultsStatus.push_back("  Best Error :"+QString::number(tempParticle.at(0).BestError));
//    SearchStatus.ResultsStatus.push_back("  K1 :"+QString::number(tempParticle.at(0).K[0]));
//    SearchStatus.ResultsStatus.push_back("  K1 :"+QString::number(tempParticle.at(0).K[1]));
//    SearchStatus.ResultsStatus.push_back("  K2 :"+QString::number(tempParticle.at(0).K[2]));
//    SearchStatus.ResultsStatus.push_back("  Angle :"+QString::number(tempParticle.at(0).Angle,'f',2));

//    QString CentreString = "  Centre : "
//            +QString::number(tempParticle.at(0).Center.x())+
//                             ","+QString::number(tempParticle.at(0).Center.y()) ;
//    SearchStatus.ResultsStatus.push_back(CentreString);

//    double Mean = statistics.getMean(StatisticalData);
//    double StdDev = statistics.getStdDeviation(StatisticalData,Mean);
//    SearchStatus.ResultsStatus.push_back(" ");
//    SearchStatus.ResultsStatus.push_back("Mean   : " + QString::number(Mean,'f',1));
//    SearchStatus.ResultsStatus.push_back("StdDev : " + QString::number(StdDev,'f',1));
//    SearchStatus.ResultsStatus.push_back(" ");

//    SearchStatus.Finished=true;
//    emit updateStatus();

    emit finished();


    return;

////CANCEL:
//    qDebug() << "Search Process Canceled";
//    SearchStatus.Finished=true;
//    emit finished();

}

//-------------------------------------------------------------------------------------------------------------
//
//  This function performs a search for the best constants K1 and K2 by
//  testing every combination of k1 with every combination of K2.
//
//  The Reference pixel positions are held in the vector 'reference'.
//
//  The Distorted pixel positions are keld in the vector 'distorted'.
//
//  The results are displayed on the console only for reference.
//
//-------------------------------------------------------------------------------------------------------------
void Search::BruteForceSearch(void)
{
//    double BestError=std::numeric_limits<double>::max();
//    double BestK1;
//    double BestK2;

    //ElapsedTime.start();

//    for(double k1=-0.2;k1<0.2;k1=k1+0.0001)
//        {
//        for(double k2=-0.2;k2<0.2;k2=k2+0.0001)
//                {
//                K.clear();
//                K.push_back(k1);
//                K.push_back(k2);

//                double error = LDC.getLDCError(reference,distorted,undistorted,K);

//                if(error<BestError){
//                    BestError=error;
//                    BestK1=k1;
//                    BestK2=k2;
//                    }
//                }
//        }
//    qDebug() << "Brute Force Search - K1:" << BestK1 << " K2:"<<BestK2 << " Lowest Error:" << BestError;
//    //qDebug() << "Brute Force Search - Search Time:" << ElapsedTime.elapsed()/1000 << "Seconds";
}

void Search::S_New(QVector<double> Bounds,int maxEvaluations)
{
double  best[PROBLEM_DIM];
double  fBest = std::numeric_limits<double>::max();
int     j = 0;
double  StepSize[PROBLEM_DIM];


        QDateTime cd = QDateTime::currentDateTime();
        qsrand(cd.toTime_t());

        // Initalise the Startup-Conditions
        for(int i=0;i<PROBLEM_DIM;i++){
            best[i] = drand(Bounds.at(i*2),Bounds.at((i*2)+1));
            StepSize[i]= 0.04 * (Bounds.at(i*2)-Bounds.at((i*2)+1));   // Step is 40% of bounds
            }

        best[3]=959;
        best[4]=539;

        DV.K[0] = drand(-0.2,0.2);
        DV.K[1] = drand(-0.2,0.2);
        DV.K[2] = drand(-0.2,0.2);
        DV.Center = QPoint(959,539);
        DV.Angle = 0;

        toro(Bounds); // Fix out of bounds conditions

        while (j < maxEvaluations)
            {
            if(Cancel==true)
                return;
            bool Improved=false;
            for(int i = 0; i < PROBLEM_DIM && j < maxEvaluations; ++i)                              // Core iteration step
                {
                    fBest = LDC.getLDCError(DV);       // Get current best error

                    switch(i)
                    {
                    case 0 :
                    case 1 :
                    case 2 : DV.K[i] = best[i]-StepSize[i];         break;
                    case 3 : DV.Center.setX(best[i]-StepSize[i]);   break;
                    case 4 : DV.Center.setY(best[i]-StepSize[i]);   break;
                    case 5 : DV.Angle = best[i]-StepSize[i];     break;
                    }
                    toro(Bounds);

                    double fitness = LDC.getLDCError(DV);    // Get new fittness

                    if(fitness < fBest)                                                             // If improved then save
                    {
                        switch(i)
                        {
                        case 0 :
                        case 1 :
                        case 2 : best[i] = DV.K[i];  break;                                                   // Adjust K
                        case 3 : best[i] = DV.Center.x();   break;
                        case 4 : best[i] = DV.Center.y();   break;
                        case 5 : best[i] = DV.Angle;  break;                                                   // Adjust K
                        }


                        Improved = true;
                        fBest = fitness;
                    }
                    else                                                                            // Else test 1/2 step opposite direction
                    {
                        switch(i)
                        {
                        case 0 :
                        case 1 :
                        case 2 : DV.K[i] = best[i]+(StepSize[i]/1.1);           break;                                                   // Adjust K
                        case 3 : DV.Center.setX(best[i]+(StepSize[i]/2));     break;
                        case 4 : DV.Center.setY(best[i]+(StepSize[i]/2));     break;
                        case 5 : DV.Angle = best[i]+(StepSize[i]/2);       break;                                                   // Adjust K
                        }

                        toro(Bounds);
                        fitness = LDC.getLDCError(DV);

                        if(fitness < fBest)                                                         // If improved save
                        {
                            switch(i)
                            {
                            case 0 :
                            case 1 :
                            case 2 : best[i] = DV.K[i]; break;                                                   // Adjust K
                            case 3 : best[i] = DV.Center.x();   break;
                            case 4 : best[i] = DV.Center.y();   break;
                            case 5 : best[i] = DV.Angle; break;                                                   // Adjust K
                            }

                            Improved = true;
                            fBest = fitness;
                        }
                        else
                            switch(i)
                            {
                            case 0 :
                            case 1 :
                            case 2 : DV.K[i]=best[i];  break;                                                   // Adjust K
                            case 3 : DV.Center.setX(best[i]);   break;
                            case 4 : DV.Center.setY(best[i]);   break;
                            case 5 : DV.Angle=best[i];  break;                                                   // Adjust K
                            }
                                                                                      // If no improvment in this step
                    }                                                                               // go back to last value
                j++;
                }
                // If no improvment in any dimension
                // reduce all step sizes.
                if(Improved==false)
                    for(int i=0;i<PROBLEM_DIM;i++)
                        switch(i)
                        {
                        case 0 :
                        case 1 :
                        case 2 : StepSize[i]= StepSize[i]/1.1; break;
                        case 3 : StepSize[i]= StepSize[i]/2; break;
                        case 4 : StepSize[i]= StepSize[i]/2; break;
                        case 5 : StepSize[i]= StepSize[i]/2; break;
                        }


            SearchStatus.BestError = fBest; //PS.particle[0].BestError;
            PARTICLE PV;
            PV.Angle = DV.Angle;
            PV.BestError=fBest;
            PV.Center = DV.Center;
            PV.K[0] = DV.K[0];
            PV.K[1] = DV.K[1];
            PV.K[2] = DV.K[2];
            SearchStatus.BestParticle.push_back(PV);

            SearchStatus.qsSearchStatus = "S-Search, Cycle :" + QString::number(SearchStatus.CurrentTestCycle) +" Loop :" + QString::number(j);

            SearchStatus.SearchProgress = j*TestCycles;

            emit updateStatus();
            QThread::msleep(500);
        }
        StatisticalData.push_back(SearchStatus.BestError);
}


//-------------------------------------------------------------------------------------------------------------
//
//  This function performs torroidal saturation of the vector using the bounds array
//
//-------------------------------------------------------------------------------------------------------------
void Search::toro(QVector<double> &bounds)
{
    for(int i=0;i<PROBLEM_DIM;i++)
    {
      if(DV.K[0] < bounds.at(0)) DV.K[0] = bounds.at(0);
      if(DV.K[0] > bounds.at(1)) DV.K[0] = bounds.at(1);
      if(DV.K[1] < bounds.at(2)) DV.K[1] = bounds.at(2);
      if(DV.K[1] > bounds.at(3)) DV.K[1] = bounds.at(3);
      if(DV.K[2] < bounds.at(4)) DV.K[2] = bounds.at(4);
      if(DV.K[2] > bounds.at(5)) DV.K[2] = bounds.at(5);
//      if(x[3] < bounds.at(6)) x[3] = bounds.at(6);
//      if(x[3] > bounds.at(7)) x[3] = bounds.at(7);
//      if(x[4] < bounds.at(8)) x[4] = bounds.at(8);
//      if(x[4] > bounds.at(9)) x[4] = bounds.at(9);
//      if(x[5] < bounds.at(10)) x[5] = bounds.at(10);
//      if(x[5] > bounds.at(11)) x[5] = bounds.at(11);
    }
}


void Search::toro(double (&x)[PROBLEM_DIM-3], double bounds[])
{
    double Xcor[5];
    double xL = bounds[0];
    double xU = bounds[1];

    for(int i=0;i<2;i++)
    {
        Xcor[i] = (x[i]-xL)/(xU-xL);
        if(Xcor[i]>1)
            Xcor[i] = Xcor[i] -fix(Xcor[i]);
        else if(Xcor[i]<0)
        {
            Xcor[i] = 1 - qAbs(Xcor[i] -fix(Xcor[i]));
        }
        Xcor[i] = xL+Xcor[i]*(xU-xL);

        x[i] = Xcor[i];
    }
}

void Search::toro(double (&variables)[PROBLEM_DIM]  , QVector<double> &bounds)
{
    for(int i=0;i<PROBLEM_DIM;i++)
    {
      if(variables[0] < bounds.at(0)) variables[0] = bounds.at(0);
      if(variables[0] > bounds.at(1)) variables[0] = bounds.at(1);
      if(variables[1] < bounds.at(2)) variables[1] = bounds.at(2);
      if(variables[1] > bounds.at(3)) variables[1] = bounds.at(3);
      if(variables[2] < bounds.at(4)) variables[2] = bounds.at(4);
      if(variables[2] > bounds.at(5)) variables[2] = bounds.at(5);
      if(variables[3] < bounds.at(6)) variables[3] = bounds.at(6);
      if(variables[3] > bounds.at(7)) variables[3] = bounds.at(7);
      if(variables[4] < bounds.at(8)) variables[4] = bounds.at(8);
      if(variables[4] > bounds.at(9)) variables[4] = bounds.at(9);
      if(variables[5] < bounds.at(10)) variables[5] = bounds.at(10);
      if(variables[5] > bounds.at(11)) variables[5] = bounds.at(11);
    }
}

 int Search::fix(double x)
    {
        return (int) ((x >= 0) ? floor(x) : ceil(x));
    }
//-------------------------------------------------------------------------------------------------------------
//
//  This function generated a random number within two bounds
//
//-------------------------------------------------------------------------------------------------------------
double Search::drand(double lower,double upper)
{
    double  r = (double)(qrand()% 100);

    r = (r/100);
    r = r * ( upper - lower );
    r = r + lower;
    return r;
}

bool Search::randChoice()
{
    if(drand(0,1)>=0.5) return true;
    return false;
}

#define MAX_PARTICLES 10000
#define ANGLE_MIN -1
#define ANGLE_MAX 1
#define X_CENTER_MIN 934
#define X_CENTER_MAX 984
#define Y_CENTER_MIN 514
#define Y_CENTER_MAX 563

#define K0_MIN -0.2
#define K0_MAX 0.2
#define K1_MIN -0.2
#define K1_MAX 0.2
#define K2_MIN -0.2
#define K2_MAX 0.2
#define QTY_SELECTED_PARICLES 200
#define RANDOMNESS 10
#define K_RANDOMNESS 20


void Search::LRSearch(void)
{
QVector <double> fitness;
QVector <PARTICLE> SelectedParticles;

    // Randomly Initalise All the Particles
    LRSearchInit();

    for(int j=0;j<10;j++)
    {
        // Test the particles
        for(int i=0;i<PS.particle.size();i++)
        {
        LRSearchCopyParticle(i);
        double Error = LDC.getLDCError(DV);
        PS.particle[i].BestError=Error;
        fitness.push_back(Error);
        }

        // Sort the Particles - lowest errors first
        qSort(PS.particle);

        if(PS.particle[0].BestError < SearchStatus.BestError)
        {
            SearchStatus.BestError = PS.particle[0].BestError;
        }
        SearchStatus.BestParticle.push_back(PS.particle[0]);

        SearchStatus.qsSearchStatus = "LRSearch Number: " + QString::number(j) + "    Error: " + QString::number(SearchStatus.BestError,'f',1);

        LRSearchCopyParticle(0);
        LDC.getLDCError(DV);

        emit updateStatus();
        QThread::msleep(500);

        SelectedParticles.clear();
        // Select Particles for mutation
        for(int i=0;i<QTY_SELECTED_PARICLES;i++)
            SelectedParticles.push_back(PS.particle.at(i));

        PS.particle.clear();

        // Perform Mutation of selected particles
        LRSearchMutate(SelectedParticles);
        LRSearchAddParticles(250);
    }
    StatisticalData.push_back(SearchStatus.BestError);

}

void Search::LRSearchInit()
{
    QDateTime cd = QDateTime::currentDateTime();
    qsrand(cd.toTime_t());

    PS.particle.clear();
    SearchStatus.BestError=std::numeric_limits<double>::max();

    // Initalise the particles
    for(int i=0;i<MAX_PARTICLES;i++)
    {
        PARTICLE particle;
        particle.BestError = std::numeric_limits<double>::max();
        particle.Angle = drand(ANGLE_MIN,ANGLE_MAX);

        qDebug() << "Particle Angle : " << particle.Angle;

        int X = (int)drand(X_CENTER_MIN,X_CENTER_MAX);
        int Y = (int)drand(Y_CENTER_MIN,Y_CENTER_MAX);

        particle.Center.setX(X);
        particle.Center.setY(Y);

//        particle.Center.setX(959);
//        particle.Center.setY(539);


        particle.K[0] = drand(K0_MIN,K0_MAX);
        particle.K[1] = drand(K1_MIN,K1_MAX);
        particle.K[2] = drand(K2_MIN,K2_MAX);

        PS.particle.push_back(particle);
    }
}

void Search::LRSearchAddParticles(int n)
{
    QDateTime cd = QDateTime::currentDateTime();
    qsrand(cd.toTime_t());


    // Initalise the particles
    for(int i=0;i<n;i++)
    {
        PARTICLE particle;
        particle.BestError = std::numeric_limits<double>::max();
        particle.Angle = drand(ANGLE_MIN,ANGLE_MAX);

        int X = (int)drand(X_CENTER_MIN,X_CENTER_MAX);
        int Y = (int)drand(Y_CENTER_MIN,Y_CENTER_MAX);

        particle.Center.setX(X);
        particle.Center.setY(Y);

//        particle.Center.setX(959);
//        particle.Center.setY(539);


        particle.K[0] = drand(K0_MIN,K0_MAX);
        particle.K[1] = drand(K1_MIN,K1_MAX);
        particle.K[2] = drand(K2_MIN,K2_MAX);

        PS.particle.push_back(particle);
    }
}

void Search::LRSearchCopyParticle(int p)
{
    DV.K[0] = PS.particle.at(p).K[0];
    DV.K[1] = PS.particle.at(p).K[1];
    DV.K[2] = PS.particle.at(p).K[2];
    DV.Angle = PS.particle.at(p).Angle;
    DV.Center = PS.particle.at(p).Center;

}

void Search::LRSearchMutate(QVector<PARTICLE> SelectedParticles)
{

    for(int cp=0;cp<SelectedParticles.size();cp++)      // Current Particle
        for(int pp=0;pp<SelectedParticles.size();pp++)  // Patner Particle
        {
            PARTICLE particle;
            particle.BestError = std::numeric_limits<double>::max();

            if(randChoice()==true)
                particle.Angle = SelectedParticles.at(cp).Angle + drand(ANGLE_MIN/10,ANGLE_MAX/10);
            else
                particle.Angle = SelectedParticles.at(pp).Angle + drand(ANGLE_MIN/10,ANGLE_MAX/10);

            int X = (int)drand(X_CENTER_MIN+20,X_CENTER_MAX-20);
            int Y = (int)drand(X_CENTER_MIN+20,X_CENTER_MAX-20);

            if(randChoice()==true)
                X = SelectedParticles.at(cp).Center.x() + 959-(int)drand(X_CENTER_MIN+20,X_CENTER_MAX-20);
            else
                X = SelectedParticles.at(pp).Center.x() + 959-(int)drand(X_CENTER_MIN+20,X_CENTER_MAX-20);

            if(randChoice()==true)
                Y = SelectedParticles.at(cp).Center.y() + 539-(int)drand(Y_CENTER_MIN+20,Y_CENTER_MAX-20);
            else
                Y = SelectedParticles.at(pp).Center.y() + 539-(int)drand(Y_CENTER_MIN+20,Y_CENTER_MAX-20);

            particle.Center.setX(X);
            particle.Center.setY(Y);

            if(randChoice()==true)
                particle.K[0] = SelectedParticles.at(cp).K[0] + drand(K0_MIN/K_RANDOMNESS,K0_MAX/K_RANDOMNESS);
            else
                particle.K[0] = SelectedParticles.at(pp).K[0] + drand(K0_MIN/K_RANDOMNESS,K0_MAX/K_RANDOMNESS);

            if(randChoice()==true)
                particle.K[1] = SelectedParticles.at(cp).K[1] + drand(K0_MIN/K_RANDOMNESS,K0_MAX/K_RANDOMNESS);
            else
                particle.K[1] = SelectedParticles.at(pp).K[1] + drand(K0_MIN/K_RANDOMNESS,K0_MAX/K_RANDOMNESS);

            if(randChoice()==true)
                particle.K[2] = SelectedParticles.at(cp).K[2] + drand(K0_MIN/K_RANDOMNESS,K0_MAX/K_RANDOMNESS);
            else
                particle.K[2] = SelectedParticles.at(pp).K[2] + drand(K0_MIN/K_RANDOMNESS,K0_MAX/K_RANDOMNESS);


            PS.particle.push_back(particle);
        }

}
/*
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

struct PSO_GLOBAL
{
    QVector <PSO_PARTICLE> P;
    double theta1;
    double BestError;
}pso;
*/
#define PSO_PARTICLES 2000
#define V_MAX 0.9
#define V_MIN -0.9
#define T1_MIN 0
#define T1_MAX +0.9
#define T2_MIN 0.0
#define T2_MAX +0.0001
#define T3_MIN 0
#define T3_MAX +0.0001


/*
#define ANGLE_MIN -1
#define ANGLE_MAX 1
#define X_CENTER_MIN 934
#define X_CENTER_MAX 984
#define Y_CENTER_MIN 514
#define Y_CENTER_MAX 563

#define K0_MIN -0.2
#define K0_MAX 0.2
#define K1_MIN -0.2
#define K1_MAX 0.2
#define K2_MIN -0.2
#define K2_MAX 0.2
*/

void Search::PSO(int maxEvaluations)
{
        QDateTime cd = QDateTime::currentDateTime();
        qsrand(cd.toTime_t());

        PSOInit();

        PSO_PARTICLE pP;

        for(int i=0;i<pso.P.size();i++)
        {
        pP = PSOPerturb(pso.P.at(i));
        double Error = PSOFunction(pP);
        if(Error < pso.P.at(i).BestError)
            {
            pso.P[i] = pP;
            pso.P[i].BestError = Error;
            if(Error < pso.BestError)
                pso.BestError = Error;
            }
        }




}

void Search::PSOInit()
{
    pso.BestError = std::numeric_limits<double>::max();
    pso.theta1 = drand(T1_MIN,T1_MAX);

    for(int i=0;i<PSO_PARTICLES;i++)
    {
        PSO_PARTICLE P;
        P.Angle = drand(ANGLE_MIN,ANGLE_MAX);
        int X = (int)drand(X_CENTER_MIN,X_CENTER_MAX);
        int Y = (int)drand(Y_CENTER_MIN,Y_CENTER_MAX);
        P.Center.setX(X);
        P.Center.setY(Y);
        P.K[0] = drand(K0_MIN,K0_MAX);
        P.K[1] = drand(K1_MIN,K1_MAX);
        P.K[2] = drand(K2_MIN,K2_MAX);
        P.V = drand(V_MIN,V_MAX);
        P.theta2 = drand(T2_MIN,T2_MAX);
        P.theta3 = drand(T3_MIN,T3_MAX);
        P.BestError = std::numeric_limits<double>::max();

        P.BestError = PSOFunction(P);
        if(P.BestError < pso.BestError)
            pso.BestError = P.BestError;

        pso.P.push_back(P);
    }

    qDebug() << "PSO Init Done....";

}

double Search::PSOFunction(int Particle)
{
    DV.Angle = pso.P.at(Particle).Angle;
    DV.K[0] = pso.P.at(Particle).K[0];
    DV.K[1] = pso.P.at(Particle).K[1];
    DV.K[2] = pso.P.at(Particle).K[2];
    DV.Center = pso.P.at(Particle).Center;

    double Error = LDC.getLDCError(DV);

    return Error;
}

double Search::PSOFunction(PSO_PARTICLE Particle)
{
    DV.Angle   = Particle.Angle;
    DV.K[0]   = Particle.K[0];
    DV.K[1]   = Particle.K[1];
    DV.K[2]   = Particle.K[2];
    DV.Center = Particle.Center;

    double Error = LDC.getLDCError(DV);

    return Error;
}

PSO_PARTICLE Search::PSOPerturb(PSO_PARTICLE P)
{
    PSO_PARTICLE pP;

    double CurrentError = PSOFunction(P);

    double NewVelocity = pso.theta1*P.V+ P.theta2*(P.BestError-CurrentError) +P.theta3*(pso.BestError-CurrentError);

    pP.Angle = P.Angle + NewVelocity*(ANGLE_MAX-ANGLE_MIN);
    pP.K[0] = P.K[0]   + NewVelocity*(K0_MAX-K0_MIN);
    pP.K[1] = P.K[1]   + NewVelocity*(K1_MAX-K1_MIN);
    pP.K[2] = P.K[2]   + NewVelocity*(K1_MAX-K1_MIN);
    int dX = (int)(NewVelocity*(+34+34));
    int dY = (int)(NewVelocity*(+34+34));

    pP.Center.setX(pP.Center.x()+dX);
    pP.Center.setY(pP.Center.y()+dY);

    return pP;

}

