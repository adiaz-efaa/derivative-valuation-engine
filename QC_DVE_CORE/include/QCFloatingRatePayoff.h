#ifndef QCFLOATINGRATEPAYOFF_H
#define QCFLOATINGRATEPAYOFF_H

#include <iostream>
#include "QCInterestRatePayoff.h"

using namespace std;

/*!
* @brief QCFloatingRatePayoff hereda de QCInterestRatePayoff e implementa la estructura de un bono
* o pata a tasa flotante.
*/
class QCFloatingRatePayoff :
	public QCInterestRatePayoff
{
public:
	/*!
	* Constructor, se apoya en el constructor de la clase madre.
	* @param floatingRate valor y forma de la tasa flotante. El valor de la tasa no es importante.
	* El factor del �ltimo fixing se obtiene desde el par�mtero fixingData.
	* @param additiveSpread spread en la misma convenci�n de la tasa flotante que se suma a esta
	* �ltima para calcular los intereses de un per�odo.
	* @param multipSpread spread multiplicativo que se aplica la tasa flotante para calcular los
	* intereses de un per�odo. Si un QCFloatingRatePayoff tiene spread aditivo (sa) y spread multiplicativo
	* sm, si flot es la tasa flotante de un per�odo, los interes de ese per�odo se calculan como
	* wf(flot*sm + sa, yf).
	* @param irLeg estructura de fechas y amortizaciones.
	* @param projectingCurve curva de proyecci�n de las tasas flotantes (para la obtenci�n del valor presente).
	* @param discountCurve curva de descuento (para la obtenci�n del valor presente)
	* @param valueDate fecha de valorizaci�n.
	* @param fixingData contiene una serie hist�rica de valores de la tasa flotante. Sirve para fijar
	* el valor de la tasa del flujo de intereses corriente (seg�n valueDate).
	*/
	QCFloatingRatePayoff(QCIntrstRtShrdPtr floatingRate,
		double additiveSpread,
		double multipSpread,
		shared_ptr<QCInterestRateLeg> irLeg,
		QCIntRtCrvShrdPtr projectingCurve,
		QCIntRtCrvShrdPtr discountCurve,
		QCDate valueDate,
		QCTimeSeriesShrdPtr fixingData
		);
	/*!
	* Sobrecarga del operador <. Se ordena por fecha final.
	* @param rhs otro QCFloatingRatePayoff para comparar (this < rhs)
	*/
	bool operator<(const QCFloatingRatePayoff& rhs);

	/*!
	* Similar al operador < pero act�a sobre shared_ptr<QCFloatingRatePayoff>.
	* @param lhs lado izquierdo de la comparaci�n.
	* @param rhs lado derecho de la comparaci�n.
	*/
	static bool lessThan(shared_ptr<QCFloatingRatePayoff> lhs, shared_ptr<QCFloatingRatePayoff> rhs);

	/*!
	* Retorna la tasa forward n�mero n calculada para el payoff, considerando valueDate.
	* @param n posici�n de la tasa forward buscada.
	* @return valor de la tasa forward.
	*/
	double getForwardRateAt(size_t n);

	/*!
	* Destructor.
	*/
	virtual ~QCFloatingRatePayoff();

protected:
	/*!
	* Se fija la tasa a aplicar en cada per�odo.
	*/
	virtual void _setAllRates() override;

	/*!
	* Variable que almacena los valores y caracter�sticas de la curva de proyecci�n.
	*/
	QCIntRtCrvShrdPtr _projectingCurve;

	/*!
	* Variable que almacena el spread aditivo.
	*/
	double _additiveSpread;

	/*!
	* Variable que almacena el spread multiplicativo.
	*/
	double _multipSpread;

	/*!
	* Aqui se guardan todas las fijaciones futuras libres de spreads.
	*/
	vector<double> _forwardRates; 
};

#endif //QCFLOATINGRATEPAYOFF_H
