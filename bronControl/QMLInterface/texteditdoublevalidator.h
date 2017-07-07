#ifndef TEXTEDITDOUBLEVALIDATOR_H
#define TEXTEDITDOUBLEVALIDATOR_H

#include <QDoubleValidator>
#include <QDebug>
#include <QLocale>


class TextFieldDoubleValidator : public QDoubleValidator {
public:
    TextFieldDoubleValidator (QObject * parent = 0) : QDoubleValidator(parent) {}
    TextFieldDoubleValidator (double bottom, double top, int decimals, QObject * parent) :
    QDoubleValidator(bottom, top, decimals, parent) {}

    QValidator::State validate(QString & s, int & pos) const {
        Q_UNUSED(pos);
        if (s.isEmpty() || s.startsWith("-")) {
            // allow empty field or minus sign
            return QValidator::Intermediate;
        }
        QLocale local;
        // check length of decimal places
        QChar point = local.decimalPoint();//'.';
        if(s.indexOf(point) != -1) {
            int lengthDecimals = s.length() - s.indexOf(point) - 1;
            if (lengthDecimals > decimals()) {
                return QValidator::Invalid;
            }
        }
        // check range of value
        bool isNumber;
        double value = locale().toDouble(s, &isNumber);
        if (isNumber && bottom() <= value && value <= top()) {
            return QValidator::Acceptable;
        }
        return QValidator::Invalid;
    }

};


#endif // TEXTEDITDOUBLEVALIDATOR_H
