
#include "olympianmodel.hpp"

OlympianTableModel::OlympianTableModel(OlympianDatabase& database, QObject *parent)
    : QAbstractTableModel(parent), OlympianDatabase(database), m_sortType(SORT_TYPE::by_name)
{
    //default to sorting in alphabetical order
    setSortByName();
}

int OlympianTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return int(m_currentDisplayVec.size());
}

int OlympianTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return NUM_COLUMNS;
}

QVariant OlympianTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Sport");
        case 2:
            return tr("State");
        case 3:
            return tr("Gender");
        case 4:
            return tr("Age (years)");
        case 5:
            return tr("Height (inches)");
        case 6:
            return tr("# Bronze Medals");
        case 7:
            return tr("# Silver Medals");
        case 8:
            return tr("# Gold Medals");

        default:
            return QVariant();
        }
    }
    return QVariant();
}

QVariant OlympianTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_currentDisplayVec.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        const auto olympian = m_currentDisplayVec.at(index.row());

        if (index.column() == 0)
            return QVariant(QString::fromStdString(olympian->getName()));
        else if (index.column() == 1)
            return QVariant(QString::fromStdString(olympian->getSport()));
        else if (index.column() == 2)
            return QVariant(QString::fromStdString(olympian->getState()));
        else if (index.column() == 3)
            return QVariant(QString(olympian->getGender()));
        else if (index.column() == 4)
            return QVariant(olympian->getAge());
        else if (index.column() == 5)
            return QVariant(olympian->getHeight());
        else if (index.column() == 6)
            return QVariant(olympian->getnBronze());
        else if (index.column() == 7)
            return QVariant(olympian->getnSilver());
        else if (index.column() == 8)
            return QVariant(olympian->getnGold());
    }
    return QVariant();
}

bool OlympianTableModel::removeRows(int row, int count, const QModelIndex& parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    for (int i = row; i < row + count; ++i)
    {
        auto oly = m_currentDisplayVec[i];
        auto pair = std::pair<Olympian*, int>(oly, i);
        m_deleteStack.push(pair);
        if (!m_currentDisplayVec.remove(size_t(i)))
            return false;
    }
    endRemoveRows();
    return true;
}
void OlympianTableModel::setSortByAge()
{
    beginResetModel();
    m_currentDisplayVec.clear();
    //traverse age BST in order to reorder the data that's being displayed
    OlympianDatabase::ageBst->insertInorder(m_currentDisplayVec);
    m_sortType = SORT_TYPE::by_age;
    endResetModel();
}

void OlympianTableModel::setSortByHeight()
{
    beginResetModel();
    m_currentDisplayVec.clear();
    //traverse age BST in order to reorder the data that's being displayed
    OlympianDatabase::heightBst->insertInorder(m_currentDisplayVec);
    m_sortType = SORT_TYPE::by_height;
    endResetModel();
}

void OlympianTableModel::setSortByName()
{
    beginResetModel();
    m_currentDisplayVec.clear();
    //traverse the linked list to reorder the display data
    OlympianDatabase::alphabeticalOrderList->insertToVector(m_currentDisplayVec);
    m_sortType = SORT_TYPE::by_name;
    endResetModel();
}

void OlympianTableModel::resetModel(Vector<Olympian *> &vec)
{
    beginResetModel();
    m_currentDisplayVec = vec;
    m_sortType = SORT_TYPE::by_search;
    endResetModel();
}
Olympian* OlympianTableModel::undoDelete()
{
    if (OlympianDatabase::undoDelete())
    {
        std::pair<Olympian*, int> deletedItem;
        if (!m_deleteStack.pop(deletedItem)) return nullptr;

        int row = deletedItem.second;
        Olympian* oly = deletedItem.first;

        auto idx = createIndex(row, 0);
        beginInsertRows(idx, row, row + 1);
        m_currentDisplayVec.insertAt(oly, row);
        endInsertRows();
        emit dataChanged(idx, createIndex(row, 9));
        return oly;
    }
    return nullptr;
}
