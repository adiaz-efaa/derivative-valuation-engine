#include "QCDiscountBondPayoff.h"
#include "QCFactoryFunctions.h"

QCDiscountBondPayoff::QCDiscountBondPayoff(
	shared_ptr<QCInterestRateLeg> irLeg,
	QCIntRtCrvShrdPtr discountCurve,
	QCDate valueDate,
	QCTimeSeriesShrdPtr fxFixingData,
	QCCurrencyConverter::QCCurrency notionalCurrency,
	QCCurrencyConverter::QCFxRate fxRate
	) :
	QCInterestRatePayoff(QCFactoryFunctions::zeroIntRateSharedPtr(),irLeg, valueDate, discountCurve,
	discountCurve, fxFixingData), _notionalCurrency(notionalCurrency), _fxRate(fxRate)
{
	if (_irLeg->size() > 1)
	{
		throw invalid_argument("IR Leg for a discount bond payoff can have only one period.");
	}
	
	_setAllRates();
}

void QCDiscountBondPayoff::_setAllRates()
{
	unsigned int numPeriods = _irLeg->size();

	_allRates.resize(numPeriods);
	double rate{ _rate->getValue() };
	for (unsigned int i = 0; i <numPeriods; ++i)
	{
		_allRates.at(i) = rate;
	}
	long curveLength{ _discountCurve->getLength() };
	vector<double> temp(curveLength, 0.0);
	_allRatesDerivatives.resize(numPeriods);
	for (unsigned int i = 0; i < numPeriods; ++i)
	{
		_allRatesDerivatives.at(i) = temp;
	}
}

double QCDiscountBondPayoff::presentValue()
{
	double pv = QCInterestRatePayoff::presentValue();
	//_valueDateCashflow = 0;
	return pv;
}

QCCurrencyConverter::QCCurrency QCDiscountBondPayoff::getNotionalCurrency()
{
	return _notionalCurrency;
}

QCDiscountBondPayoff::~QCDiscountBondPayoff()
{
}

