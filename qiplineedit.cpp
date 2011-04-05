#include "qiplineedit.h"

QIPLineEdit::QIPLineEdit(QWidget *parent) :
    QLineEdit(parent)
{

    QRegExp rx("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    validator = new QRegExpValidator(rx,this);
    setValidator(validator);
    setInputMask("000.000.000.000;0");

}

QIPLineEdit::~QIPLineEdit()
{
    delete validator;
}
