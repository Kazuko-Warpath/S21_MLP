QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller/controller.cc \
    main.cpp \
    model/common.cc \
    model/graph_perceptron.cc \
    model/layer.cc \
    model/matrix_perceptron.cc \
    model/neuron.cc \
    view/mainwindow.cpp \
    view/metrics.cpp \
    view/paint.cpp \
    view/qcustomplot.cpp \
    view/report.cpp \
    view/test.cpp \
    view/train.cpp \
    view/validation.cpp

HEADERS += \
    controller/controller.h \
    model/common.h \
    model/graph_perceptron.h \
    model/layer.h \
    model/matrix_perceptron.h \
    model/neuron.h \
    view/mainwindow.h \
    view/metrics.h \
    view/paint.h \
    view/qcustomplot.h \
    view/report.h \
    view/test.h \
    view/train.h \
    view/validation.h

FORMS += \
    view/mainwindow.ui \
    view/metrics.ui \
    view/report.ui \
    view/test.ui \
    view/train.ui \
    view/validation.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    view/resource/resource.qrc
