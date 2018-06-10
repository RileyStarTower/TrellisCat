#include <QtDebug>

#include "cardmodelvector.h"

CardModelVector::CardModelVector(QFile* cardFile)
    : QObject()
{
    this->cardFile = cardFile;
    readAllCards();
    addSpacers(getRoot());
}

void CardModelVector::addModel(CardModel model)
{
    cardModelVector.append(&model);
}

void CardModelVector::addCard(Card* card, int modelIndex)
{
    if (modelIndex >= cardModelVector.size()) {
        CardModel* newModel = new CardModel();
        cardModelVector.append(newModel);
        QObject::connect(newModel, &QAbstractItemModel::dataChanged, this, &CardModelVector::writeAllCards);
    }
    CardModel* model = cardModelVector.at(modelIndex); // TODO: could be optimized when adding a new model
    model->appendCard(card);
}

void CardModelVector::addBodyText(Card* card, Card* parent)
{
    bodyTextVector.append(card);
    parent->addBodyText(card);
}

// TODO: Add error handling, this is dangerous
CardModel* CardModelVector::getCardModel(int index)
{
    return cardModelVector.at(index);
}

void CardModelVector::connectModels(QQmlContext* context)
{
    int i = 0;
    for (CardModel* model : cardModelVector) {
        context->setContextProperty("row" + i++, model);
    }
}

Card* CardModelVector::getRoot()
{
    return cardModelVector.at(0)->getRoot();
}

// returns the CardModel object for the given card
CardModel* CardModelVector::getContainingModel(Card card)
{
    int level = card.getLevel();    // the level corresponds to the index of the internal vector
    // TODO: validate the index:
    // if (level < cardModelVector.size())
    return cardModelVector.at(level);
}

// populates the internal vector with a series of CardModels based on the given file
void CardModelVector::readAllCards()
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
void CardModelVector::addSpacers(Card* root)
{
    // TODO: spacers are added to end of parent's child list instead of being inserted. Is that a problem?

    // we don't need to add spacers for the actual root
    if (root->getLevel() == 0) {
        for (Card* child : root->getChildList()) {
            addSpacers(child);
        }
    } else if (root->getChildList().size() < 1 && root->getLevel() < (cardModelVector.size() - 1)) {
        // leaf node that isn't in the last column, add fillers out to the last column
        SpacerCard* temp;
        Card* parent = root;
        do {
            temp = new SpacerCard(parent, parent->getLevel() + 1);
            addCard(temp, temp->getLevel());
            parent = temp;
        } while (temp->getLevel() < cardModelVector.size());
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
void CardModelVector::insertSpacer(SpacerCard* spacer, int modelIndex, Card* sibling) {
    // TODO: error handling for out-of-bounds errors on modelIndex, which should never occur
    CardModel* model = cardModelVector.at(modelIndex);
    model->insertSpacer(spacer, sibling);
}

void CardModelVector::writeAllCards()
{
    if (!cardFile->open(QIODevice::WriteOnly | QIODevice::Text)) return; // TODO: add error handling

    QTextStream out(cardFile);

    // we start from the root card, and navigate all the child lists
    writeAllChildCards(getRoot(), &out);

    cardFile->close();
}

void CardModelVector::writeAllChildCards(Card* root, QTextStream* out)
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





















