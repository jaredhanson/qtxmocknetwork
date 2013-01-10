#ifndef QTXMOCKNETWORK_MOCKNETWORKACCESSMANAGER_H
#define QTXMOCKNETWORK_MOCKNETWORKACCESSMANAGER_H

#include "mocknetworkglobal.h"
#include <QtNetwork>
#include <QtCore>

QTX_BEGIN_NAMESPACE


class MockNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
    
public:
    MockNetworkAccessManager(QObject *parent = 0);
    virtual ~MockNetworkAccessManager();
};


QTX_END_NAMESPACE

#endif // QTXMOCKNETWORK_MOCKNETWORKACCESSMANAGER_H
