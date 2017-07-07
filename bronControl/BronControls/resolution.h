#ifndef RESOLUTION_H
#define RESOLUTION_H

#include <QObject>
#include <QSizeF>
#include <QRect>

namespace QMLInterface{

class Resolution : public QObject{
    Q_OBJECT
    Q_PROPERTY(qreal textScale READ textScale WRITE setTextScale NOTIFY textScaleChanged)
    Q_PROPERTY(qreal minWidthMM  READ minWidthMM  NOTIFY minWidthMMChanged)
    Q_PROPERTY(qreal minWidthDots  READ minWidthDots  NOTIFY minWidthDotsChanged)
    Q_PROPERTY(qreal logicalDpiX READ logicalDipX NOTIFY logicalDpiXChanged)
    Q_PROPERTY(qreal physicalDpiX READ physicalDpiX NOTIFY physicalDpiXChanged)
    Q_PROPERTY(qreal dotsPerMM READ dotsPerMM NOTIFY dotsPerMMChanged)

public:

    qreal textScale() const;
    qreal minWidthMM() const;
    qreal minWidthDots() const;
    qreal logicalDipX() const;
    qreal physicalDpiX() const;
    qreal dotsPerMM() const;
    void setTextScale(const qreal val);

    Resolution(QObject *parent = 0);
    /**
     * @brief dp returns the number of pixels in an device independent format. The calcualtion is
     * the reference display has 160 dots per inch
     * @param desired device independet pixels
     * @return real pixels for the screen density
     */
    Q_INVOKABLE qreal dp(qreal value);


    Q_INVOKABLE qreal sp(qreal value);
    Q_INVOKABLE void showInfo();

signals:
    void textScaleChanged(qreal val);
    void minWidthMMChanged(qreal minWidthMM);
    void minWidthDotsChanged(qreal minWidthDots);
    void logicalDpiXChanged(qreal logicalDpiX);
    void physicalDpiXChanged(qreal physicalDpiX);
    void dotsPerMMChanged(qreal dotsPerMM);


//public slots:
//    void scaleChanged(qreal dpi);
public slots:
    void onOrientationChanged(Qt::ScreenOrientation orientation);

private:
    const qreal m_dpi;
    qreal m_scaleFactor;
    qreal m_textScale;
    qreal m_logicalDpiX;
    qreal m_logicalDpiY;
    qreal m_physicalDpiX;
    qreal m_qhysicalDpiY;
    qreal m_dotsPerMM;
    QSizeF m_sizeMM;
    QRect m_avaiableGeomatry;
    Qt::ScreenOrientation m_orientation;
};

inline qreal Resolution::textScale() const{
    return m_textScale;
}

inline qreal Resolution::logicalDipX() const{
    return m_logicalDpiX;
}

inline qreal Resolution::physicalDpiX() const{
    return m_physicalDpiX;
}

inline qreal Resolution::dotsPerMM() const{
    return m_dotsPerMM;
}

inline void Resolution::setTextScale(const qreal val){
    if(val != m_textScale){
        m_textScale = val;
        emit textScaleChanged(m_textScale);
    }
}


}

#endif // RESOLUTION_H
