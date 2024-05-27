// booksmodel.h

#ifndef BOOKSMODEL_H
#define BOOKSMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class BooksModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit BooksModel(QObject *parent = nullptr);

    Q_INVOKABLE void addBook(const QString &title);
    Q_INVOKABLE void removeBook(int index);
    Q_INVOKABLE QString bookTitle(int index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

signals:
    void itemClicked(int index);

private:
    QStringList m_bookTitles;
};

#endif // BOOKSMODEL_H
