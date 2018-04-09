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
double Distortion::getLDCError(DISTORTION_VARS &DV)
{
    double error=0;

    DV.undistortedPoints.clear();


    for(int i=0; i< DV.referencePoints.size(); i++)
    {
    QPoint UndistortedPoint = getUndistortedPoint(DV.distortedPoints.at(i),DV);
    error += qAbs(GetDistanceBetweenPoints(DV.referencePoints.at(i),UndistortedPoint));
    DV.undistortedPoints.push_back(UndistortedPoint);
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
//
//
//-------------------------------------------------------------------------------------------------------------
QPoint Distortion::getUndistortedPoint(QPoint p,DISTORTION_VARS DV)
{

    QPoint CentrePoint(DV.Center.x(),DV.Center.y());

    double radius = GetDistanceBetweenPoints(p,CentrePoint);

    double normalized_radius = radius / 1100.0918;

    double x_corrected = 0;
    double y_corrected = 0;
    double temp = 0;
    double r2 = normalized_radius * normalized_radius;

    temp = 1 + (DV.K[0] * r2) + (DV.K[1] * r2 *r2) + (DV.K[2] *r2*r2*r2*r2)+(DV.K[3] *r2*r2*r2*r2*r2*r2*r2*r2);

    x_corrected = CentrePoint.x() + ( (p.x() - CentrePoint.x()) * temp);
    y_corrected = CentrePoint.y() + ( (p.y() - CentrePoint.y()) * temp);

    QPoint UndistortedPoint;

    UndistortedPoint.setX(x_corrected);
    UndistortedPoint.setY(y_corrected);

    UndistortedPoint = rotate_point(UndistortedPoint,CentrePoint,DV.Angle);

    return UndistortedPoint;
}


QPoint Distortion::rotate_point(QPoint P, QPoint Centre,float angle)
{
  angle = angle / 57.3;

  double s = qSin(angle);
  double c = qCos(angle);

  // translate point back to origin:
  P.setX(P.x()-Centre.x());
  P.setY(P.y()-Centre.y());

  // rotate point
  double xnew = P.x() * c - P.y() * s;
  double ynew = P.x() * s + P.y() * c;

  // translate point back:
  int x = (int)(xnew + Centre.x());
  int y = (int)(ynew + Centre.y());

  QPoint Rotated(x,y);
  return Rotated;
}
