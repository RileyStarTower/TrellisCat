#ifndef CARDMODELVECTOR_H
#define CARDMODELVECTOR_H

#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QObject>
#include <QQmlContext>
#include <QQmlApplicationEngine>

#include "spacercard.h"

/**
 * @brief A vector of columns
 *
 * This class contains a vector of CardModels. Each model
 * in the vector is a column on the screen.
 *
 * This class also has methods for file IO, and finding
 * indexes for a given Card.
 */
class CardVector : public QObject
{
    Q_OBJECT
public:
    CardVector(QFile* cardFile);
    CardVector();

    void addColumn(QVector<Card*>* column);
    void addCard(Card* card, int columnIndex);
    void insertCard(Card* card, int columnIndex, int cardIndex);
    void addBodyText(Card* card, Card* parent);
    QVector<Card*>* getColumn(int index);
    Card* getRoot();
    void setCardFile(QFile* cardFile) { this->cardFile = cardFile; }
    int vectorSize();                       // returns the size of the underlying vectors (which are the same after adding spacers)
    int getColumnCount() { return cardVector.size(); }
    int getCardIndex(Card* card);
    void updateSpacers(Card* start);        // adds new spacers when we add new cards

public slots:
    void writeAllCards();

private:
    QVector<QVector<Card*>*> cardVector;    // internal vector
    QVector<Card*> bodyTextVector;          // vector of body text, which needs to be stored separately from the regular cards
    QFile* cardFile;                        // we can keep the file so we can write to it as changes occur


    void readAllCards();                                    // reads all cards from a given file QFile* cardFile
    void addSpacers(Card* root);                            // adds spacers to the tree of Cards, so the tree structure is preserved in the list models
    void insertSpacer(SpacerCard* spacer, int modelIndex, Card* sibling);
    void writeAllChildCards(Card* root, QTextStream *out);  // recursive method to write all cards to file
};

#endif // CARDMODELVECTOR_H
