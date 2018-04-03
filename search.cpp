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
    Cancel = false;

}

void Search::process() {

    qDebug() << "Search Process Started";

    double bounds[]={-0.2,0.2};
    S(bounds,150);

    SearchStatus.qsStatus ="";
    emit signalUpdateTextStatus("Search is finished.....");

    emit signalUpdateTextStatus(" ");
    emit signalUpdateTextStatus("---------Best Results-------");
    emit signalUpdateTextStatus("  K0 :"+QString::number(DV.K[0]));
    emit signalUpdateTextStatus("  K1 :"+QString::number(DV.K[1]));
    emit signalUpdateTextStatus("  K2 :"+QString::number(DV.K[2]));
    emit signalUpdateTextStatus("  K3 :"+QString::number(DV.K[3]));
    emit signalUpdateTextStatus("  Angle    = 0.964");
    emit signalUpdateTextStatus("  Centre   = (567,234)");
    emit signalUpdateTextStatus(" ");


    SearchStatus.Finished=true;
    emit finished();
    return;

CANCEL:
    qDebug() << "Search Process Canceled";
    SearchStatus.Finished=true;
    emit finished();

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
    double BestError=std::numeric_limits<double>::max();
    double BestK1;
    double BestK2;

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

        // Initalise the Startup-Conditions
        for(int i=0;i<PROBLEM_DIM;i++){
            best[i] = drand(bounds[0],bounds[1]);
            StepSize[i]= 0.4 * (bounds[1]-bounds[0]);   // Step is 40% of bounds
            }

        QDateTime cd = QDateTime::currentDateTime();
        qsrand(cd.toTime_t());

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
                        DV.K[i] = best[i]+(StepSize[i]/2);
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
                        StepSize[i]= StepSize[i]/2;

                SearchStatus.error.push_back(fBest);
                SearchStatus.K[0].push_back(DV.K[0]);
                SearchStatus.K[1].push_back(DV.K[1]);
                SearchStatus.K[2].push_back(DV.K[2]);
                SearchStatus.Angle.push_back(0.15);
                SearchStatus.Centre= QPoint(500,500);
                DV2=DV;
                SearchStatus.qsStatus = "Search Number: " + QString::number(j) + " Error: " + QString::number(fBest);
                emit updateStatus();

                qDebug() << "j:" << j;

                QThread::msleep(500);

            }

//        Results Data;                                                                               // Save the data for this run for
//        Data.Best = fBest;
//         for(int i=0;i<PROBLEM_DIM;i++)   // statistical analysis later
//            Data.K.push_back(best[i]);

//        StatisticalData.push_back(Data);
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


