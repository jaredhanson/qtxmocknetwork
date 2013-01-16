#ifndef QTXMOCKNETWORK_MOCKNETWORKACCESSMANAGER_H
#define QTXMOCKNETWORK_MOCKNETWORKACCESSMANAGER_H

#include "mocknetworkglobal.h"
#include <QtNetwork>
#include <QtCore>

QTX_BEGIN_NAMESPACE


class IMockNetworkAccessManagerDelegate;
class MockNetworkAccessManagerPrivate;

class MockNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
    
public:
    MockNetworkAccessManager(QObject *parent = 0);
    virtual ~MockNetworkAccessManager();
    
    void setDelegate(IMockNetworkAccessManagerDelegate *delegate);
    
protected:
    virtual QNetworkReply *createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0);

protected:
    MockNetworkAccessManagerPrivate *d_ptr;
private:
    Q_DECLARE_PRIVATE(MockNetworkAccessManager);
};


QTX_END_NAMESPACE

#endif // QTXMOCKNETWORK_MOCKNETWORKACCESSMANAGER_H
