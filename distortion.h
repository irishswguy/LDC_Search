#ifndef DISTORTION_H
#define DISTORTION_H

#include <QObject>
#include <QVector>
#include <QPoint>
#include <QtMath>

class Distortion : public QObject
{
    Q_OBJECT
public:
    explicit Distortion(QObject *parent = nullptr);
    double getLDCError(QVector <QPoint> reference,QVector <QPoint> real, QVector<QPoint> &Undistorted  ,QVector <double> K );
    double getLDCError(QVector<QPoint> reference, QVector<QPoint> real, QVector<QPoint> &undistorted,double k1, double k2);
    double GetDistanceBetweenPoints(QPoint P1, QPoint P2);
    QPoint getUndistortedPoint(QPoint p,QVector <double> K);

signals:

public slots:
};

#endif // DISTORTION_H
