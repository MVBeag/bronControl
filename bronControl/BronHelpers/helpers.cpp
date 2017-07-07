#include "helpers.h"


int extractNum(const QString &s){
    QString num;
    foreach(QChar c, s) {
        if (c.isDigit()) {
            num.append(c);
        }
    }
    return num.toInt();
}
