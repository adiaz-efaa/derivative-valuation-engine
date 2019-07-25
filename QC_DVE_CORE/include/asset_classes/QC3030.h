//
// Created by Alvaro Diaz on 16-07-16.
//


#ifndef QC3030_H
#define QC3030_H


#include "QCYearFraction.h"

/*!
* @brief La clase QC30360 implementa el m�todo 30/360.
* Hereda de la clase abstracta QCYearFraction
* @author Alvaro D�az
*/
class QC3030 : public QCYearFraction
{
public:
	/*!
	* La funci�n yf devuelve la fracci�n de a�o entre dos fechas en convenci�n 30/360.
	* @param firstDate es la fecha m�s antigua de las dos si se desea retornar un valor positivo
	* @param secondDate es la fecha m�s reciente de las dos si se desea retornar un valor positivo
	* @return un double con la fracci�n de a�o calculada
	*/
	double yf(const QCDate& firstDate, const QCDate& secondDate);

	/*!
	* La funci�n countDays devuelve el n�mero de d�as entre firstDate y secondDate en 30/360.
	* Si se desea un n�mero positivo firstDate debe ser menor que secondDate
	* @param firstDate es la fecha m�s antigua de las dos si se desea retornar un valor positivo
	* @param secondDate es la fecha m�s reciente de las dos si se desea retornar un valor positivo
	* @return un long con el n�mero de d�as calculados
	*/
	long countDays(const QCDate& firstDate, const QCDate& secondDate);

	/*!
	* La funci�n yf devuelve un proxy de la fracci�n de a�o cuando el argumento es un
	* numero de dias.
	* @param days corresponde a un numero de dias suponiendo que este numero ya esta bien calculado
	* @return un double con la fracci�n de a�o calculada
	*/
	virtual double yf(long days);

	/**
	* @fn	std::string QCAct360::description();
	*
	* @brief	Gets the description. In this case "30360"
	*
	* @author	Alvaro D�az V.
	* @date	28/09/2017
	*
	* @return	A std::string.
	*/
	std::string description();

private:
	const double _basis = 30.0;
};


#endif //QC3030_H
