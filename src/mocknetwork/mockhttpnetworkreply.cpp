#include "mockhttpnetworkreply.h"

QTX_BEGIN_NAMESPACE


MockHttpNetworkReply::MockHttpNetworkReply(QIODevice *device, QObject *parent /* = 0 */)
    : QNetworkReply(parent),
      mDevice(device)
{
    mDevice->setParent(this);
}

MockHttpNetworkReply::~MockHttpNetworkReply()
{
}

bool MockHttpNetworkReply::open(OpenMode mode)
{
    qDebug() << "MockHttpNetworkReply::open";

    QMetaObject::invokeMethod(this, "doReceive", Qt::QueuedConnection);
    return QIODevice::open(mode);
}

void MockHttpNetworkReply::abort()
{
}

void MockHttpNetworkReply::doReceive()
{
    qDebug() << "MockHttpNetworkReply::doReceive()";
    
    mBuffer = mDevice->readAll();
    parse();
}

void MockHttpNetworkReply::parse()
{
    int idx = mBuffer.indexOf("\r\n");
                
    // The response is invalid, due to the absence of a delimiter signalling the
    // end of the status line.
    if (-1 == idx)
    {
        emit error(QNetworkReply::ProtocolFailure);
        return;
    }
            
            
    QByteArray status_line = mBuffer.left(idx);
    mBuffer.remove(0, idx + 2);

    // Parse out the HTTP version. If the space character, which separates it
    // from the status code, is not found, then the response is invalid.
    idx = status_line.indexOf(' ');
    if (-1 == idx)
    {
        emit error(QNetworkReply::ProtocolFailure);
        return;
    }

    if (!status_line.startsWith("HTTP/"))
    {
        emit error(QNetworkReply::ProtocolFailure);
        return;
    }
            
    mVersion = QString::fromUtf8(status_line.mid(5, idx - 5));
    status_line.remove(0, idx + 1);

    // Parse out the status code. If the space character, which separates it
    // from the reason phrase, is not found, then the request is invalid.
    idx = status_line.indexOf(' ');
    if (-1 == idx)
    {
        emit error(QNetworkReply::ProtocolFailure);
        return;
    }
            
    mStatusCode = QString::fromUtf8(status_line.left(idx)).toInt();
    status_line.remove(0, idx + 1);
    
    setAttribute(QNetworkRequest::HttpStatusCodeAttribute, mStatusCode);

    // The remainder of the status line consists of the reason phrase.
    mReasonPhrase = status_line;


    idx = mBuffer.indexOf("\r\n\r\n");
            
    // The response is invalid, due to the absence of a delimiter signalling the
    // end of the headers.
    if (-1 == idx)
    {
        emit error(QNetworkReply::ProtocolFailure);
        return;
    }
    
    while (mBuffer.size() > 0)
    {
        idx = mBuffer.indexOf("\r\n");

        // The parser has encountered the end of headers.  The remainder of the
        // buffer is the body of the message.
        if (0 == idx)
        {
            mBuffer.remove(0, idx + 2);
            break;
        }

        QByteArray header = mBuffer.left(idx);
        QByteArray field;
        QByteArray value;

        int idy = header.indexOf(':');
        if (-1 == idy)
        {
            emit error(QNetworkReply::ProtocolFailure);
            return;
        }
        
        field = header.left(idy);
        value = header.mid(idy + 2);
        
        setRawHeader(field, value);
        
        if (QString::fromUtf8(field) == "Location") {
            setAttribute(QNetworkRequest::RedirectionTargetAttribute, QUrl(QString::fromUtf8(value)));
        }

        mBuffer.remove(0, idx + 2);
    }

    emit readyRead();
}

qint64 MockHttpNetworkReply::readData(char *data, qint64 maxSize)
{
    //qDebug() << "MockHttpNetworkReply::readData()";
    //qDebug() << "  maxSize: " << maxSize;
    
    if (mBuffer.size() == 0)
        return -1;

    qint64 size = maxSize;
    if (mBuffer.size() < maxSize)
        size = mBuffer.size();

    memcpy(data, mBuffer.data(), size);
    mBuffer.remove(0, size);

    return size;
}

qint64 MockHttpNetworkReply::writeData(const char *data, qint64 maxSize)
{
    Q_UNUSED(data)
    Q_UNUSED(maxSize)

    //qDebug() << "MockHttpNetworkReply::writeData()";

    return -1;
}


QTX_END_NAMESPACE
