#ifndef TST_MOCKHTTPNETWORKREPLY_H
#define TST_MOCKHTTPNETWORKREPLY_H

#include "mockhttpnetworkreply.h"
#include <QtTest/QtTest>

QTX_USE_NAMESPACE


class tst_MockHttpNetworkReply : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();
    
    void ok();
    void multipleChoices();
    void found();
    void internalServerError();
    void notImplemented();
    
    void invalidProtocol();
    
private:
    QEventLoop mEventLoop;
    MockHttpNetworkReply *mReply;
    
public slots:
    void onFinished();
    void onError(QNetworkReply::NetworkError code);
    void onTimeout();
};

#endif // TST_MOCKHTTPNETWORKREPLY_H
