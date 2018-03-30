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
double Distortion::getLDCError()
{
    double error=0;

    DV.undistorted.clear();

    for(int i=0; i< DV.reference.size(); i++)
    {
    QPoint UndistortedPoint = getUndistortedPoint(DV.distorted.at(i));
    error += qAbs(GetDistanceBetweenPoints(DV.reference.at(i),UndistortedPoint));
    DV.undistorted.push_back(UndistortedPoint);
    }
    return error;
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
QPoint Distortion::getUndistortedPoint(QPoint p)
{
    QPoint image_centre(959, 539);

    double radius = GetDistanceBetweenPoints(p,image_centre);

    double normalized_radius = radius / 1100.0918;

    double x_corrected = 0;
    double y_corrected = 0;
    double temp = 0;
    double r2 = normalized_radius * normalized_radius;

    temp = (1 + (DV.K.at(0) * r2)+ (DV.K.at(1) * r2 *r2));

    x_corrected = 959 + ( (p.x() - 959) * temp);
    y_corrected = 539 + ( (p.y() - 539) * temp);

    QPoint UndistortedPoint;

    UndistortedPoint.setX(x_corrected);
    UndistortedPoint.setY(y_corrected);

    return UndistortedPoint;
}
