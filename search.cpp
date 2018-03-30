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

    double SearchError=0.0;
    for(int i=0;i<360;i++){
        SearchError = (double)i/50.0;
        SearchStatus.error.push_back(SearchError);
        SearchStatus.K[0].push_back(qCos((double)i/10));
        SearchStatus.K[1].push_back(qCos((double)i/20.0));
        SearchStatus.K[2].push_back(qCos((double)i/57.8));
        SearchStatus.Angle.push_back(qSin((double)i/57.8));
        SearchStatus.Centre.push_back(QPoint(10,10));
        SearchStatus.qsStatus = "Search Number:" + QString::number(i);
        emit updateStatus();
        QThread::msleep(100);
        if (Cancel == true) goto CANCEL;
        }

    emit error("This is a signal");


    qDebug() << "Search Process Finished";
    SearchStatus.Finished=true;
    emit finished();
    return;

CANCEL:
    qDebug() << "Search Process Canceled";
    SearchStatus.Finished=true;
    emit finished();

}
