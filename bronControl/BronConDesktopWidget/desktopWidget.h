#ifndef DESKTOPWIDGET_H
#define DESKTOPWIDGET_H

#include <QWidget>
#include "triangularbutton.h"
#include <QBoxLayout>
#include <QLineEdit>
#include "bronconinterface.h"

class DesktopWidget : public QWidget
{
    Q_OBJECT

public:
    DesktopWidget(QApplication *app, QWidget *parent = 0);
    ~DesktopWidget();
    QSize sizeHint() const Q_DECL_OVERRIDE;

public Q_SLOTS:
    void onHigherEnergy(bool);
    void onLowerEnergy(bool);
    void onShow(bool);
    void onHide(bool);
    void onOnlineChanged(bool val);
    void onNewEnergy(float val);

protected:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    QApplication *app;
    QPoint dragPosition;
    TriangularButton *btUp;
    TriangularButton *btDown;
    QLineEdit *viewEnergy;
    TriangularButton *btShow;
    TriangularButton *btHide;
    BronConInterface *bronCon;
    double increaseValue;
    double decreaseValue;
    bool online;
};

inline void DesktopWidget::onHigherEnergy(bool){
    bronCon->send("higherEnergy", QString::number(increaseValue));
}

inline void DesktopWidget::onLowerEnergy(bool){
    bronCon->send("lowerEnergy", QString::number(decreaseValue));
}

inline void DesktopWidget::onShow(bool){
    bronCon->send("show", "true");
}

inline void DesktopWidget::onHide(bool){
    bronCon->send("hide", "true");
}

#endif
