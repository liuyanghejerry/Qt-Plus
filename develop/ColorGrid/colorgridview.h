#ifndef COLORGRIDVIEW_H
#define COLORGRIDVIEW_H

#include <QAbstractItemView>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include <QScrollBar>
#include <QMenu>
#include <QDebug>

#include "colorwheel.h"

class ColorGridView : public QAbstractItemView
{
    Q_OBJECT
public:
    explicit ColorGridView(QWidget *parent = 0);
    // ColorGridView will NOT become parent of ColorWheel
    void setColorWheel(ColorWheel *wheel);

    virtual QSize sizeHint () const;
    virtual QSize minimumSizeHint () const;
    virtual QRect visualRect(const QModelIndex&index) const;
    virtual void scrollTo(const QModelIndex&, QAbstractItemView::ScrollHint);
    virtual QModelIndex indexAt(const QPoint&p) const;
    virtual QModelIndex moveCursor(QAbstractItemView::CursorAction, Qt::KeyboardModifiers);
    virtual int horizontalOffset() const;
    virtual int verticalOffset() const;
    virtual bool isIndexHidden(const QModelIndex &index) const;
    virtual void setSelection(const QRect&, QFlags<QItemSelectionModel::SelectionFlag>);
    virtual QRegion visualRegionForSelection(const QItemSelection&) const;
    
signals:
    void color(QColor);
    
public slots:
    void setColor(int row, int column, const QColor &color);
    void colorPicked();
    void colorCleared();
    void colorDroped();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
private:
    QMenu menu;
    ColorWheel *colorWheel;
    int margin;
    int gridMargin;
    int gridWidth;
    int gridCount;
    QSize gridsSize;
    QImage backgroundImg;
    void initGridBackgroundImg();
    void initContextMenu();
private slots:
};

#endif // COLORGRIDVIEW_H
