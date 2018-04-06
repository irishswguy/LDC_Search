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

void Search::process() {

    qDebug() << "Search Process Started";

    SearchStatus.ResultsStatus.clear();


    if( LR_Search == true)
            LRSearch();

    if(S_Search==true){
        double bounds[]={-0.2,0.2};
        SearchStatus.ResultsStatus.push_back("Starting S-Search Algrothim");
        SearchStatus.ResultsStatus.push_back("------------------------------------------");
        emit updateStatus();

        QThread::msleep(500);
        SearchStatus.ResultsStatus.clear();

        for(int i=0;i<TestCycles;i++){
            Start();
            SearchStatus.CurrentTestCycle=i;;

            S(bounds,250);

            SearchStatus.ResultsStatus.push_back("Test Cycle : "+QString::number(i+1)+" Error : " + QString::number(StatisticalData.at(i),'f',2));
            emit updateStatus();
            QThread::msleep(500);
            SearchStatus.ResultsStatus.clear();
            emit finished();
            }
        double Mean = statistics.getMean(StatisticalData);
        double StdDev = statistics.getStdDeviation(StatisticalData,Mean);

        SearchStatus.ResultsStatus.push_back("---------Best Results-------");
        SearchStatus.ResultsStatus.push_back("  K0 :"+QString::number(DV.K[0]));
        SearchStatus.ResultsStatus.push_back("  K1 :"+QString::number(DV.K[1]));
        SearchStatus.ResultsStatus.push_back("  K2 :"+QString::number(DV.K[2]));
        SearchStatus.ResultsStatus.push_back("  Angle    = 0.964");
         SearchStatus.ResultsStatus.push_back("  Centre   = (567,234)");
        SearchStatus.ResultsStatus.push_back(" ");
        SearchStatus.ResultsStatus.push_back("Mean   : " + QString::number(Mean,'f',1));
        SearchStatus.ResultsStatus.push_back("StdDev : " + QString::number(StdDev,'f',1));
        SearchStatus.ResultsStatus.push_back(" ");
        SearchStatus.Finished=true;
        emit updateStatus();

        }

    if(GP_Search==true)
        {
        qDebug() << "To Be Done....";
        }


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

//-------------------------------------------------------------------------------------------------------------
//
//  This function performs a search for the best constants K1 and K2.
//
//  The Reference pixel positions are held in the vector 'reference'.
//
//  The Distorted pixel positions are keld in the vector 'distorted'.
//
//  The Function LDC.getLDCError()  calculates the absolute difference
//  between the 'refernece' pixels and the transformed ( by k1 & k2 )
//  'distorted' pixels.
//
//  Results from the run are stored in the 'StatisticalData' vector.
//
//-------------------------------------------------------------------------------------------------------------
void Search::S(double bounds[PROBLEM_DIM],int maxEvaluations)
{
double  best[PROBLEM_DIM];
double  fBest = std::numeric_limits<double>::max();
int     j = 0;
double  StepSize[PROBLEM_DIM];


        QDateTime cd = QDateTime::currentDateTime();
        qsrand(cd.toTime_t());

        // Initalise the Startup-Conditions
        for(int i=0;i<PROBLEM_DIM;i++){
            best[i] = drand(bounds[0],bounds[1]);
            StepSize[i]= 0.04 * (bounds[1]-bounds[0]);   // Step is 40% of bounds
            }


        DV.K[0] = drand(-0.2,0.2);
        DV.K[1] = drand(-0.2,0.2);
        DV.K[2] = drand(-0.2,0.2);

        toro(DV.K, bounds); // Fix out of bounds conditions

        while (j < maxEvaluations)
            {
            if(Cancel==true)
                return;
            bool Improved=false;
            for(int i = 0; i < PROBLEM_DIM && j < maxEvaluations; ++i)                              // Core iteration step
                {
                    fBest = LDC.getLDCError(DV);       // Get current best error

                    DV.K[i] = best[i]-StepSize[i];                                                     // Adjust K
                    toro(DV.K, bounds);

                    double fitness = LDC.getLDCError(DV);    // Get new fittness

                    if(fitness < fBest)                                                             // If improved then save
                    {
                        best[i] = DV.K[i];
                        Improved = true;
                        fBest = fitness;
                    }
                    else                                                                            // Else test 1/2 step opposite direction
                    {
                        DV.K[i] = best[i]+(StepSize[i]/1.0);
                        toro(DV.K, bounds);
                        fitness = LDC.getLDCError(DV);

                        if(fitness < fBest)                                                         // If improved save
                        {
                            best[i] = DV.K[i];
                            Improved = true;
                            fBest = fitness;
                        }
                        else
                            DV.K[i]=best[i];                                                           // If no improvment in this step
                    }                                                                               // go back to last value
                j++;
                }

                if(Improved==false)                                                                 // If no improvment in any dimension
                    for(int i=0;i<PROBLEM_DIM;i++)                                                  // reduce all step sizes by half.
                        StepSize[i]= StepSize[i]/1.1;

                SearchStatus.error.push_back(fBest);
                SearchStatus.K[0].push_back(DV.K[0]);
                SearchStatus.K[1].push_back(DV.K[1]);
                SearchStatus.K[2].push_back(DV.K[2]);
                SearchStatus.Angle.push_back(0.15);
                SearchStatus.Centre= QPoint(500,500);
                SearchStatus.qsSearchStatus = "S-Search, Cycle :" + QString::number(SearchStatus.CurrentTestCycle) +" Loop :" + QString::number(j);

                SearchStatus.SearchProgress = j*TestCycles;

                emit updateStatus();

                QThread::msleep(500);

            }


          StatisticalData.push_back(fBest);
}

//-------------------------------------------------------------------------------------------------------------
//
//  This function performs torroidal saturation of the vector using the bounds array
//
//-------------------------------------------------------------------------------------------------------------
void Search::toro(double (&x)[PROBLEM_DIM], double bounds[])
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

#define MAX_PARTICLES 1000
#define ANGLE_MIN -5
#define ANGLE_MAX 5
#define CENTER_MIN -100
#define CENTER_MAX 100
#define K0_MIN -0.2
#define K0_MAX 0.2
#define K1_MIN -0.2
#define K1_MAX 0.2
#define K2_MIN -0.2
#define K2_MAX 0.2
#define QTY_SELECTED_PARICLES 200
#define RANDOMNESS 10

void Search::LRSearch(void)
{
QVector <double> fitness;
QVector <PARTICLE> SelectedParticles;

    // Randomly Initalise All the Particles
    LRSearchInit();

    for(int j=0;j<5;j++)
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

        qDebug() << "j: " << j << " BestError : "<< PS.particle[0].BestError;
        qDebug() << "K[0]: " << PS.particle.at(0).K[0];
        qDebug() << "K[1]: " << PS.particle.at(0).K[1];
        qDebug() << "K[2]: " << PS.particle.at(0).K[2];

        SearchStatus.error.push_back(PS.particle[0].BestError);
        SearchStatus.K[0].push_back(PS.particle.at(0).K[0]);
        SearchStatus.K[1].push_back(PS.particle.at(0).K[1]);
        SearchStatus.K[2].push_back(PS.particle.at(0).K[2]);
        SearchStatus.Angle.push_back(0.15);
        SearchStatus.Centre= QPoint(500,500);
        SearchStatus.qsSearchStatus = "LRSearch Number: " + QString::number(j) + "    Error: " + QString::number(PS.particle[0].BestError,'f',1);

        LRSearchCopyParticle(0);
        LDC.getLDCError(DV);

        emit updateStatus();
        QThread::msleep(500);


        // Select Particles for mutation
        for(int i=0;i<QTY_SELECTED_PARICLES;i++)
            SelectedParticles.push_back(PS.particle.at(i));

        PS.particle.clear();

        // Perform Mutation of selected particles

        LRSearchMutate(SelectedParticles);
    }







}

void Search::LRSearchInit()
{
    QDateTime cd = QDateTime::currentDateTime();
    qsrand(cd.toTime_t());

    // Initalise the particles
    for(int i=0;i<MAX_PARTICLES;i++)
    {
        PARTICLE particle;
        particle.BestError = std::numeric_limits<double>::max();
        particle.Angle = drand(ANGLE_MIN,ANGLE_MAX);

        int X = (int)drand(CENTER_MIN,CENTER_MAX);
        int Y = (int)drand(CENTER_MIN,CENTER_MAX);

        particle.Center.setX(X);
        particle.Center.setY(Y);

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

            int X = (int)drand(CENTER_MIN,CENTER_MAX);
            int Y = (int)drand(CENTER_MIN,CENTER_MAX);

            if(randChoice()==true)
                X = SelectedParticles.at(cp).Center.x() + drand(CENTER_MIN/10,CENTER_MAX/10);
            else
                X = SelectedParticles.at(pp).Center.x() + drand(CENTER_MIN/10,CENTER_MAX/10);

            if(randChoice()==true)
                Y = SelectedParticles.at(cp).Center.y() + drand(CENTER_MIN/10,CENTER_MAX/10);
            else
                Y = SelectedParticles.at(pp).Center.y() + drand(CENTER_MIN/10,CENTER_MAX/10);

            particle.Center.setX(X);
            particle.Center.setY(Y);

            if(randChoice()==true)
                particle.K[0] = SelectedParticles.at(cp).K[0] + drand(K0_MIN/10,K0_MAX/10);
            else
                particle.K[0] = SelectedParticles.at(pp).K[0] + drand(K0_MIN/10,K0_MAX/10);

            if(randChoice()==true)
                particle.K[1] = SelectedParticles.at(cp).K[1] + drand(K0_MIN/10,K0_MAX/10);
            else
                particle.K[1] = SelectedParticles.at(pp).K[1] + drand(K0_MIN/10,K0_MAX/10);

            if(randChoice()==true)
                particle.K[2] = SelectedParticles.at(cp).K[2] + drand(K0_MIN/10,K0_MAX/10);
            else
                particle.K[2] = SelectedParticles.at(pp).K[2] + drand(K0_MIN/10,K0_MAX/10);


            PS.particle.push_back(particle);
        }

}

