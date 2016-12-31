#ifndef QCFIXEDRATEPAYOFF_H
#define QCFIXEDRATEPAYOFF_H

#include "QCInterestRatePayoff.h"
#include "QCInterestRate.h"

class QCFixedRatePayoff :
	public QCInterestRatePayoff
{
public:
	QCFixedRatePayoff(QCIntrstRtShrdPtr fixedRate,
		shared_ptr<QCInterestRateLeg> irLeg,
		QCIntRtCrvShrdPtr discountCurve,
		QCDate valueDate,
		QCTimeSeriesShrdPtr fixingData);

	virtual ~QCFixedRatePayoff();

protected:
	virtual void _setAllRates() override;

private:

};

#endif //QCFIXEDRATEPAYOFF_H

