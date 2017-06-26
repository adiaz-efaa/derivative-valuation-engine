#ifndef QCOPERATION_H
#define QCOPERATION_H

#include <vector>
#include <memory>

/*!
* @brief Por ahora el �nico prop�sito de esta clase es proveer un m�nimo de estructura para
* modelar operaciones a partir de vectores de QCInterestRatePayoff.
*/
class QCOperation
{
public:
	/*!
	* Constructor
	*/
	QCOperation()
	{
	}

	/*!
	* Retorna el valor de mercado de la operaci�n.
	*/
	virtual double marketValue() = 0;

	/*!
	* Destructor.
	*/
	virtual ~QCOperation()
	{}
};

#endif //QCOPERATION_H

