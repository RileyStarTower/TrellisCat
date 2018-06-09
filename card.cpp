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

// Removing a child card from whever it is in the parent's child list
void Card::removeChild(Card* child)
{
    childList.remove(childList.indexOf(child));
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

// Basically a setter, we're just adding new text to the card
void Card::addBodyText(Card* bodyText)
{
    bodyTextList.append(bodyText);
}

// Helpful method to compose the card's level and text into the internal storage format
QString Card::getFileText()
{
    QString fileText = QString::number(getLevel()) + "|" + cardText;
    return fileText;
}






















