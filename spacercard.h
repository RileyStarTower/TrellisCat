#ifndef SPACERCARD_H
#define SPACERCARD_H

#include "card.h"

class SpacerCard : public Card
{
public:
    SpacerCard(Card* parent, int level, Card* sibling);
    SpacerCard(Card* parent, int level);

private:
    bool followedByCard();
};

#endif // SPACERCARD_H
