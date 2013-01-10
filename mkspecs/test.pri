include(features/qtx_testcase.prf)


QT += network

INCLUDEPATH += ../../../src/mocknetwork

LIBS += -L../../../lib

LIBS += -lQtxMockNetwork
