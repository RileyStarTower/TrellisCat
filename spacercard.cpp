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
