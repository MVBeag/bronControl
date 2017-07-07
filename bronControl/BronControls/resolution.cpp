#include "resolution.h"
#include <QGuiApplication>
#include <QScreen>
#include <QList>
#include <QDebug>

qreal QMLInterface::Resolution::minWidthMM() const{
    return m_sizeMM.height() <= m_sizeMM.width() ? m_sizeMM.height() : m_sizeMM.width();
}

qreal QMLInterface::Resolution::minWidthDots() const{
    return m_avaiableGeomatry.width() <= m_avaiableGeomatry.height() ? m_avaiableGeomatry.width() : m_avaiableGeomatry.height();
}



QMLInterface::Resolution::Resolution(QObject *parent)
    : QObject(parent)
    , m_dpi(QGuiApplication::primaryScreen()->logicalDotsPerInch())
    , m_scaleFactor(m_dpi/160)
    , m_textScale(1.0)
    , m_logicalDpiX(QGuiApplication::primaryScreen()->logicalDotsPerInchX())
    , m_logicalDpiY(QGuiApplication::primaryScreen()->logicalDotsPerInchY())
    , m_physicalDpiX(QGuiApplication::primaryScreen()->physicalDotsPerInchX())
    , m_qhysicalDpiY(QGuiApplication::primaryScreen()->physicalDotsPerInchY())
    , m_dotsPerMM(m_physicalDpiX/25.4)
    , m_sizeMM(QGuiApplication::primaryScreen()->physicalSize())
    , m_avaiableGeomatry(QGuiApplication::primaryScreen()->availableGeometry())
    , m_orientation(QGuiApplication::primaryScreen()->orientation()){
    connect(QGuiApplication::primaryScreen(), &QScreen::orientationChanged, this, &Resolution::onOrientationChanged);
//    showInfo();
}


qreal QMLInterface::Resolution::dp(qreal value){
    return value * m_scaleFactor;
}


qreal QMLInterface::Resolution::sp(qreal value){
    return value /* m_scaleFactor */ * m_textScale;
}

void QMLInterface::Resolution::showInfo(){
    qDebug() << "dpi: " << m_dpi;
    qDebug() << "dpi.x " << m_logicalDpiX;
    qDebug() << "dpi.y " << m_logicalDpiY;
    qDebug() << "primary screen availableSize " << QGuiApplication::primaryScreen()->availableSize();
    qDebug() << "physical dpi " << QGuiApplication::primaryScreen()->physicalDotsPerInch();
    qDebug() << "physical dpiX " << m_physicalDpiX;
    qDebug() << "logical dpi " << QGuiApplication::primaryScreen()->logicalDotsPerInch();
    qDebug() << "available geometry " << QGuiApplication::primaryScreen()->availableGeometry();
    qDebug() << "qhysical Size " << m_sizeMM;
    qDebug() << "scale factor " << m_scaleFactor;
    qDebug() << "orientation " << m_orientation;
}

void QMLInterface::Resolution::onOrientationChanged(Qt::ScreenOrientation orientation){
    if(orientation != m_orientation){
        m_orientation = orientation;
    }
}


