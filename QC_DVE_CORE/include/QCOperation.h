#ifndef QCOPERATION_H
#define QCOPERATION_H

#include <vector>
#include <memory>

#include "QCInterestRatePayoff.h"

using namespace std;

/*!
* @brief Por ahora el �nico prop�sito de esta clase es proveer un m�nimo de estructura para
* modelar operaciones a partir de vectores de QCInterestRatePayoff.
*/
class QCOperation
{
public:
	QCOperation()
	{
	}

	virtual double marketValue() = 0;

	virtual ~QCOperation();
};

#endif //QCOPERATION_H

