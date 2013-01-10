#ifndef QTXMOCKNETWORK_MOCKHTTPNETWORKREPLY_H
#define QTXMOCKNETWORK_MOCKHTTPNETWORKREPLY_H

#include "mocknetworkglobal.h"
#include <QtNetwork>
#include <QtCore>

QTX_BEGIN_NAMESPACE


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
    
private:
    void parse();
    
private:
    QIODevice *mDevice;
    
    QString mVersion;
    quint16 mStatusCode;
    QString mReasonPhrase;
    QByteArray mBuffer;
};


QTX_END_NAMESPACE

#endif // QTXMOCKNETWORK_MOCKHTTPNETWORKREPLY_H
