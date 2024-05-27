#include "booksmodel.h"

BooksModel::BooksModel(QObject *parent)
    : QAbstractListModel(parent){}

void BooksModel::addBook(const QString &title)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_bookTitles << title;
    endInsertRows();
}

void BooksModel::removeBook(int index)
{
    if (index < 0 || index >= m_bookTitles.size())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_bookTitles.removeAt(index);
    endRemoveRows();
}

int BooksModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_bookTitles.count();
}

QVariant BooksModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_bookTitles.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return m_bookTitles.at(index.row());

    return QVariant();
}


QString BooksModel::bookTitle(int index) const
{
    if (index < 0 || index >= m_bookTitles.size())
        return QString();

    return m_bookTitles.at(index);
}


