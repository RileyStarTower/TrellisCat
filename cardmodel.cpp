#include <QtDebug>

#include "cardmodel.h"

CardModel::CardModel() : QAbstractListModel() {}

CardModel::CardModel(CardVector* cardVector2D) : QAbstractListModel()
{
    this->cardVector2D = cardVector2D;
    for (int i = 0; i < cardVector2D->vectorSize(); i++) {
        for (int j = 1; j < cardVector2D->getColumnCount(); j++) {
            QVector<Card*>* column = cardVector2D->getColumn(j);
            appendCard(column->at(i));
        }
    }
}

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
    // TODO: make this a switch
    switch(role) {
    case TextRole : return cardVector.at(index.row())->getCardText();
    case TypeRole : return cardVector.at(index.row())->getCardType();
    case ChildTypeRole : return cardVector.at(index.row())->getChildType();
    case SiblingTypeRole : return cardVector.at(index.row())->getSiblingType();
    case ChildCount : return cardVector.at(index.row())->getChildCount();
    case ColumnCount : return 3; // TODO: don't hard code it
    case BacktabSearch : return backtabSearch(cardVector.at(index.row()));
    case PrevSiblings: {
        // if it's in the top row, don't move
        if (index.row() < 3) return 0; // TODO: don't hard code the column count
        // otherwise, calculate how many spacers we need to move over
        else return cardVector.at(index.row())->getPrevSiblingCount();
    }
    default : return "Error";
    }
}

// there's only one role for now, so we're just calling setCardText
bool CardModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        // get the relevant Card, and set its text
        int row = index.row();
        Card* card = cardVector.at(row);
        bool ret = card->setCardText(value);
//        todo: write all cards
        emit dataChanged(index, index);

        return ret;
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
    roles[Qt::EditRole]     = "editedData";
    roles[TextRole]         = "cardText";
    roles[TypeRole]         = "cardType";
    roles[ChildTypeRole]    = "childType";
    roles[SiblingTypeRole]  = "siblingType";
    roles[ChildCount]       = "childCount";
    roles[ColumnCount]      = "columnCount";
    roles[PrevSiblings]     = "prevSiblings";
    roles[BacktabSearch]    = "backtabSearch";
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

Card* CardModel::getRoot()
{
    return cardVector.at(0);
}

int CardModel::size()
{
    return cardVector.size();
}

Card* CardModel::at(int index)
{
    return cardVector.at(index);
}

int CardModel::backtabSearch(Card* start) const
{
    if (start->getLevel() == 1) {
        // can't go farther left than this
        return 0;
    }
    // find the index of the parent, take the difference in indices
    int startIndex = cardVector.indexOf(start);
    int parentIndex = cardVector.indexOf(start->getParent());
    return startIndex - parentIndex;
}






























