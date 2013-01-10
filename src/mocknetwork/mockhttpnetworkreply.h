#ifndef QTXMOCKNETWORK_MOCKHTTPNETWORKREPLY_H
#define QTXMOCKNETWORK_MOCKHTTPNETWORKREPLY_H

#include "mocknetworkglobal.h"
#include <QtNetwork>
#include <QtCore>

QTX_BEGIN_NAMESPACE


class MockHttpNetworkReplyPrivate;

class MockHttpNetworkReply : public QNetworkReply
{
    Q_OBJECT
    
public:
    MockHttpNetworkReply(QIODevice *device, QObject *parent = 0);
    virtual ~MockHttpNetworkReply();
    
    virtual bool open(OpenMode mode);
    
public slots:
    virtual void abort();
    
protected:
    virtual qint64 readData(char *data, qint64 maxSize);
    virtual qint64 writeData(const char *data, qint64 maxSize);
    
private slots:
    void doReceive();
    
protected:
    MockHttpNetworkReplyPrivate *d_ptr;
private:
    Q_DECLARE_PRIVATE(MockHttpNetworkReply);
};


QTX_END_NAMESPACE

#endif // QTXMOCKNETWORK_MOCKHTTPNETWORKREPLY_H
