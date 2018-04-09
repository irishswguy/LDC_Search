#ifndef DISTORTION_H
#define DISTORTION_H

#pragma once

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QtMath>


struct DISTORTION_VARS
{
    QVector <QPoint> referencePoints;
    QVector <QPoint> distortedPoints;
    QVector <QPoint> undistortedPoints;
    double K[3];
    double Angle;
    QPoint Center;
};

class Distortion : public QObject
{
    Q_OBJECT
public:
    explicit Distortion(QObject *parent = nullptr);
    double getLDCError(DISTORTION_VARS &DV);
    double GetDistanceBetweenPoints(QPoint P1, QPoint P2);
    QPoint getUndistortedPoint(QPoint p,DISTORTION_VARS DV);
    QPoint rotate_point(QPoint P, QPoint Centre,float angle);

    //struct DISTORTION_VARS DV;

    bool ExtraK = false;



signals:

public slots:

private:

};

#endif // DISTORTION_H
