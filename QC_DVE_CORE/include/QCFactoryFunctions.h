#ifndef QCFACTORYFUNCTIONS_H
#define QCFACTORYFUNCTIONS_H

#include <memory>
#include <string>

#include "QCYearFraction.h"
#include "QCWealthFactor.h"
#include "QCInterestRateLeg.h"
#include "QCDefinitions.h"
#include "QCDate.h"
#include "QCInterestRateCurve.h"

using namespace std;

namespace QCFactoryFunctions
{
	shared_ptr<QCYearFraction> yfSharedPtr(const string& whichYf);
	
	shared_ptr<QCWealthFactor> wfSharedPtr(const string& whichWf);

	shared_ptr<QCInterestRate> intRateSharedPtr(double value, const string& whichYf, const string& whichWf);
	
	QCZrCpnCrvShrdPtr zrCpnCrvShrdPtr(vector<long>& tenors, vector<double>& rates,
		const string& interpolator, const string& yf, const string& wf);
	
	QCZrCpnCrvShrdPtr zrCpnCrvShrdPtr(vector<long>& tenors, vector<double>& rates,
		const string& interpolator, const string& yf, const string& wf, const string& typeCurve);

	shared_ptr<QCInterestRateCurve> intRtCrvShrdPtr(
		vector<long>& tenors, vector<double>& rates,
		const string& interpolator, const string& wf, const string& yf,
		QCInterestRateCurve::QCTypeInterestRateCurve typeCurve);
	
	QCIntRtCrvShrdPtr discFctrCrvShrdPtr(vector<long>& tenors, vector<double>& dfs,
		const string& interpolator, const string& yf, const string& wf);

	QCInterestRateLeg buildFixedRateLeg(
		string receivePay,				//receive or pay
		QCDate startDate,				//start date
		QCDate endDate,					//end date
		vector<QCDate> calendar,		//settlement calendar
		int settlementLag,				//settlement lag
		QCInterestRateLeg::QCStubPeriod stubPeriod,		//stub period
		string periodicity,				//periodicity
		QCDate::QCBusDayAdjRules endDateAdjustment,		//end date adjustment
		QCInterestRateLeg::QCAmortization amortization,	//amortization
		vector<tuple<QCDate, double, double>> amortNotionalByDate, //amortization and notional by date
		double notional					//notional
		);

	QCInterestRateLeg buildIcpLeg(
		string receivePay,				//receive or pay
		QCDate startDate,				//start date
		QCDate endDate,					//end date
		vector<QCDate> calendar,		//settlement calendar
		int settlementLag,				//settlement lag
		QCInterestRateLeg::QCStubPeriod stubPeriod,		//stub period
		string periodicity,				//periodicity
		QCDate::QCBusDayAdjRules endDateAdjustment,		//end date adjustment
		QCInterestRateLeg::QCAmortization amortization,	//amortization
		vector<tuple<QCDate, double, double>> amortNotionalByDate, //amortization and notional by date
		double notional									//notional
		);

	QCInterestRateLeg buildFloatingRateLeg(
		string receivePay,					//receive or pay
		QCDate startDate,					//start date
		QCDate endDate,						//end date
		vector<QCDate> settleCalendar,		//settlement calendar
		int settlementLag,					//settlement lag
		string stubPeriod,					//stub period
		string settlePeriodicity,			//settlement periodicity
		QCDate::QCBusDayAdjRules endDateAdjustment, //end date adjustment
		string amortization,						//amortization
		vector<tuple<QCDate, double, double>> amortNotionalByDate,	//amortization and notional by end date
		string fixingStubPeriod,					//fixing stub period
		string fixingPeriodicity,					//fixing periodicity
		vector<QCDate> fixingCalendar,				//fixing calendar
		pair<string, string> interestRateIndexChars,	//interest rate index tenor (3M, 6M ...)	
		double notional								//notional
		);
};
#endif //QCFACTORYFUNCTIONS_H