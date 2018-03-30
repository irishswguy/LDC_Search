#ifndef STATISTICS_H
#define STATISTICS_H

#include <QObject>
#include <QVector>
#include <QtMath>

class Statistics : public QObject
{
    Q_OBJECT
public:
    explicit Statistics(QObject *parent = nullptr);

    double getMean(QVector <double> data);
    double getStdDeviation(QVector <double> data,double mean);

signals:

public slots:
};

#endif // STATISTICS_H
