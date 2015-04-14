#ifndef _Golographic_Model_H
#define _Golographic_Model_H

#include <QVector>
#include <QAbstractTableModel>

#include "gologramma.h"
#include "src_config.h"

class _FFTMATR_EXPORT_ GolographicModel : public QAbstractTableModel
{
public:
    GolographicModel (const QVector<golographicData>& imData, QObject * parent=0);
    virtual ~GolographicModel (void);

    virtual int columnCount (const QModelIndex& parent=QModelIndex()) const;
    virtual int rowCount (const QModelIndex& parent=QModelIndex()) const;

    virtual QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:
    //
    // Variables
    //
    QVector<golographicData> gData;

private:
    Q_OBJECT
};

#endif
