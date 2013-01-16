#ifndef QTXMOCKNETWORK_IMOCKNETWORKACCESSMANAGERDELEGATE_H
#define QTXMOCKNETWORK_IMOCKNETWORKACCESSMANAGERDELEGATE_H

#include "mocknetworkglobal.h"
#include <QtCore>

QTX_BEGIN_NAMESPACE


class IMockNetworkAccessManagerDelegate
{
public:
    virtual ~IMockNetworkAccessManagerDelegate() {};
        
    virtual QIODevice *createIncomingData(const QNetworkRequest & req, QIODevice * outgoingData = 0) = 0;
};


QTX_END_NAMESPACE

#endif // QTXMOCKNETWORK_IMOCKNETWORKACCESSMANAGERDELEGATE_H
