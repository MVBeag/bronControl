#include "triangularbutton.h"
#include <QPoint>
#include <QVector>

TriangularButton::TriangularButton(Direction dir, const QString &text, QWidget *parent)
    : QPushButton (text, parent)
    , direction(dir){
}

QSize TriangularButton::sizeHint() const{
     return QSize(33, 33);
}

void TriangularButton::resizeEvent(QResizeEvent * /* event */){
    int side = qMin(width(), height());


    // polygon for left pointing
    static QVector<QPoint> vectleft(std::initializer_list<QPoint>{
                                         {0, 0},
                                         {0, side},
                                         {side, side/2}
                                    });

    static QVector<QPoint> vectright(std::initializer_list<QPoint>{
                                         {0,side/2},
                                         {side, 0},
                                         {side, side}
                                     });

    static QVector<QPoint> vectup(std::initializer_list<QPoint>{
                                         {side/2,0},
                                         {side, side},
                                         {0, side}
                                     });

    static QVector<QPoint> vectdown(std::initializer_list<QPoint>{
                                         {0,0},
                                         {side, 0},
                                         {side/2, side}
                                     });

    QVector<QPoint> *pVect = &vectleft;

    switch(direction){
    case eUp:       pVect = &vectup; break;
    case eDown:     pVect = &vectdown; break;
    case eLeft:  break;
    case eRight:    pVect = &vectright; break;
    }

    QPolygon triangle(*pVect);
    QRegion maskedRegion(triangle);
    setMask(maskedRegion);
}
