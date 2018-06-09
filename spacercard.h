#ifndef SPACERCARD_H
#define SPACERCARD_H

#include "card.h"

class SpacerCard : public Card
{
public:
    SpacerCard(Card* parent, int level);
};

#endif // SPACERCARD_H
