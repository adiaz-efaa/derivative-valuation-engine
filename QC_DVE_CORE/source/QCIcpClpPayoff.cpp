#include "QCIcpClpPayoff.h"
#include <cmath>

QCIcpClpPayoff::QCIcpClpPayoff(QCIntrstRtShrdPtr floatingRate,
	double additiveSpread,
	double multipSpread,
	shared_ptr<QCInterestRateLeg> irLeg,
	QCIntRtCrvShrdPtr projectingCurve,
	QCIntRtCrvShrdPtr discountCurve, //QCZrCpnCrvShrdPtr
	QCDate valueDate,
	QCTimeSeriesShrdPtr fixingData) :
	QCInterestRatePayoff(floatingRate, irLeg, valueDate, projectingCurve, discountCurve, fixingData),
	_additiveSpread(additiveSpread), _multipSpread(multipSpread)
{
	_setAllRates();
}

double QCIcpClpPayoff::getForwardRateAt(int n)
{
	return _forwardRates.at(n);
}

void QCIcpClpPayoff::_setAllRates()
{
	//Loopea sobre los periods de interest rate leg a partir del current period
	size_t numPeriods = _irLeg->size();
	_allRates.resize(numPeriods);
	_forwardRates.resize(numPeriods);
	_allRatesDerivatives.resize(numPeriods);
	size_t tempCurrentPeriod;
	if (_currentPeriod == -1)
	{
		tempCurrentPeriod = 0;
	}
	else
	{
		tempCurrentPeriod = _currentPeriod;
	}
	for (size_t i = tempCurrentPeriod; i < numPeriods; ++i)
	{
		auto per = _irLeg->getPeriodAt(i);
		//Si el fixing date es menor que _valueDate lo va a calcular con fixingData
		QCDate startDate = get<QCInterestRateLeg::intRtPrdElmntStartDate>(per); //Fecha inicial del periodo
		if (_valueDate >= startDate)
		{
			//Trae ICP a startDate
			// cout << "Start date: " << startDate.description() << endl;
			double icpStart = _fixingData->at(startDate);
			// cout << "Ok start date" << endl;
			
			//Trae ICP _valueDate
			double icpValue = _fixingData->at(_valueDate);

			//Calcula la TNA con el redondeo. Sea pTNA el plazo desde el inicio del cupon
			//hasta _valueDate. El redondeo se aplica para que al final del periodo 
			//el cashflow coincida con el de contrato.
			int pTNA = startDate.dayDiff(_valueDate);

			//Esta linea debe reemplazar la anterior
			pTNA = _rate->getYearFraction()->countDays(startDate, _valueDate);

			double TNA;
			if (pTNA == 0)
			{
				TNA = 0;
			}
			else
			{
				TNA = round((icpValue / icpStart - 1) * 360.0 / (double)pTNA * 10000) / 10000.0;

				// Esta linea reemplaza a la anterior
				TNA = round(_rate->getRateFromWf(icpValue / icpStart, pTNA) * 10000) / 10000.0;

				/* cout << "icp inicio: " << icpValue << endl;
				cout << "icp hoy: " << icpStart << endl;
				cout << "TNA: " << TNA << endl;
				*/
			}
			
			//Calcula wf = (1+TNA*pTNA/360)*(1+z*pZ/360)
			_rate->setValue(TNA);
			double wfTNA = _rate->wf(pTNA);

			//Calcula el plazo pZ desde valueDate hasta endDate
			QCDate endDate = get<QCInterestRateLeg::intRtPrdElmntEndDate>(per);
			int pZ = _valueDate.dayDiff(endDate);

			//Calcula wfZ al plazo pZ
			double wfZ = QCInterestRatePayoff::_projectingCurve->getForwardWf(0, pZ);
			//Se calculan y guardan las derivadas de este factor Fwd
			vector<double> tempDer;
			tempDer.resize(QCInterestRatePayoff::_projectingCurve->getLength());
			for (unsigned int j = 0; j < QCInterestRatePayoff::_projectingCurve->getLength(); ++j)
			{
				tempDer.at(j) = QCInterestRatePayoff::_projectingCurve->fwdWfDerivativeAt(j)
					* wfTNA * 360.0 / (pZ + pTNA);

				// Esta linea reemplaza la anterior. El plazo Actual se reemplaza por el plazo
				// de la tasa definida en el payoff.
                tempDer.at(j) = QCInterestRatePayoff::_projectingCurve->fwdWfDerivativeAt(j)
                                * wfTNA / _rate->getYearFraction()->yf(startDate, endDate) ;

				//cout << "der: " << j << ": " << tempDer.at(j) << endl;
				//(wfTNA*wfZ-1)*360/(pZ+pTNA)
				//wfTNA*wfZ*360/(pZ+pTNA)-360/(pZ+pTNA)

			}
			_allRatesDerivatives.at(i) = tempDer;

			//Se calcula y guarda la tasa forward z a partir de wfZ
			double z = _rate->getRateFromWf(wfZ, pZ);

			// Las siguientes dos lineas reemplazan a la anterior
			pZ = _rate->getYearFraction()->countDays(_valueDate, endDate);
			z =  _rate->getRateFromWf(wfZ, pZ); // La definición de pZ cambió
			_forwardRates.at(i) = z;

			//Calcula tasa asociada a este wf y guardar en _allRates
			_allRates.at(i) = (_rate->getRateFromWf(wfTNA * wfZ, pZ + pTNA)
				+ _additiveSpread) * _multipSpread;

			// La siguiente linea reemplaza a la anterior
            _allRates.at(i) = (_rate->getRateFromWf(wfTNA * wfZ, startDate, endDate)
                               + _additiveSpread) * _multipSpread;

			//cout << "all rates: " << i << " " << _allRates.at(i) << endl;
		}
		else
		{
			//Si no calcula el factor de descuento fwd segun los parametros del periodo
			QCDate date1 = get<QCInterestRateLeg::intRtPrdElmntStartDate>(per);
			QCDate date2 = get<QCInterestRateLeg::intRtPrdElmntEndDate>(per);
			long d1 = _valueDate.dayDiff(date1);
			long d2 = _valueDate.dayDiff(date2);

			//La tasa fwd se expresa en la convencion en que se construyo
			//la curva de proyeccion. Al fabricar el payoff se debe poner atencion
			//a que esta coincida con las caracteristicas del swap.
			double wfFwd = QCInterestRatePayoff::_projectingCurve->getForwardWf(d1, d2);

			//Se calculan y guardan las derivadas de este factor Fwd
			vector<double> tempDer;
			tempDer.resize(QCInterestRatePayoff::_projectingCurve->getLength());
			for (unsigned int j = 0; j < QCInterestRatePayoff::_projectingCurve->getLength(); ++j)
			{
				tempDer.at(j) = QCInterestRatePayoff::_projectingCurve->fwdWfDerivativeAt(j)
					* 360.0 / (d2 - d1);

				// Esta linea reemplaza la anterior
                tempDer.at(j) = QCInterestRatePayoff::_projectingCurve->fwdWfDerivativeAt(j)
                                / _rate->getYearFraction()->yf(date1, date2);

				//cout << "d1: " << d1 << endl;
				//cout << "d2: " << d2 << endl;
			}
			_allRatesDerivatives.at(i) = tempDer;

			//Cada tasa fwd (o fijacion anterior) se guarda en _forwardRates
			_forwardRates.at(i) = _rate->getRateFromWf(wfFwd, d2 - d1);

			// Esta linea reemplaza a la anterior
            _forwardRates.at(i) = _rate->getRateFromWf(wfFwd, date1, date2);

			//Se aplican los spreads y se guarda la tasa en _allRates
			_allRates.at(i) = (_forwardRates.at(i) + _additiveSpread) * _multipSpread;
			//cout << "all rates: " << i << " " << _allRates.at(i) << endl;

		}
	}
}

QCIcpClpPayoff::~QCIcpClpPayoff()
{
}
