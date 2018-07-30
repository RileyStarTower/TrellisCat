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
    QObject::connect(this, &QAbstractItemModel::dataChanged, cardVector2D, &CardVector::writeAllCards);
}

CardModel::CardModel(QFile *cardFile) : QAbstractListModel()
{
    // validate the file
    if (!cardFile->open(QIODevice::ReadOnly | QIODevice::Text)) return; // TODO: add error handling

    // initialization
    QTextStream in(cardFile);
    Card* parent = new Card(); // the first Card is just the root
    root = parent;
    int childLevel = 1;
    treeDepth = childLevel;

    while (!in.atEnd()) {
        QString line = in.readLine();

        Card* newCard = new Card(line);
        parent = newCard->resolveParent(parent, childLevel);

        if (newCard->getLevel() == 1000)
        {
            // this Card is just body text, so add it to that vector
            continue;
        } else {
            childLevel = parent->getLevel() + 1;
            if (childLevel > treeDepth) {
                treeDepth = childLevel;
            }
        }
    }

//    root->addSpacers();
    flattenTree(root);
}

void CardModel::flattenTree(Card* subroot)
{
    if (subroot->getLevel() > 0) {
        cardVector.append(subroot);
    }

    // pre-order traversal, and fill in spacers if where needed
    if (subroot->getChildCount() < 1) {
        addSpacers(treeDepth - subroot->getLevel(), subroot);
    } else {
        for (int i = 0; i < subroot->getChildCount(); i++) {
            if (i > 0) {
                addSpacers(subroot->getLevel(), subroot);
            }
            flattenTree(subroot->getChild(i));
        }
    }
}

void CardModel::addSpacers(int count, Card* parent)
{
    for (int i = 0; i < count; i++) {
        cardVector.append(new SpacerCard(parent));
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
    case MoveUp : return moveUp(index.row());
    case MoveDown : return moveDown(index.row());
    case MoveRight : return moveRight(index.row());
    case MoveLeft : return moveLeft(index.row());
    case GridWidth : return cardVector2D->getColumnCount() * 410; // TODO: there's a hard coded thing here...
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
    roles[MoveUp]           = "moveUp";
    roles[MoveDown]         = "moveDown";
    roles[MoveRight]        = "moveRight";
    roles[MoveLeft]         = "moveLeft";
    roles[GridWidth]        = "gridWidth";
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

// called from QML when adding a new child from the GUI
void CardModel::addChild(int index)
{
    // add a child to the Card
//    bool newColumn = (index >= cardVector2D->getColumnCount());
    Card* parent = cardVector.at(index);
    Card* child = new Card(parent->getLevel() + 1, parent);
    if (parent->hasCards()) {
        parent->insertChild(0, child);
    } else {
        // if the parent has no Cards, remove the spacer and add the new one
        if (parent->getChildCount() > 0) {
            parent->removeChild(0);
        }
        parent->addChild(child);
    }

    // update the underlying 2D vector
    cardVector2D->insertCard(child, child->getLevel(), index);
    cardVector2D->updateSpacers(child);

    // update the flattened model
    // TODO: do this directly instead of rebuilding the whole thing
    int count = cardVector2D->getColumnCount() * cardVector2D->vectorSize();
    beginRemoveRows(QModelIndex(), 0, count);
    cardVector.clear();
    endRemoveRows();

    beginInsertRows(QModelIndex(), 0, count);
    for (int i = 0; i < cardVector2D->vectorSize(); i++) {
        for (int j = 1; j < cardVector2D->getColumnCount(); j++) {
            // TODO: add a 2D index function so we don't have to get a column each time
            QVector<Card*>* column = cardVector2D->getColumn(j);
            appendCard(column->at(i));
        }
    }
    endInsertRows();

    QObject* gridView = appWindow->findChild<QObject*>("cardGrid");
    gridView->setProperty("width", (cardVector2D->getColumnCount() - 1) * 400);
}

int CardModel::moveUp(int index) const
{
    // jump up through the flat vector until we find a Card
//    int jump = cardVector2D->getColumnCount() - 1; // subtract 1 because the first column isn't used in the flat model
    int jump = treeDepth;
    int count = jump;
    while (index - count >= 0) {
        if (cardVector.at(index - count)->getCardType() == 1) {
            // if we've found a Card, return the current count
            return count * -1;
        }
        // otherwise, jump up again
        count += jump;
    }
    return 0;
}

int CardModel::moveDown(int index) const
{
    // jump down through the flat vector until we find a Card
//    int jump = cardVector2D->getColumnCount() - 1; // subtract 1 because the first column isn't used in the flat model
    int jump = treeDepth;
    int count = jump;
    while (index + count < cardVector.size()) {
        if (cardVector.at(index + count)->getCardType() == 1) {
            // if we've found a Card, return the current count
            return count;
        }
        // otherwise, jump up again
        count += jump;
    }
    return 0;
}

int CardModel::moveRight(int index) const
{
    // move to the right if there are any Cards there
    Card* start = cardVector.at(index);
    if (start->hasCards()) {
        return 1;
    } else {
        return 0;
    }
}

int CardModel::moveLeft(int index) const
{
    // if we can't move left, just return 0
    if (index == 0) { return 0; }
    // move back through the flat vector until we find the parent
    Card* start = cardVector.at(index);
    int count = 1;
    Card* temp = cardVector.at(index - count);
    while ((temp != start->getParent()) && ((index - count) >= 0)) {
        count++;
        temp = cardVector.at(index - count);
    }
    return count * -1;
}
