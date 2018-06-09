#include <QtDebug>

#include "cardmodel.h"

CardModel::CardModel() : QAbstractListModel() {}

// just returns the number of Cards in the vector
int CardModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return cardVector.size();
}

// calls the Card method so we can make that more complex if we need to
// I suppose we could use the role to handle that complexity, too
QVariant CardModel::data(const QModelIndex &index, int role) const
{
    if (role == TextRole) {
        return cardVector.at(index.row())->getCardText();
    } else if (role == TypeRole) {
        return cardVector.at(index.row())->getCardType();
    } else {
        return "Error";
    }
}

// there's only one role for now, so we're just calling setCardText
bool CardModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        // TODO: TESTING ONLY writeDebug("setData", "made it");

        // get the relevant Card, and set its text
        int row = index.row();
        Card* card = cardVector.at(row);
        emit dataChanged(index, index);

        // TODO: TESTING ONLY writeDebug("setData", "setting data");

        return card->setCardText(value);
    }

    else return false;
}

// we just need to return the header value
QVariant CardModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);

    // each list just has one header, so don't care about anything else, just return the header
    return header;
}

bool CardModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);

    // we only have one header, so just set the header
    header = value.toString();
    return true;
}

QHash<int, QByteArray> CardModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TextRole] = "cardText";
    roles[TypeRole] = "cardType";
    return roles;
}

// this makes sense, although I don't understand all the specifics
Qt::ItemFlags CardModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

// this adds a bunch of blank rows, it doesn't put any data in those rows
bool CardModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row+count);
    cardVector.insert(row, count, new Card());
    endInsertRows();

    return true; // TODO: we can return false if we do some validation first
}

bool CardModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row+count);
    cardVector.remove(row, count);
    endRemoveRows();

    return true; // TODO: we can return false if we do some validation first
}

// adds a Card to the model
bool CardModel::appendCard(Card *card)
{
    cardVector.append(card);

    return true;
}

bool CardModel::insertSpacer(SpacerCard *spacer, Card *sibling)
{
    int spacerIndex = cardVector.indexOf(sibling) + 1;
    if (spacerIndex < cardVector.size()) {
        cardVector.insert(spacerIndex, spacer);
    } else {
        cardVector.append(spacer);
    }
    return true;
}

Card* CardModel::getRoot()
{
    return cardVector.at(0);
}






























