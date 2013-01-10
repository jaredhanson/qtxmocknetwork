#include "mocknetworkaccessmanager.h"

QTX_BEGIN_NAMESPACE


MockNetworkAccessManager::MockNetworkAccessManager(QObject *parent /* = 0 */)
    : QNetworkAccessManager(parent)
{
}

MockNetworkAccessManager::~MockNetworkAccessManager()
{
}


QTX_END_NAMESPACE
