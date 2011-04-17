#ifndef QIMAGEBUTTON_H
#define QIMAGEBUTTON_H

#include <QAbstractButton>
#include <QPaintEvent>
#include <QPainter>

class QImageButton : public QAbstractButton
{
    Q_OBJECT
public:
    QImageButton(const QString &normal, const QString &hovered, const QString &pressed, const QString &checked ="");
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    QSize maximumSize() const;

    void changePics(const QString &normal, const QString &hovered, const QString &pressed, const QString &checked = "");
protected:
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
private:
    QPixmap myPic, myHoveredPic, myCheckedPic, myPressedPic;
    int lastUnderMouse; // last mouse pos recorded
    bool bpressed;

    enum State {
        Normal,
        Hovered,
        Checked,
        Pressed
    };
    int lastState;
};

#endif // QIMAGEBUTTON_H
