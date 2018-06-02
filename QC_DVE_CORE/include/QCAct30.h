//
// Created by Alvaro Diaz on 16-07-16.
//

#ifndef QCACT30_H
#define QCACT30_H


#include "QCYearFraction.h"

/*!
* @brief La clase QCAct30 implementa el m�todo Act/30.
* Hereda de la clase abstracta QCYearFraction
*/
class QCAct30 : public QCYearFraction
{
public:
	/*!
	* La funci�n yf devuelve la fracci�n de a�o entre dos fechas en convenci�n Act/30.
	* @param firstDate es la fecha m�s antigua de las dos si se desea retornar un valor positivo
	* @param secondDate es la fecha m�s reciente de las dos si se desea retornar un valor positivo
	* @return un double con la fracci�n de a�o calculada
	*/
	double yf(const QCDate& firstDate, const QCDate& secondDate);

	/*!
	* La funci�n yf devuelve un proxy de la fracci�n de a�o cuando el argumento es un
	* numero de dias.
	* @param days
	* @return un double con la fracci�n de a�o calculada
	*/
	virtual double yf(long days);

	/*!
	* La funci�n countDays devuelve el n�mero de d�as entre firstDate y secondDate en Act/360.
	* Si se desea un n�mero positivo firstDate debe ser menor que secondDate
	* @param firstDate es la fecha m�s antigua de las dos si se desea retornar un valor positivo
	* @param secondDate es la fecha m�s reciente de las dos si se desea retornar un valor positivo
	* @return un long con el n�mero de d�as calculados
	*/
	long countDays(const QCDate& firstDate, const QCDate& secondDate);

	/**
	* @fn	std::string QCAct30::description();
	*
	* @brief	Gets the description. In this case "Act360"
	*
	* @author	Alvaro D�az V.
	* @date	28/09/2017
	*
	* @return	A std::string.
	*/
	std::string description();

private:
	/*!
	* En esta variable se guarda la base de la tasa que, en este caso, es un n�mero fijo.
	*/
	const double _basis = 30.0;
};


#endif //QCACT30_H
