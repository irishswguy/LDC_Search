#ifndef DISTORTION_H
#define DISTORTION_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QtMath>

struct DISTORTION_VARS
{
    QVector <QPoint> reference;
    QVector <QPoint> distorted;
    QVector <QPoint> undistorted;
    QVector <double> K;
    double Angle;
    QPoint Center;
};

class Distortion : public QObject
{
    Q_OBJECT
public:
    explicit Distortion(QObject *parent = nullptr);
    double getLDCError();
    double GetDistanceBetweenPoints(QPoint P1, QPoint P2);
    QPoint getUndistortedPoint(QPoint p);

    struct DISTORTION_VARS DV;

signals:

public slots:

private:

};

#endif // DISTORTION_H
