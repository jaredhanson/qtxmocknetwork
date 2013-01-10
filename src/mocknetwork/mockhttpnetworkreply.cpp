#include "mockhttpnetworkreply.h"

QTX_BEGIN_NAMESPACE


class MockHttpNetworkReplyPrivate
{
public:
    MockHttpNetworkReplyPrivate(MockHttpNetworkReply *q);
    virtual ~MockHttpNetworkReplyPrivate();
    
    void receive();
    void parse();
    
    qint64 readData(char *data, qint64 maxSize);
    qint64 writeData(const char *data, qint64 maxSize);
    
public:
    MockHttpNetworkReply *q_ptr;
    Q_DECLARE_PUBLIC(MockHttpNetworkReply);
    
    QIODevice *device;
    QByteArray buffer;
};


MockHttpNetworkReply::MockHttpNetworkReply(QIODevice *device, QObject *parent /* = 0 */)
    : QNetworkReply(parent),
      d_ptr(new MockHttpNetworkReplyPrivate(this))
{
    device->setParent(this);
    d_ptr->device = device;
}

MockHttpNetworkReply::~MockHttpNetworkReply()
{
    if (d_ptr) {
        delete d_ptr;
        d_ptr = 0;
    }
}

bool MockHttpNetworkReply::open(OpenMode mode)
{
    QMetaObject::invokeMethod(this, "doReceive", Qt::QueuedConnection);
    return QIODevice::open(mode);
}

void MockHttpNetworkReply::abort()
{
}

void MockHttpNetworkReply::doReceive()
{
    Q_D(MockHttpNetworkReply);
    d->receive();
}

qint64 MockHttpNetworkReply::readData(char *data, qint64 maxSize)
{
    Q_D(MockHttpNetworkReply);
    return d->readData(data, maxSize);
}

qint64 MockHttpNetworkReply::writeData(const char *data, qint64 maxSize)
{
    Q_D(MockHttpNetworkReply);
    return d->writeData(data, maxSize);
}


MockHttpNetworkReplyPrivate::MockHttpNetworkReplyPrivate(MockHttpNetworkReply *q)
    : q_ptr(q),
      device(0)
{
}

MockHttpNetworkReplyPrivate::~MockHttpNetworkReplyPrivate()
{
    device = 0;
}

void MockHttpNetworkReplyPrivate::receive()
{
    buffer = device->readAll();
    parse();
}

void MockHttpNetworkReplyPrivate::parse()
{
    Q_Q(MockHttpNetworkReply);
    
    QString version;
    quint16 statusCode;
    QString reasonPhrase;
    

    int idx = buffer.indexOf("\r\n");
                
    // The response is invalid, due to the absence of a delimiter signalling the
    // end of the status line.
    if (-1 == idx) {
        emit q->error(QNetworkReply::ProtocolFailure);
        return;
    }
            
            
    QByteArray statusLine = buffer.left(idx);
    buffer.remove(0, idx + 2);

    // Parse out the HTTP version. If the space character, which separates it
    // from the status code, is not found, then the response is invalid.
    idx = statusLine.indexOf(' ');
    if (-1 == idx) {
        emit q->error(QNetworkReply::ProtocolFailure);
        return;
    }
    if (!statusLine.startsWith("HTTP/")) {
        emit q->error(QNetworkReply::ProtocolFailure);
        return;
    }
            
    version = QString::fromUtf8(statusLine.mid(5, idx - 5));
    statusLine.remove(0, idx + 1);

    // Parse out the status code. If the space character, which separates it
    // from the reason phrase, is not found, then the request is invalid.
    idx = statusLine.indexOf(' ');
    if (-1 == idx) {
        emit q->error(QNetworkReply::ProtocolFailure);
        return;
    }
            
    statusCode = QString::fromUtf8(statusLine.left(idx)).toInt();
    statusLine.remove(0, idx + 1);
    q->setAttribute(QNetworkRequest::HttpStatusCodeAttribute, statusCode);

    // The remainder of the status line consists of the reason phrase.
    reasonPhrase = statusLine;
    q->setAttribute(QNetworkRequest::HttpReasonPhraseAttribute, reasonPhrase);


    idx = buffer.indexOf("\r\n\r\n");
            
    // The response is invalid, due to the absence of a delimiter signalling the
    // end of the headers.
    if (-1 == idx) {
        emit q->error(QNetworkReply::ProtocolFailure);
        return;
    }
    
    while (buffer.size() > 0) {
        idx = buffer.indexOf("\r\n");

        // The parser has encountered the end of headers.  The remainder of the
        // buffer is the body of the message.
        if (0 == idx) {
            buffer.remove(0, idx + 2);
            break;
        }

        QByteArray header = buffer.left(idx);
        QByteArray field;
        QByteArray value;

        int idy = header.indexOf(':');
        if (-1 == idy) {
            emit q->error(QNetworkReply::ProtocolFailure);
            return;
        }
        
        field = header.left(idy);
        value = header.mid(idy + 2);
        q->setRawHeader(field, value);
        
        if ((QString::fromUtf8(field) == "Location") && (statusCode / 100 == 3)) {
            q->setAttribute(QNetworkRequest::RedirectionTargetAttribute, QUrl(QString::fromUtf8(value)));
        }

        buffer.remove(0, idx + 2);
    }

    emit q->metaDataChanged();
    emit q->readyRead();
    
    if (statusCode / 100 == 5) {
       emit q->error(QNetworkReply::UnknownContentError);
    }
    
    emit q->finished();
}

qint64 MockHttpNetworkReplyPrivate::readData(char *data, qint64 maxSize)
{
    if (buffer.size() == 0)
        return -1;

    qint64 size = maxSize;
    if (buffer.size() < maxSize)
        size = buffer.size();

    memcpy(data, buffer.data(), size);
    buffer.remove(0, size);
    return size;
}

qint64 MockHttpNetworkReplyPrivate::writeData(const char *data, qint64 maxSize)
{
    Q_UNUSED(data)
    Q_UNUSED(maxSize)

    return -1;
}


QTX_END_NAMESPACE
