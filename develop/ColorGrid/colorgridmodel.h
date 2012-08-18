#ifndef COLORGRIDMODEL_H
#define COLORGRIDMODEL_H

#include <QAbstractTableModel>
#include <QColor>
#include <QList>
#include <QVariant>

class ColorGridModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ColorGridModel(QObject *parent = 0);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
signals:
    
public slots:
protected:
private:
    QList< QList<QColor> > colors;
    int capacity;
};

#endif // COLORGRIDMODEL_H
