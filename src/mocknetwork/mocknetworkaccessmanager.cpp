#include "mocknetworkaccessmanager.h"
#include "mockhttpnetworkreply.h"
#include "imocknetworkaccessmanagerdelegate.h"

QTX_BEGIN_NAMESPACE


MockNetworkAccessManager::MockNetworkAccessManager(QObject *parent /* = 0 */)
    : QNetworkAccessManager(parent),
      mDelegate(0)
{
}

MockNetworkAccessManager::~MockNetworkAccessManager()
{
}

void MockNetworkAccessManager::setDelegate(IMockNetworkAccessManagerDelegate *delegate)
{
    mDelegate = delegate;
}

QNetworkReply *MockNetworkAccessManager::createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData /* = 0 */)
{
    qDebug() << "MockNetworkAccessManager::createRequest";
    qDebug() << "  url: " << req.url();
    qDebug() << "  scheme: " << req.url().scheme();

    if (mDelegate) {
        QIODevice *data = mDelegate->incomingDataForRequest(req);
        if (data) {
            QString scheme = req.url().scheme();
            QNetworkReply *reply = 0;
            
            if ("http" == scheme) {
                reply = new MockHttpNetworkReply(data, this);
            }
            
            if (reply) {
                reply->open(QIODevice::ReadOnly);
                return reply;
            }
        }
    }

    return QNetworkAccessManager::createRequest(op, req, outgoingData);
}


QTX_END_NAMESPACE
