#ifndef SEARCH_H
#define SEARCH_H

#include <QObject>
#include <QDebug>
#include <QVector>
#include <QThread>
#include <QPoint>
#include <QtMath>

struct SEARCH_STATUS
{
    QVector <double> error;
    QVector <double> K[3];
    QVector <double> Angle;
    QPoint  Centre;
    bool Finished=false;
    QString qsStatus;
};

class Search : public QObject
{
    Q_OBJECT
public:
    explicit Search(QObject *parent = nullptr);
    ~Search();
    void Start();

    bool Cancel = false;
    struct SEARCH_STATUS SearchStatus;


public slots:
    void process();
signals:
    void finished();
    void updateStatus(void);
    void signalUpdateTextStatus(QString);
private:


public slots:
};

#endif // SEARCH_H
