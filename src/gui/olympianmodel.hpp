#pragma once
#include <QAbstractTableModel> 
#include "../OlympicDatabase.h"

class OlympianTableModel : public QAbstractTableModel, private OlympicDatabase
{
    Q_OBJECT
public:
    OlympianTableModel(OlympicDatabase& database, QObject *parent = nullptr);
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    //bool insertRows(int row, int count, const QModelIndex& parent) override;
    bool removeRows(int row, int count, const QModelIndex& parent) override;

    enum class SORT_TYPE
    {
        by_name,
        by_age,
        by_height,
        by_search
    };
    void resetModel(Vector<Olympian*>& vec);
    Olympian* undoDelete();
    void setSortByAge();
    void setSortByHeight();
    void setSortByName();

private:
    Vector<Olympian*> m_currentDisplayVec;
    Stack<std::pair<Olympian*, int>> m_deleteStack;
    SORT_TYPE m_sortType;
    static const int NUM_COLUMNS = 9;
};
