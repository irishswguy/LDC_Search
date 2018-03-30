#include "statistics.h"

Statistics::Statistics(QObject *parent) : QObject(parent)
{

}
// Return the mean of a vector of doubles
double Statistics::getMean(QVector <double> data)
{
    double sum=0;
    double mean=0;

    for(int i=0; i< data.size(); i++)
    {
    sum+=data.at(i);
    }

    if(data.size()!=0)
        mean = sum/data.size();

    return mean;
}
// Return the standard deviation of a vector of doubles
double Statistics::getStdDeviation(QVector <double> data,double mean)
{
    double sum=0;
    double stdDeviation=0;

    for(int i=0; i< data.size(); i++)
    {
    sum+=qPow(mean-data.at(i),2);
    }

    if(data.size()!=0)
        stdDeviation = qSqrt((1/(double)data.size())*sum);

    return stdDeviation;
}
