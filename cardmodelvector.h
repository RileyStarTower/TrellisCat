#ifndef CARDMODELVECTOR_H
#define CARDMODELVECTOR_H

#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QObject>
#include <QQmlContext>
#include <QQmlApplicationEngine>

#include "cardmodel.h"
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
class CardModelVector : public QObject
{
    Q_OBJECT
public:
    CardModelVector(QFile* cardFile);
    CardModelVector();

    void addModel(CardModel model);
    void addCard(Card* card, int modelIndex);
    void addBodyText(Card* card, Card* parent);
    CardModel* getCardModel(int index);
    void connectModels(QQmlContext* context);
    Card* getRoot();

    CardModel* getContainingModel(Card card);

    CardModelVector* flattenModel();

    // TODO: figure out a better solution than this
    void setCardFile(QFile* cardFile) { this->cardFile = cardFile; }

public slots:
    void writeAllCards();

private:
    QVector<CardModel*> cardModelVector;    // internal vector
    QVector<Card*> bodyTextVector;          // vector of body text, which needs to be stored separately from the regular cards
    QFile* cardFile;                        // we can keep the file so we can write to it as changes occur

    int vectorSize();                       // returns the size of the underlying vectors (which are the same after adding spacers)

    void readAllCards();                    // reads all cards from a given file QFile* cardFile
    void addSpacers(Card* root);            // adds spacers to the tree of Cards, so the tree structure is preserved in the list models
    void insertSpacer(SpacerCard* spacer, int modelIndex, Card* sibling);
    void writeAllChildCards(Card* root, QTextStream *out);  // recursive method to write all cards to file
};

#endif // CARDMODELVECTOR_H
