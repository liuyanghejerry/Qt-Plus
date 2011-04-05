#ifndef QIPLINEEDIT_H
#define QIPLINEEDIT_H

#include <QLineEdit>
#include <QRegExpValidator>

class QIPLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    QIPLineEdit(QWidget *parent = 0);
    ~QIPLineEdit();
private:
    QRegExpValidator * validator;
};

#endif // QIPLINEEDIT_H
