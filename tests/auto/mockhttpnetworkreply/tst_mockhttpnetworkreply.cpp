#include "tst_mockhttpnetworkreply.h"
#include <QtNetwork>


void tst_MockHttpNetworkReply::initTestCase()
{
    qRegisterMetaType<int>("QNetworkReply::NetworkError");
}

void tst_MockHttpNetworkReply::init()
{
    mReply = 0;
    QTimer::singleShot(10000, this, SLOT(onTimeout()));
}

void tst_MockHttpNetworkReply::cleanup()
{
    if (mReply) {
        delete mReply;
        mReply = 0;
    }
}

void tst_MockHttpNetworkReply::ok()
{
    QFile file("data/200-ok.http");
    file.open(QIODevice::ReadOnly);
    
    mReply = new MockHttpNetworkReply(&file);
    connect(mReply, SIGNAL(finished()), SLOT(onFinished()));
    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(onError(QNetworkReply::NetworkError)));
    
    QSignalSpy metaDataChangedSpy(mReply, SIGNAL(metaDataChanged()));
    QSignalSpy finishedSpy(mReply, SIGNAL(finished()));
    QSignalSpy errorSpy(mReply, SIGNAL(error(QNetworkReply::NetworkError)));
    QList<QVariant> arguments;
    
    mReply->open(QIODevice::ReadOnly);
    mEventLoop.exec();
    
    QVERIFY(metaDataChangedSpy.count() == 1);
    QVERIFY(errorSpy.count() == 0);
    QVERIFY(finishedSpy.count() == 1);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 200);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString(), QString("OK"));
    QCOMPARE(mReply->attribute(QNetworkRequest::RedirectionTargetAttribute), QVariant());
    QCOMPARE(QString::fromUtf8(mReply->rawHeader("Content-Type")), QString("text/plain"));
    QCOMPARE(QString::fromUtf8(mReply->readAll()), QString("Hello World!"));
}

void tst_MockHttpNetworkReply::multipleChoices()
{
    QFile file("data/300-multiple-choices.http");
    file.open(QIODevice::ReadOnly);
    
    mReply = new MockHttpNetworkReply(&file);
    connect(mReply, SIGNAL(finished()), SLOT(onFinished()));
    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(onError(QNetworkReply::NetworkError)));
    
    QSignalSpy metaDataChangedSpy(mReply, SIGNAL(metaDataChanged()));
    QSignalSpy finishedSpy(mReply, SIGNAL(finished()));
    QSignalSpy errorSpy(mReply, SIGNAL(error(QNetworkReply::NetworkError)));
    QList<QVariant> arguments;
    
    mReply->open(QIODevice::ReadOnly);
    mEventLoop.exec();
    
    QVERIFY(metaDataChangedSpy.count() == 1);
    QVERIFY(errorSpy.count() == 0);
    QVERIFY(finishedSpy.count() == 1);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 300);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString(), QString("Multiple Choices"));
    QCOMPARE(mReply->attribute(QNetworkRequest::RedirectionTargetAttribute), QVariant());
    QCOMPARE(QString::fromUtf8(mReply->rawHeader("Content-Type")), QString("application/json"));
    QCOMPARE(QString::fromUtf8(mReply->readAll()), QString("{ \"redirect\": true }"));
}

void tst_MockHttpNetworkReply::found()
{
    QFile file("data/302-found.http");
    file.open(QIODevice::ReadOnly);
    
    mReply = new MockHttpNetworkReply(&file);
    connect(mReply, SIGNAL(finished()), SLOT(onFinished()));
    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(onError(QNetworkReply::NetworkError)));
    
    QSignalSpy metaDataChangedSpy(mReply, SIGNAL(metaDataChanged()));
    QSignalSpy finishedSpy(mReply, SIGNAL(finished()));
    QSignalSpy errorSpy(mReply, SIGNAL(error(QNetworkReply::NetworkError)));
    QList<QVariant> arguments;
    
    mReply->open(QIODevice::ReadOnly);
    mEventLoop.exec();
    
    QVERIFY(metaDataChangedSpy.count() == 1);
    QVERIFY(errorSpy.count() == 0);
    QVERIFY(finishedSpy.count() == 1);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 302);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString(), QString("Found"));
    QCOMPARE(mReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl(), QUrl("http://www.example.com/goto.html"));
    QCOMPARE(QString::fromUtf8(mReply->rawHeader("Content-Type")), QString("application/json"));
    QCOMPARE(QString::fromUtf8(mReply->readAll()), QString("{ \"redirect\": true }"));
}

void tst_MockHttpNetworkReply::badRequest()
{
    QFile file("data/400-bad-request.http");
    file.open(QIODevice::ReadOnly);
    
    mReply = new MockHttpNetworkReply(&file);
    connect(mReply, SIGNAL(finished()), SLOT(onFinished()));
    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(onError(QNetworkReply::NetworkError)));
    
    QSignalSpy metaDataChangedSpy(mReply, SIGNAL(metaDataChanged()));
    QSignalSpy finishedSpy(mReply, SIGNAL(finished()));
    QSignalSpy errorSpy(mReply, SIGNAL(error(QNetworkReply::NetworkError)));
    QList<QVariant> arguments;
    
    mReply->open(QIODevice::ReadOnly);
    mEventLoop.exec();
    
    QVERIFY(metaDataChangedSpy.count() == 1);
    QVERIFY(errorSpy.count() == 1);
    arguments = errorSpy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == QNetworkReply::UnknownContentError);
    QVERIFY(finishedSpy.count() == 1);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 400);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString(), QString("Bad Request"));
    QCOMPARE(QString::fromUtf8(mReply->rawHeader("Content-Type")), QString("application/json"));
    QCOMPARE(QString::fromUtf8(mReply->readAll()), QString("{ \"error\": \"something went wrong\" }"));
}

void tst_MockHttpNetworkReply::unauthorized()
{
    QFile file("data/401-unauthorized.http");
    file.open(QIODevice::ReadOnly);
    
    mReply = new MockHttpNetworkReply(&file);
    connect(mReply, SIGNAL(finished()), SLOT(onFinished()));
    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(onError(QNetworkReply::NetworkError)));
    
    QSignalSpy metaDataChangedSpy(mReply, SIGNAL(metaDataChanged()));
    QSignalSpy finishedSpy(mReply, SIGNAL(finished()));
    QSignalSpy errorSpy(mReply, SIGNAL(error(QNetworkReply::NetworkError)));
    QList<QVariant> arguments;
    
    mReply->open(QIODevice::ReadOnly);
    mEventLoop.exec();
    
    QVERIFY(metaDataChangedSpy.count() == 1);
    QVERIFY(errorSpy.count() == 1);
    arguments = errorSpy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == QNetworkReply::AuthenticationRequiredError);
    QVERIFY(finishedSpy.count() == 1);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 401);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString(), QString("Unauthorized"));
    QCOMPARE(QString::fromUtf8(mReply->rawHeader("Content-Type")), QString("application/json"));
    QCOMPARE(QString::fromUtf8(mReply->readAll()), QString("{ \"error\": \"something went wrong\" }"));
}

void tst_MockHttpNetworkReply::forbidden()
{
    QFile file("data/403-forbidden.http");
    file.open(QIODevice::ReadOnly);
    
    mReply = new MockHttpNetworkReply(&file);
    connect(mReply, SIGNAL(finished()), SLOT(onFinished()));
    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(onError(QNetworkReply::NetworkError)));
    
    QSignalSpy metaDataChangedSpy(mReply, SIGNAL(metaDataChanged()));
    QSignalSpy finishedSpy(mReply, SIGNAL(finished()));
    QSignalSpy errorSpy(mReply, SIGNAL(error(QNetworkReply::NetworkError)));
    QList<QVariant> arguments;
    
    mReply->open(QIODevice::ReadOnly);
    mEventLoop.exec();
    
    QVERIFY(metaDataChangedSpy.count() == 1);
    QVERIFY(errorSpy.count() == 1);
    arguments = errorSpy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == QNetworkReply::ContentOperationNotPermittedError);
    QVERIFY(finishedSpy.count() == 1);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 403);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString(), QString("Forbidden"));
    QCOMPARE(QString::fromUtf8(mReply->rawHeader("Content-Type")), QString("application/json"));
    QCOMPARE(QString::fromUtf8(mReply->readAll()), QString("{ \"error\": \"something went wrong\" }"));
}

void tst_MockHttpNetworkReply::notFound()
{
    QFile file("data/404-not-found.http");
    file.open(QIODevice::ReadOnly);
    
    mReply = new MockHttpNetworkReply(&file);
    connect(mReply, SIGNAL(finished()), SLOT(onFinished()));
    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(onError(QNetworkReply::NetworkError)));
    
    QSignalSpy metaDataChangedSpy(mReply, SIGNAL(metaDataChanged()));
    QSignalSpy finishedSpy(mReply, SIGNAL(finished()));
    QSignalSpy errorSpy(mReply, SIGNAL(error(QNetworkReply::NetworkError)));
    QList<QVariant> arguments;
    
    mReply->open(QIODevice::ReadOnly);
    mEventLoop.exec();
    
    QVERIFY(metaDataChangedSpy.count() == 1);
    QVERIFY(errorSpy.count() == 1);
    arguments = errorSpy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == QNetworkReply::ContentNotFoundError);
    QVERIFY(finishedSpy.count() == 1);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 404);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString(), QString("Not Found"));
    QCOMPARE(QString::fromUtf8(mReply->rawHeader("Content-Type")), QString("application/json"));
    QCOMPARE(QString::fromUtf8(mReply->readAll()), QString("{ \"error\": \"something went wrong\" }"));
}

void tst_MockHttpNetworkReply::methodNotAllowed()
{
    QFile file("data/405-method-not-allowed.http");
    file.open(QIODevice::ReadOnly);
    
    mReply = new MockHttpNetworkReply(&file);
    connect(mReply, SIGNAL(finished()), SLOT(onFinished()));
    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(onError(QNetworkReply::NetworkError)));
    
    QSignalSpy metaDataChangedSpy(mReply, SIGNAL(metaDataChanged()));
    QSignalSpy finishedSpy(mReply, SIGNAL(finished()));
    QSignalSpy errorSpy(mReply, SIGNAL(error(QNetworkReply::NetworkError)));
    QList<QVariant> arguments;
    
    mReply->open(QIODevice::ReadOnly);
    mEventLoop.exec();
    
    QVERIFY(metaDataChangedSpy.count() == 1);
    QVERIFY(errorSpy.count() == 1);
    arguments = errorSpy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == QNetworkReply::ContentOperationNotPermittedError);
    QVERIFY(finishedSpy.count() == 1);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 405);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString(), QString("Method Not Allowed"));
    QCOMPARE(QString::fromUtf8(mReply->rawHeader("Content-Type")), QString("application/json"));
    QCOMPARE(QString::fromUtf8(mReply->readAll()), QString("{ \"error\": \"something went wrong\" }"));
}

void tst_MockHttpNetworkReply::proxyAuthenticationRequired()
{
    QFile file("data/407-proxy-authentication-required.http");
    file.open(QIODevice::ReadOnly);
    
    mReply = new MockHttpNetworkReply(&file);
    connect(mReply, SIGNAL(finished()), SLOT(onFinished()));
    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(onError(QNetworkReply::NetworkError)));
    
    QSignalSpy metaDataChangedSpy(mReply, SIGNAL(metaDataChanged()));
    QSignalSpy finishedSpy(mReply, SIGNAL(finished()));
    QSignalSpy errorSpy(mReply, SIGNAL(error(QNetworkReply::NetworkError)));
    QList<QVariant> arguments;
    
    mReply->open(QIODevice::ReadOnly);
    mEventLoop.exec();
    
    QVERIFY(metaDataChangedSpy.count() == 1);
    QVERIFY(errorSpy.count() == 1);
    arguments = errorSpy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == QNetworkReply::ProxyAuthenticationRequiredError);
    QVERIFY(finishedSpy.count() == 1);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 407);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString(), QString("Proxy Authentication Required"));
    QCOMPARE(QString::fromUtf8(mReply->rawHeader("Content-Type")), QString("application/json"));
    QCOMPARE(QString::fromUtf8(mReply->readAll()), QString("{ \"error\": \"something went wrong\" }"));
}

void tst_MockHttpNetworkReply::internalServerError()
{
    QFile file("data/500-internal-server-error.http");
    file.open(QIODevice::ReadOnly);
    
    mReply = new MockHttpNetworkReply(&file);
    connect(mReply, SIGNAL(finished()), SLOT(onFinished()));
    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(onError(QNetworkReply::NetworkError)));
    
    QSignalSpy metaDataChangedSpy(mReply, SIGNAL(metaDataChanged()));
    QSignalSpy finishedSpy(mReply, SIGNAL(finished()));
    QSignalSpy errorSpy(mReply, SIGNAL(error(QNetworkReply::NetworkError)));
    QList<QVariant> arguments;
    
    mReply->open(QIODevice::ReadOnly);
    mEventLoop.exec();
    
    QVERIFY(metaDataChangedSpy.count() == 1);
    QVERIFY(errorSpy.count() == 1);
    arguments = errorSpy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == QNetworkReply::UnknownContentError);
    QVERIFY(finishedSpy.count() == 1);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 500);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString(), QString("Internal Server Error"));
    QCOMPARE(QString::fromUtf8(mReply->rawHeader("Content-Type")), QString("application/json"));
    QCOMPARE(QString::fromUtf8(mReply->readAll()), QString("{ \"error\": \"something went wrong\" }"));
}

void tst_MockHttpNetworkReply::notImplemented()
{
    QFile file("data/501-not-implemented.http");
    file.open(QIODevice::ReadOnly);
    
    mReply = new MockHttpNetworkReply(&file);
    connect(mReply, SIGNAL(finished()), SLOT(onFinished()));
    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(onError(QNetworkReply::NetworkError)));
    
    QSignalSpy metaDataChangedSpy(mReply, SIGNAL(metaDataChanged()));
    QSignalSpy finishedSpy(mReply, SIGNAL(finished()));
    QSignalSpy errorSpy(mReply, SIGNAL(error(QNetworkReply::NetworkError)));
    QList<QVariant> arguments;
    
    mReply->open(QIODevice::ReadOnly);
    mEventLoop.exec();
    
    QVERIFY(metaDataChangedSpy.count() == 1);
    QVERIFY(errorSpy.count() == 1);
    arguments = errorSpy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == QNetworkReply::ProtocolUnknownError);
    QVERIFY(finishedSpy.count() == 1);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 501);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString(), QString("Not Implemented"));
    QCOMPARE(QString::fromUtf8(mReply->rawHeader("Content-Type")), QString("application/json"));
    QCOMPARE(QString::fromUtf8(mReply->readAll()), QString("{ \"error\": \"something went wrong\" }"));
}

void tst_MockHttpNetworkReply::invalidProtocol()
{
    QFile file("data/invalid.fuzz");
    file.open(QIODevice::ReadOnly);
    
    mReply = new MockHttpNetworkReply(&file);
    connect(mReply, SIGNAL(finished()), SLOT(onFinished()));
    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(onError(QNetworkReply::NetworkError)));
    
    QSignalSpy metaDataChangedSpy(mReply, SIGNAL(metaDataChanged()));
    QSignalSpy finishedSpy(mReply, SIGNAL(finished()));
    QSignalSpy errorSpy(mReply, SIGNAL(error(QNetworkReply::NetworkError)));
    QList<QVariant> arguments;
    
    mReply->open(QIODevice::ReadOnly);
    mEventLoop.exec();
    
    QVERIFY(metaDataChangedSpy.count() == 0);
    QVERIFY(errorSpy.count() == 1);
    arguments = errorSpy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == QNetworkReply::ProtocolFailure);
    QVERIFY(finishedSpy.count() == 1);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute), QVariant());
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute), QVariant());
}

void tst_MockHttpNetworkReply::connectionRefused()
{
    mReply = new MockHttpNetworkReply(0);
    connect(mReply, SIGNAL(finished()), SLOT(onFinished()));
    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(onError(QNetworkReply::NetworkError)));
    
    QSignalSpy metaDataChangedSpy(mReply, SIGNAL(metaDataChanged()));
    QSignalSpy finishedSpy(mReply, SIGNAL(finished()));
    QSignalSpy errorSpy(mReply, SIGNAL(error(QNetworkReply::NetworkError)));
    QList<QVariant> arguments;
    
    mReply->open(QIODevice::ReadOnly);
    mEventLoop.exec();
    
    QVERIFY(metaDataChangedSpy.count() == 0);
    QVERIFY(errorSpy.count() == 1);
    arguments = errorSpy.takeFirst();
    QVERIFY(arguments.at(0).toInt() == QNetworkReply::ConnectionRefusedError);
    QVERIFY(finishedSpy.count() == 1);
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute), QVariant());
    QCOMPARE(mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute), QVariant());
}

void tst_MockHttpNetworkReply::onFinished()
{
    //qDebug() << "tst_MockHttpNetworkReply::onFinished";

    mEventLoop.quit();
}

void tst_MockHttpNetworkReply::onError(QNetworkReply::NetworkError code)
{
    //qDebug() << "tst_MockHttpNetworkReply::onError";
    //qDebug() << "  code: " << code;

    Q_UNUSED(code)
}

void tst_MockHttpNetworkReply::onTimeout()
{
    mEventLoop.quit();
}

QTEST_MAIN(tst_MockHttpNetworkReply)
