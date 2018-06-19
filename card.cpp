#include "card.h"

Card::Card(QString fileLine)
{
    int levelPos = fileLine.indexOf('|');
    bool ok;

    level = fileLine.left(levelPos).toInt(&ok); // the level comes before the |

    // if (level == 1000) isBodyText = true; // this is jsut a weird value for now, but I should probably make some kind of constant or something
    cardText = QString(fileLine.mid(levelPos+1)); // the text comes after the |

    // initialize the parent to null, we calculate it after
    parent = Q_NULLPTR;

    cardType = 1;
}

Card::Card(int level, Card* parent)
{
    this->level = level;
    this->parent = parent;
    cardText = "";
    cardType = 1;
}

// Constructor with no input, just initialize everything
Card::Card()
{
    parent = Q_NULLPTR;
    level = 0;
    cardText = "";
    cardType = 1;
}

// Getter
QVariant Card::getCardText()
{
    return cardText;
}

// Setter
bool Card::setCardText(QVariant text)
{
    cardText = text.toString();
    return true;
}

// Adding a child card to the end of the card's child list
void Card::addChild(Card* child)
{
    childList.append(child);
}

void Card::insertChild(int index, Card* child)
{
    childList.insert(index, child);
}

// Removing a child card from whever it is in the parent's child list
void Card::removeChild(Card* child)
{
    childList.remove(childList.indexOf(child));
}

void Card::removeChild(int index)
{
    childList.remove(index);
}

// Return the child at the end of the card's child list
Card* Card::getLastChild()
{
    return childList.last();
}

/**
 * @brief Card::resolveParent
 *      finds the correct parent for a new card, based on the new Card's level
 *      relative to the most recent level we've been using
 * @param parent
 *      most recent parent used
 * @param childLevel
 *      most recent child level used
 * @return
 *      the parent for the Card being made
 */
Card* Card::resolveParent(Card *parent, int childLevel)
{
    Card* newParent;
    if (level == childLevel) {
        // if the new card is at the current level, then it has the same parent as the last card
        parent->addChild(this);
        setParent(parent);
        newParent = parent;
    } else if (level > childLevel) {
        // if we've gone down a level, then the previous Card (the parent's last child) is the new parent
        // TODO: this doesn't handle the situation where the first Card is not on level 1
        newParent = parent->getLastChild();
        setParent(newParent);
        if (level < 1000)
            newParent->addChild(this);
    } else {
        // this is the complicated case: if we've gone back up a level, then we need to find the right parent.
        // go back up the parent list until the level is higher (lower value)
        newParent = parent->getParent();
        while (newParent->getLevel() >= getLevel()) { // TODO: does the compiler optimize the getLevel call to a constant?
            newParent = newParent->getParent();
        }

        newParent->addChild(this);
        setParent(newParent);
    }

    return newParent;
}

bool Card::isSibling(Card* sibling)
{
    if (parent->childList.contains(sibling)) {
        // if both cards are children of the same parent, they are siblings
        return true;
    } else {
        return false;
    }
}

bool Card::hasCards()
{
    // just check the first child, since Spacers go after Cards
    return (childList.at(0)->getCardType() == 1);
}

// TODO: rename this, it's not very descriptive

/************************************
 * OK, SO, WE'RE KEEPING THAT 2D VECTOR, RIGHT?
 * LET'S JUST LOOK UP THE PREVIOUS CARD IN THAT VECTOR!
 * LIKE, WE KNOW WHAT LIST THE CARD IS IN, SO WE CAN JUST GO BACK UP THAT LIST!
 ************************************
 */
int Card::getPrevSiblingCount()
{
    if (findIndex() > 0) {
        // if it's not the first child, find the previous Card sibling
        int index = findIndex() - 1;
        Card* temp;
        temp = parent->getPrevCardChild(index);
        return temp->getChildCount();
    } else {
        // otherwise, find the previous parent, and use their last Card child
        Card* tempParent = parent->getParent()->getPrevCardChild(parent->findIndex() - 1);
//        while (tempParent->findIndex() == 0) {
//            // if the parent is also a first child, keep going up
//            tempParent = tempParent->getParent();
//        }
        Card* temp = tempParent->getPrevCardChild(tempParent->getChildCount() - 1);
        while (temp->getLevel() < level) {
            // if we went up, we have to come back down
            temp = getPrevCardChild(temp->getChildCount() - 1);
        }
        return temp->getChildCount();
    }
}

/* This function does not validate the child list
 * if there are no Cards in the list, it will hit an index out of bounds
 * you must validate the list before calling this function
 */
Card* Card::getPrevCardChild(int index) {
    // loop until we find a Card instead of a spacer
    while (childList.at(index)->getCardType() != 1) {
        index--;
    }
    return childList.at(index);
}

int Card::findIndex()
{
    return parent->getChildList().indexOf(this);
}

// Basically a setter, we're just adding new text to the card
void Card::addBodyText(Card* bodyText)
{
    bodyTextList.append(bodyText);
}

int Card::getChildType()
{
    if (parent->getLevel() == 0) {
        // if the parent is the root, then this isn't a child in the UI, so it's type 0
        return 0;
    } else if (parent->getChildList().indexOf(this) == 0) {
        // if this is the first child, it's type 1
        return 1;
    } else {
        // all other children are type 2
        return 2;
    }
}

int Card::getSiblingType()
{
    if ((parent->getChildList().size() == 1) || (parent->getLevel() == 0)) {
        // only child, and the top level is type 0
        return 0;
    } else if (parent->getChildList().indexOf(this) == 0) {
        // first child is type 1
        return 1;
    }   else if (parent->getChildList().indexOf(this) < parent->getChildList().size() - 1) {
        // middle child is type 2
        return 2;
    } else {
        // last child is type 3
        return 3;
    }
}

int Card::getChildCount()
{
    return childList.size();
}

// Helpful method to compose the card's level and text into the internal storage format
QString Card::getFileText()
{
    QString fileText = QString::number(getLevel()) + "|" + cardText;
    return fileText;
}
