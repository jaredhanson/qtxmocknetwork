#include "mocknetworkaccessmanager.h"
#include "mockhttpnetworkreply.h"
#include "imocknetworkaccessmanagerdelegate.h"

QTX_BEGIN_NAMESPACE


class MockNetworkAccessManagerPrivate
{
public:
    MockNetworkAccessManagerPrivate(MockNetworkAccessManager *q);
    virtual ~MockNetworkAccessManagerPrivate();
    
public:
    MockNetworkAccessManager *q_ptr;
    Q_DECLARE_PUBLIC(MockNetworkAccessManager);
    
    IMockNetworkAccessManagerDelegate* delegate;
};


MockNetworkAccessManager::MockNetworkAccessManager(QObject *parent /* = 0 */)
    : QNetworkAccessManager(parent),
      d_ptr(new MockNetworkAccessManagerPrivate(this))
{
}

MockNetworkAccessManager::~MockNetworkAccessManager()
{
    if (d_ptr) {
        delete d_ptr;
        d_ptr = 0;
    }
}

void MockNetworkAccessManager::setDelegate(IMockNetworkAccessManagerDelegate *delegate)
{
    Q_D(MockNetworkAccessManager);
    d->delegate = delegate;
}

QNetworkReply *MockNetworkAccessManager::createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData /* = 0 */)
{
    Q_D(MockNetworkAccessManager);
    if (d->delegate) {
        QIODevice *data = d->delegate->createIncomingData(req);
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


MockNetworkAccessManagerPrivate::MockNetworkAccessManagerPrivate(MockNetworkAccessManager *q)
    : q_ptr(q),
      delegate(0)
{
}

MockNetworkAccessManagerPrivate::~MockNetworkAccessManagerPrivate()
{
    delegate = 0;
}


QTX_END_NAMESPACE
