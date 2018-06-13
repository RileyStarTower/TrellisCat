#ifndef CARDMODEL_H
#define CARDMODEL_H

#include <QAbstractListModel>
#include <QVariant>
#include <QModelIndex>
#include <QHash>
#include <QByteArray>
#include <QVector>
#include <QString>

#include "card.h"
#include "spacercard.h"

/**
 * @brief A single column of Cards
 *
 * This class contains a single column in the UI.
 * We then create a vector of these models, and pass
 * them to the listView objects to create the appearance
 * of navigating a single structure.
 *
 * This class subclasses QAbstractListModel to be used
 * with the Card class by using a QVector of Card objects
 */
class CardModel : public QAbstractListModel
{
    Q_OBJECT

public:
    // enum used by the data function to send data to the view
    enum CardRoles {
        TextRole = Qt::UserRole,
        TypeRole,
        ChildTypeRole,
        SiblingTypeRole,
    };

    CardModel();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role);
    QHash<int, QByteArray> roleNames() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);

    // custom methods
    bool appendCard(Card* card);
    bool insertSpacer(SpacerCard* spacer, Card* sibling);
    Card* getRoot();

private:
    QVector<Card*> cardVector;
    QString header;

};

#endif // CARDMODEL_H
