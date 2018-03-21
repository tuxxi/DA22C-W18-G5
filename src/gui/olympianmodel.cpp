
#include "olympianmodel.hpp"

OlympianTableModel::OlympianTableModel(OlympianDatabase& database, QObject *parent)
    : QAbstractTableModel(parent), OlympianDatabase(database)
{

    //default to sorting in alphabetical order
    setSortByName();
}

int OlympianTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return int(currentDisplay.size());
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

    if (index.row() >= OlympianDatabase::getnRecords() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        const auto olympian = currentDisplay.at(index.row());

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

bool OlympianTableModel::insertRows(int row, int count, const QModelIndex& parent)
{
    return false;
}

bool OlympianTableModel::removeRows(int row, int count, const QModelIndex& parent)
{
    return false;
}
void OlympianTableModel::setSortByAge()
{
    beginResetModel();
    currentDisplay.clear();
    //traverse age BST in order to reorder the data that's being displayed
    OlympianDatabase::ageBst->insertInorder(currentDisplay);
    endResetModel();
}

void OlympianTableModel::setSortByHeight()
{
    beginResetModel();
    currentDisplay.clear();
    //traverse age BST in order to reorder the data that's being displayed
    OlympianDatabase::heightBst->insertInorder(currentDisplay);
    endResetModel();
}

void OlympianTableModel::setSortByName()
{
    beginResetModel();
    currentDisplay.clear();
    OlympianDatabase::alphabeticalOrderList->insertToVector(currentDisplay);
    endResetModel();
}
void OlympianTableModel::resetModel(Vector<Olympian *> &vec)
{
    beginResetModel();
    currentDisplay = vec;
    endResetModel();
}
