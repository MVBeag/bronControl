#include <QtWidgets>
#include "bronglobals.h"
#include "desktopWidget.h"

DesktopWidget::DesktopWidget(QApplication *papp, QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint)
    , app(papp)
    , btUp(nullptr)
    , btDown(nullptr)
    , viewEnergy(nullptr)
    , btShow(nullptr)
    , btHide(nullptr)
    , increaseValue(0.1)
    , decreaseValue(0.1)
    , online(false){

    bronCon = BronConInterface::getInstance(false);
    QAction *quitAction = new QAction(tr("E&xit"), this);
    quitAction->setShortcut(tr("Ctrl+Q"));
    connect(quitAction, SIGNAL(triggered()), app, SLOT(quit()));
    addAction(quitAction);

    int width = sizeHint().width()/3;
    int height = sizeHint().height()/3;

    btUp = new TriangularButton(TriangularButton::eUp,"&+", this);
    btUp->setGeometry(width, 0, width, height);
    connect(btUp, &QPushButton::clicked, this, &DesktopWidget::onHigherEnergy);

    viewEnergy = new QLineEdit("OFF", this);
    viewEnergy->setGeometry(height, height, height, height);
    viewEnergy->setReadOnly(true);
    connect(bronCon, &BronConInterface::newEnergy, this, &DesktopWidget::onNewEnergy);
    connect(bronCon, &BronConInterface::onlineChanged, this, &DesktopWidget::onOnlineChanged);

    btDown = new TriangularButton(TriangularButton::eDown,"&-", this);
    btDown->setGeometry(width, height*2, width, height);
    connect(btDown, &QPushButton::clicked, this, &DesktopWidget::onLowerEnergy);

    btShow = new TriangularButton(TriangularButton::eLeft,"", this);
    btShow->setGeometry(width * 2, height, height, width);
    connect(btShow, &QPushButton::clicked, this, &DesktopWidget::onShow);


    btHide = new TriangularButton(TriangularButton::eRight,"", this);
    btHide->setGeometry(0, width, height, width);
    connect(btHide, &QPushButton::clicked, this, &DesktopWidget::onHide);


    setContextMenuPolicy(Qt::ActionsContextMenu);
    setToolTip(tr("Drag the Widget with the left mouse button.\n"
                  "Use the right mouse button to open a context menu."));
    setWindowTitle(tr("BronCon Remote"));
}

DesktopWidget::~DesktopWidget(){
    BronConInterface::destroy();
}


void DesktopWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void DesktopWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void DesktopWidget::resizeEvent(QResizeEvent * /* event */)
{
    int side = qMin(width(), height());
    QRegion maskedRegion(width() / 2 - side / 2, height() / 2 - side / 2, side,
                         side, QRegion::Ellipse);
    setMask(maskedRegion);
}


QSize DesktopWidget::sizeHint() const
{
    return QSize(100, 100);
}

void DesktopWidget::onOnlineChanged(bool val){
    if(val != online){
        online = val;
        if(!online){
            viewEnergy->setText("OFF");
        }
    }
}

void DesktopWidget::onNewEnergy(float val){
    if(online){
        viewEnergy->setText(QString::number(val));
    }
}

