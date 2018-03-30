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
        SearchStatus.Centre= QPoint(500+i,500+i);
        SearchStatus.qsStatus = "Search Number:" + QString::number(i);
        emit updateStatus();
        emit signalUpdateTextStatus("Test"+QString::number(i));
        QThread::msleep(100);
        if (Cancel == true) goto CANCEL;
        }

    SearchStatus.qsStatus ="";
    emit signalUpdateTextStatus("Search is finished.....");

    emit signalUpdateTextStatus(" ");
    emit signalUpdateTextStatus("---------Best Results-------");
    emit signalUpdateTextStatus("  K0       = 0.964");
    emit signalUpdateTextStatus("  K2       = 0.964");
    emit signalUpdateTextStatus("  K3       = 0.964");
    emit signalUpdateTextStatus("  Angle    = 0.964");
    emit signalUpdateTextStatus("  Centre   = (567,234)");
    emit signalUpdateTextStatus(" ");


    SearchStatus.Finished=true;
    emit finished();
    return;

CANCEL:
    qDebug() << "Search Process Canceled";
    SearchStatus.Finished=true;
    emit finished();

}

