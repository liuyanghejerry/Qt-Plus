#ifndef QIRCLINEEDIT_H
#define QIRCLINEEDIT_H

#include <QLineEdit>
#include <QList>
#include <QKeyEvent>
class QIRCLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    QIRCLineEdit();
private:
    void keyPressEvent(QKeyEvent *);
    QList<QString> m_Inputlist;
    quint16 listindex;
public slots:
    void myTextEdited();
    void myclear();
};

#endif // QIRCLINEEDIT_H
