#include "cardvector.h"

CardVector::CardVector(QFile* cardFile)
    : QObject()
{
    this->cardFile = cardFile;
    readAllCards();
    addSpacers(getRoot());
}

CardVector::CardVector() : QObject()
{

}

void CardVector::addColumn(QVector<Card*>* column)
{
    cardVector.append(column);
}

void CardVector::addCard(Card* card, int columnIndex)
{
    if (columnIndex >= cardVector.size()) {
        QVector<Card*>* newColumn = new QVector<Card*>();
        cardVector.append(newColumn);
//        QObject::connect(newColumn, &QAbstractItemModel::dataChanged, this, &CardVector::writeAllCards);
    }
    QVector<Card*>* column = cardVector.at(columnIndex); // TODO: could be optimized when adding a new model
    column->append(card);
}

void CardVector::addBodyText(Card* card, Card* parent)
{
    bodyTextVector.append(card);
    parent->addBodyText(card);
}

// TODO: Add error handling, this is dangerous
QVector<Card*>* CardVector::getColumn(int index)
{
    return cardVector.at(index);
}

Card* CardVector::getRoot()
{
    Card* root = cardVector.at(0)->at(0);
    while (root->getLevel() > 0) {
        root = root->getParent();
    }

    return root;
}

// populates the internal vector with a series of CardModels based on the given file
void CardVector::readAllCards()
{
    // validate the file
    if (!cardFile->open(QIODevice::ReadOnly | QIODevice::Text)) return; // TODO: add error handling

    // initialization
    QTextStream in(cardFile);
    Card* parent = new Card(); // the first Card is just the root
    addCard(parent, 0);
    int childLevel = 1;

    while (!in.atEnd()) {
        QString line = in.readLine();

        // create a new Card object from the current line
        Card* newCard = new Card(line);
        parent = newCard->resolveParent(parent, childLevel);

        if (newCard->getLevel() == 1000)
        {
            // this Card is just body text, so add it to that vector
            addBodyText(newCard, parent);
        } else {
            childLevel = parent->getLevel()+1;
            addCard(newCard, childLevel);
        }
    }

    cardFile->close();
}

// DFS through tree, and add spacers to fill out a grid
void CardVector::addSpacers(Card* root)
{
    // TODO: spacers are added to end of parent's child list instead of being inserted. Is that a problem?

    // we don't need to add spacers for the actual root
    if (root->getLevel() == 0) {
        for (Card* child : root->getChildList()) {
            addSpacers(child);
        }
    } else if (root->getChildList().size() < 1 && root->getLevel() < (cardVector.size() - 1)) {
        // leaf node that isn't in the last column, add fillers out to the last column
        SpacerCard* temp;
        Card* parent = root;
        do {
            temp = new SpacerCard(parent, parent->getLevel() + 1);
            addCard(temp, temp->getLevel());
            parent = temp;
        } while (temp->getLevel() < cardVector.size() - 1);
    } else if (root->getChildList().size() > 1) {
        // more than one child means we need to add spacers for all the additional children

        // first, add spacers for all subtrees
        for (Card* child : root->getChildList()) {
            addSpacers(child);
        }

        // then add spacers, starting at one because we don't need a spacer for the first child
        for (int i = 1; i < root->getChildList().size(); i++) {
            SpacerCard* temp = new SpacerCard(root->getParent(), root->getLevel(), root);
            insertSpacer(temp, root->getLevel(), root);
        }
    } else if (root->getChildList().size() == 1) {
        // only one child, so just add spacers for that child
        addSpacers(root->getLastChild());
    }
}

// sibling is the immediate sibling, i.e. the spacer goes immediately after the sibling
void CardVector::insertSpacer(SpacerCard* spacer, int columnIndex, Card* sibling) {
    // TODO: error handling for out-of-bounds errors on columnIndex, which should never occur
    QVector<Card*>* column = cardVector.at(columnIndex);
//    column->insertSpacer(spacer, sibling);
    int spacerIndex = column->indexOf(sibling) + 1;
    if (spacerIndex < column->size()) {
        column->insert(spacerIndex, spacer);
    } else {
        column->append(spacer);
    }
}

void CardVector::writeAllCards()
{
    if (!cardFile->open(QIODevice::WriteOnly | QIODevice::Text)) return; // TODO: add error handling

    QTextStream out(cardFile);

    // we start from the root card, and navigate all the child lists
    writeAllChildCards(getRoot(), &out);

    cardFile->close();
}

int CardVector::vectorSize()
{
    // use vector 1 because vector 0 ends up being empty
    return cardVector.at(1)->size();
}

int CardVector::getCardIndex(Card* card)
{
    return cardVector.at(card->getLevel())->indexOf(card);
}

void CardVector::writeAllChildCards(Card* root, QTextStream* out)
{
    // TODO: don't use hard coded type values
    if (root->getCardType() == 1) {
        // only write Card objects, ignore fillers

        // write the current root if it has anything
        if (root->getLevel() > 0)
            (*out) << root->getFileText() << "\n";

        // write all the associated body text
        QVector<Card*> bodyTextVector = root->getBodyTextList();
        for (Card* bodyText : bodyTextVector)
        {
            (*out) << bodyText->getFileText() << "\n";
        }

        // recursively navigate through all children
        for (Card* card : root->getChildList())
        {
            writeAllChildCards(card, out);
        }
    }
}





















