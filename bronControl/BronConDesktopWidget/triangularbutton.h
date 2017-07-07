#ifndef TRIANGULARBUTTON_H
#define TRIANGULARBUTTON_H

#include <QObject>
#include <QPushButton>

class TriangularButton : public QPushButton{
public:
    enum Direction{
        eRight,
        eLeft,
        eUp,
        eDown
    };

    TriangularButton(Direction dir, const QString &text, QWidget *parent = nullptr);
    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

public:
    Direction direction;
};

#endif // TRIANGULARBUTTON_H
