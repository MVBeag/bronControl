#ifndef TEXTEDITINTVALIDATOR_H
#define TEXTEDITINTVALIDATOR_H

#include <QIntValidator>
#include <QDebug>

class TextFieldIntValidator : public QIntValidator {
public:
    TextFieldIntValidator (QObject * parent = 0)
        : QIntValidator(parent) {}
    TextFieldIntValidator (int bottom, int top, QObject * parent)
        : QIntValidator(bottom, top, parent) {}

    QValidator::State validate(QString & s, int & pos) const {
        qDebug() << "int validate " << s << " pos " << pos;
        Q_UNUSED(pos);
        QValidator::State ret = QIntValidator::validate(s, pos);
        qDebug() << "int validate return " << ret;
        return ret;
    }

};


#endif // TEXTEDITINTVALIDATOR_H
