#pragma once
#include <QAbstractTableModel> 
#include "../OlympianDatabase.h"

class OlympianTableModel : public QAbstractTableModel, private OlympianDatabase
{
    Q_OBJECT
public:
    OlympianTableModel(OlympianDatabase& database, QObject *parent = nullptr);
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    bool insertRows(int row, int count, const QModelIndex& parent) override;
    bool removeRows(int row, int count, const QModelIndex& parent) override;

    void setSortByAge();
    void setSortByHeight();
    void setSortByName();
    Vector<Olympian*> currentDisplay;

private:

    static const int NUM_COLUMNS = 9;
};
