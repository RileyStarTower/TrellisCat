#include <QtDebug>

#include "spacercard.h"

// This constructor inserts a spacer in the parent's child list after the sibling Card
SpacerCard::SpacerCard(Card* parent, int level, Card* sibling)
{
    cardType = 2;
    // TODO: I think there's a more compact way of initializing these
    this->level = level;
    this->parent = parent;
    int spacerIndex = parent->getChildList().indexOf(sibling) + 1;
    if (spacerIndex < parent->getChildList().size()) {
        parent->insertChild(spacerIndex, this);
    } else {
        parent->addChild(this);
    }
}

// This constructor just adds a spacer to the parent's child list, intended for when the child list is empty otherwise
SpacerCard::SpacerCard(Card* parent, int level)
{
    cardType = 2;
    // TODO: I think there's a more compact way of initializing these
    this->level = level;
    this->parent = parent;
    parent->addChild(this);
}

// This constructor copies the sibling, and is used when we're using the parent's insert method
SpacerCard::SpacerCard(Card* parent)
{
    cardType = 2;
    // TODO: I think there's a more compact way of initializing these
    this->level = parent->getLevel() + 1;
    this->parent = parent;
}

// TODO: we'll need to reset this in the add card method
void SpacerCard::setDisconnected()
{
    cardType = 0;
}

bool SpacerCard::followedByCard()
{
    // start at the current SpacerCard's next sibling
    int idx = parent->getChildList().indexOf(this) + 1;
    for (; idx < parent->getChildList().size(); idx++) {
        // if the sibling is a Card instead of a spacer, then return true
        if (parent->getChildList().at(idx)->getCardType() == 1) {
            return true;
        }
    }

    return false;
}
