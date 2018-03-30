#include "distortion.h"

Distortion::Distortion(QObject *parent) : QObject(parent)
{

}

//-------------------------------------------------------------------------------------------------------------
//
//  The Function LDC.getLDCError()  calculates the absolute difference
//  between the 'referenece' pixels and the transformed ( by k1 & k2 )
//  'distorted' pixels.
//
//  The cordianates of the transformed points are stored in the 'undistorted'
//  vector.
//
//-------------------------------------------------------------------------------------------------------------
double Distortion::getLDCError(QVector<QPoint> reference, QVector<QPoint> real, QVector<QPoint> &undistorted,QVector<double> K)
{
    double error=0;

    undistorted.clear();

    for(int i=0; i< reference.size(); i++)
    {
    QPoint UndistortedPoint = getUndistortedPoint(real.at(i),K);
    error += qAbs(GetDistanceBetweenPoints(reference.at(i),UndistortedPoint));
    undistorted.push_back(UndistortedPoint);
    }
    return error;
}
//-------------------------------------------------------------------------------------------------------------
//
//  Overloaded version of getLDCError
//
//-------------------------------------------------------------------------------------------------------------
double Distortion::getLDCError(QVector<QPoint> reference, QVector<QPoint> real, QVector<QPoint> &undistorted,double k1, double k2)
{
    QVector <double> K;

    K.push_back(k1);
    K.push_back(k2);

    return getLDCError(reference, real, undistorted, K);
}
//-------------------------------------------------------------------------------------------------------------
//
//  Function returns distance between two
//
//-------------------------------------------------------------------------------------------------------------
double Distortion::GetDistanceBetweenPoints(QPoint P1, QPoint P2)
{
    QPoint diff = P1-P2;
    double Distance = qSqrt((diff.x()*diff.x()) + (diff.y()*diff.y()));

    return(Distance);
}
//-------------------------------------------------------------------------------------------------------------
//
//  Overloaded version of getLDCError
//
//-------------------------------------------------------------------------------------------------------------
QPoint Distortion::getUndistortedPoint(QPoint p, QVector<double> K)
{
    QPoint image_centre(959, 539);

    double radius = GetDistanceBetweenPoints(p,image_centre);

    double normalized_radius = radius / 1100.0918;

    double x_corrected = 0;
    double y_corrected = 0;
    double temp = 0;
    double r2 = normalized_radius * normalized_radius;

    temp = (1 + (K.at(0) * r2)+ (K.at(1) * r2 *r2));

    x_corrected = 959 + ( (p.x() - 959) * temp);
    y_corrected = 539 + ( (p.y() - 539) * temp);

    QPoint UndistortedPoint;

    UndistortedPoint.setX(x_corrected);
    UndistortedPoint.setY(y_corrected);

    return UndistortedPoint;
}
