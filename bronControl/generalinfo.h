/**
  * @abstract generalInfo contains main info for this application
  *         * Version
  * it is exposed to QML to have this only once in the application
  */

#ifndef GENERALINFO_H
#define GENERALINFO_H

#include <QObject>
#include <QString>

// version history producition version
// V2.0 x       development releases
// V2.0 Beta    change some classes to smart pointer to prevent memory leaks, Enterprize Mode, Command Sequences
// V2.0 beta    change to smart pointers to prevent memory leaks, some minor changes at the gui
// V2.0 RC 1    revision changed withing the network discovery to autodetect enterprize mode devices,
//              changed at the gui:
//                  * lamp address now visible in the lamp and group symbol
//                  * application width now adjustable
//                  * focus changes to next element on Enter Key
// V2.0 RC 2    some minor changes in gui
// V2.0         changed page indicator to ScrollBar in studioCtrls
//              added small triangle in More Button
//              correct reset behaviour for special applications

namespace Framework{

class GeneralInfo : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString version READ getVersion NOTIFY versionChanged)
public:
    explicit GeneralInfo(QObject *parent = 0)
        : QObject(parent){}

    GeneralInfo(const GeneralInfo&inf)
        : QObject(inf.parent()){}
    /**
     * @brief getVersion
     * @return returns the version in a human readable form
     */
    QString getVersion() const { return "V2.0";}
    static QString sVersion() {return "V2.0";}

signals:
    void versionChanged(const QString version);

};
}

Q_DECLARE_METATYPE(Framework::GeneralInfo)

#endif // GENERALINFO_H
