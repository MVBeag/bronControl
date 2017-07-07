#ifndef APPCONTROL_H
#define APPCONTROL_H

#include <QObject>
#ifndef Q_OS_IOS
#include <QProcess>
#endif
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>

class AppControl : public QObject{
    Q_OBJECT
public:
    static AppControl *getInstance(QObject *parent = nullptr);

    ~AppControl(){}

    /**
     * @brief eventFilter is used to destroy some singeltons to an earlier time
     * @param object
     * @param event
     * @return
     */
    bool eventFilter(QObject *object, QEvent *event) override;

signals:
    void showCogni();
private:
    AppControl(QObject *app);
};




#endif // APPCONTROL_H
