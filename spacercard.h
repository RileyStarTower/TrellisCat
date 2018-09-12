#ifndef SPACERCARD_H
#define SPACERCARD_H

#include "card.h"

class SpacerCard : public Card
{
public:
//    SpacerCard(Card* parent, int level, Card* sibling);
//    SpacerCard(Card* parent, int level);
    SpacerCard(Card* parent);

    void setDisconnected();

    void setSiblingType(int siblingType);
    int getSiblingType();

private:
    bool followedByCard();
};

#endif // SPACERCARD_H
