#ifndef CARD_H
#define CARD_H

#include <QString>
#include <QVariant>
#include <QVector>

/**
 * @brief A single card
 *
 * The basic unit of data in the program, it's sort of like a fancy notecard.
 * Mostly just has getters and setters, but there's a complicated function to
 * find the correct parent for a newly created card.
 */

class Card
{
public:
    // main constructor takes in one line from the file
    Card(QString fileLine);
    // additional constructor for adding new Cards
    Card(int level, Card* parent);
    // we also need a blank constructor
    Card();

    // make these generic so I can extend the functionality later
    QVariant getCardText();
    bool setCardText(QVariant text);
    QString getFileText();

    // child management methods
    void addChild(Card* child);
    void insertChild(int index, Card* child);
    void removeChild(Card* child);
    void removeChild(int index);
    Card* getFirstChild();
    Card* getLastChild();
    Card* getChild(int index);
    Card* resolveParent(Card* parent, int childLevel);
    bool isSibling(Card* sibling);
    bool hasCards();

    // getters and setters
    int getLevel() const {return level;}
    void setLevel(int level) {this->level = level;}
    void setParent(Card* parent) {this->parent = parent;}
    Card* getParent() const {return parent;}
    int getCardType() { return cardType; }

    void addBodyText(Card* bodyText);
    QVector<Card*> getChildList() const {return childList;}
    QVector<Card*> getBodyTextList() const {return bodyTextList;}
    int findIndex();

    // roles for the Card UI connections
    int getChildType();
    int getSiblingType();
    int getChildCount();

private:
    QString cardText;   // main text of the card
    QVector<Card*> bodyTextList;

protected:
    int cardType;
    int siblingType;
    Card* parent;       // parent card pointer
    int level;          // level, i.e. which column is it in
    QVector<Card*> childList;

};

#endif // CARD_H
