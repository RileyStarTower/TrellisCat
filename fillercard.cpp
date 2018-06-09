#include "spacercard.h"

SpacerCard::SpacerCard(Card* parent, int level)
{
    cardType = 2;
    // TODO: I think there's a more compact way of initializing these
    this->level = level;
    this->parent = parent;
    parent->addChild(this);
}
