#ifndef WRAPPERS_H
#define WRAPPERS_H

#include<memory>
#include<string>

#include <boost/python.hpp>
#include <boost/python/wrapper.hpp>

#include "Cashflow.h"
#include "FixedRateCashflow.h"
#include "IborCashflow.h"
#include "SimpleCashflow.h"
#include "SimpleMultiCurrencyCashflow.h"
#include "IcpClpCashflow.h"
#include "IcpClfCashflow.h"
#include "InterestRateCurve.h"
#include "ZeroCouponCurve.h"
#include "PresentValue.h"

#include "QCDate.h"
#include "QCCurrency.h"
#include "QCInterpolator.h"
#include "QCLinearInterpolator.h"
#include "QCInterestRate.h"

namespace qf = QCode::Financial;

namespace wrappers
{
	PyObject* show(std::shared_ptr<qf::FixedRateCashflow> cshflwPtr)
	{
		// The types inside the wrapper are:
		// QCDate, QCDate, QCDate, double, double, double, bool,
		// shared_ptr<QCCurrency>, QCInterestRate

		auto cshflw = *cshflwPtr;
		auto cashflow = cshflw.wrap();

		// We will first unpack the wrapper
		std::string startDate;
		std::string endDate;
		std::string settlementDate;
		double nominal;
		double amortization;
		double interest;
		long doesAmortize = 0;
		std::string currency;
		try
		{
			startDate = std::get<0>(*cashflow).description(false);
			endDate = std::get<1>(*cashflow).description(false);
			settlementDate = std::get<2>(*cashflow).description(false);
			nominal = std::get<3>(*cashflow);
			amortization = std::get<4>(*cashflow);
			interest = std::get<5>(*cashflow);
			if (std::get<6>(*cashflow))
			{
				doesAmortize = 1;
			}
			currency = std::get<7>(*cashflow)->getIsoCode();
		}
		catch (exception& e)
		{
			string msg = string(e.what());
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, msg.c_str());
			return NULL;
		}

		// Now we pack the values in a Python tuple
		PyObject* result = PyTuple_New(11);
		int success;
		
		success = PyTuple_SetItem(result, 0, PyString_FromString(startDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "startDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 1, PyString_FromString(endDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "endDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 2, PyString_FromString(settlementDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "settlementDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 3, PyFloat_FromDouble(nominal));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "nominal");
			return NULL;
		}

		success = PyTuple_SetItem(result, 4, PyFloat_FromDouble(amortization));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "amortization");
			return NULL;
		}

		success = PyTuple_SetItem(result, 5, PyFloat_FromDouble(interest));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "interest");
			return NULL;
		}
		
		success = PyTuple_SetItem(result, 6, PyBool_FromLong(doesAmortize));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "doesAmortize");
			return NULL;
		}
		
		double amount = cshflw.amount();
		success = PyTuple_SetItem(result, 7, PyFloat_FromDouble(amount));
		if (success != 0)
			{
				PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
				PyErr_SetString(qcfError, "total flow");
				return NULL;
			}

		success = PyTuple_SetItem(result, 8, PyString_FromString(currency.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "currency");
			return NULL;
		}

		double rate = std::get<8>(*cashflow).getValue();
		success = PyTuple_SetItem(result, 9, PyFloat_FromDouble(rate));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "rate value");
			return NULL;
		}

		std::string wf = std::get<8>(*cashflow).getWealthFactor()->description();
		std::string yf = std::get<8>(*cashflow).getYearFraction()->description();
		success = PyTuple_SetItem(result, 10, PyString_FromString((wf + yf).c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "currency");
			return NULL;
		}

		return result;
	}

	PyObject* show(qf::FixedRateCashflow cshflw)
	{
		// The types inside the wrapper are:
		// QCDate, QCDate, QCDate, double, double, double, bool,
		// shared_ptr<QCCurrency>, QCInterestRate

		auto cashflow = cshflw.wrap();

		// We will first unpack the wrapper
		std::string startDate;
		std::string endDate;
		std::string settlementDate;
		double nominal;
		double amortization;
		double interest;
		long doesAmortize = 0;
		std::string currency;
		try
		{
			startDate = std::get<0>(*cashflow).description(false);
			endDate = std::get<1>(*cashflow).description(false);
			settlementDate = std::get<2>(*cashflow).description(false);
			nominal = std::get<3>(*cashflow);
			amortization = std::get<4>(*cashflow);
			interest = std::get<5>(*cashflow);
			if (std::get<6>(*cashflow))
			{
				doesAmortize = 1;
			}
			currency = std::get<7>(*cashflow)->getIsoCode();
		}
		catch (exception& e)
		{
			string msg = string(e.what());
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, msg.c_str());
			return NULL;
		}

		// Now we pack the values in a Python tuple
		PyObject* result = PyTuple_New(11);
		int success;

		success = PyTuple_SetItem(result, 0, PyString_FromString(startDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "startDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 1, PyString_FromString(endDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "endDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 2, PyString_FromString(settlementDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "settlementDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 3, PyFloat_FromDouble(nominal));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "nominal");
			return NULL;
		}

		success = PyTuple_SetItem(result, 4, PyFloat_FromDouble(amortization));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "amortization");
			return NULL;
		}

		success = PyTuple_SetItem(result, 5, PyFloat_FromDouble(interest));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "interest");
			return NULL;
		}

		success = PyTuple_SetItem(result, 6, PyBool_FromLong(doesAmortize));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "doesAmortize");
			return NULL;
		}

		double amount = cshflw.amount();
		success = PyTuple_SetItem(result, 7, PyFloat_FromDouble(amount));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "total flow");
			return NULL;
		}

		success = PyTuple_SetItem(result, 8, PyString_FromString(currency.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "currency");
			return NULL;
		}

		double rate = std::get<8>(*cashflow).getValue();
		success = PyTuple_SetItem(result, 9, PyFloat_FromDouble(rate));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "rate value");
			return NULL;
		}

		std::string wf = std::get<8>(*cashflow).getWealthFactor()->description();
		std::string yf = std::get<8>(*cashflow).getYearFraction()->description();
		success = PyTuple_SetItem(result, 10, PyString_FromString((wf + yf).c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "currency");
			return NULL;
		}

		return result;
	}

	PyObject* show(qf::IborCashflow cshflw)
	{
		// The types inside the wrapper are:
		// startDate,
		// endDate,
		// fixingDate,
		// settlementDate,
		// nominal,
		// amortization,
		// interest,
		// doesAmortize,
		// amount,
		// _currency,
		// _index->getCode(),
		// _index->getRate(),
		// _spread,
		// _gearing);
		
		auto cashflow = cshflw.wrap();

		// We will first unpack the wrapper
		std::string startDate;
		std::string endDate;
		std::string fixingDate;
		std::string settlementDate;
		double nominal;
		double amortization;
		double interest;
		long doesAmortize = 0;
		double cashflowAmount;
		std::string currency;
		std::string code;
		double rate_value;
		double spread;
		double gearing;
		try
		{
			startDate = std::get<0>(*cashflow).description(false);
			endDate = std::get<1>(*cashflow).description(false);
			fixingDate = std::get<2>(*cashflow).description(false);
			settlementDate = std::get<3>(*cashflow).description(false);

			nominal = std::get<4>(*cashflow);
			amortization = std::get<5>(*cashflow);
			interest = std::get<6>(*cashflow);
			if (std::get<7>(*cashflow))
			{
				doesAmortize = 1;
			}
			cashflowAmount = std::get<8>(*cashflow);
			currency = std::get<9>(*cashflow)->getIsoCode();
			code = std::get<10>(*cashflow);
			rate_value = std::get<11>(*cashflow).getValue();
			spread = std::get<12>(*cashflow);
			gearing = std::get<13>(*cashflow);
		}
		catch (exception& e)
		{
			string msg = string(e.what());
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, msg.c_str());
			return NULL;
		}

		// Now we pack the values in a Python tuple
		PyObject* result = PyTuple_New(15);
		int success;

		success = PyTuple_SetItem(result, 0, PyString_FromString(startDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "startDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 1, PyString_FromString(endDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "endDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 2, PyString_FromString(fixingDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "fixingDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 3, PyString_FromString(settlementDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "settlementDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 4, PyFloat_FromDouble(nominal));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "nominal");
			return NULL;
		}

		success = PyTuple_SetItem(result, 5, PyFloat_FromDouble(amortization));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "amortization");
			return NULL;
		}

		success = PyTuple_SetItem(result, 6, PyFloat_FromDouble(interest));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "interest");
			return NULL;
		}

		success = PyTuple_SetItem(result, 7, PyBool_FromLong(doesAmortize));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "doesAmortize");
			return NULL;
		}

		success = PyTuple_SetItem(result, 8, PyFloat_FromDouble(cashflowAmount));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "total flow");
			return NULL;
		}

		success = PyTuple_SetItem(result, 9, PyString_FromString(currency.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "currency");
			return NULL;
		}

		const char* cCode = code.c_str();
		success = PyTuple_SetItem(result, 10, PyString_FromString(cCode));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "ir code");
			return NULL;
		}

		double rate = std::get<11>(*cashflow).getValue();
		success = PyTuple_SetItem(result, 11, PyFloat_FromDouble(rate));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "rate value");
			return NULL;
		}

		success = PyTuple_SetItem(result, 12, PyFloat_FromDouble(spread));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "spread");
			return NULL;
		}

		success = PyTuple_SetItem(result, 13, PyFloat_FromDouble(gearing));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "total flow");
			return NULL;
		}

		std::string wf = std::get<11>(*cashflow).getWealthFactor()->description();
		std::string yf = std::get<11>(*cashflow).getYearFraction()->description();
		success = PyTuple_SetItem(result, 14, PyString_FromString((wf + yf).c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "currency");
			return NULL;
		}

		return result;

	}

	PyObject* show(std::shared_ptr<qf::IborCashflow> cshflwPtr)
	{
		// The types inside the wrapper are:
		// startDate       QCDate,
		// endDate         QCDate,
		// fixingDate      QCDate,
		// settlementDate  QCDate,
		// nominal         double,
		// amortization    double,
		// interest        double,
		// doesAmortize    bool,
		// currency        shared_ptr<QCCurrency>,
		// code            std::string,
		// rate            QCInterestRate,
		// spread          double,
		// gearing         double.
		
		auto cshflw = *cshflwPtr;
		return show(cshflw);
	}

	PyObject* show(qf::SimpleCashflow cshflw)
	{
		// The types inside the wrapper are:
		// QCDate, double, shared_ptr<QCCurrency>

		auto cashflow = cshflw.wrap();

		// We will first unpack the wrapper
		std::string endDate;
		double nominal;
		std::string currency;
		try
		{
			endDate = std::get<0>(*cashflow).description(false);
			nominal = std::get<1>(*cashflow);
			currency = std::get<2>(*cashflow)->getIsoCode();
		}
		catch (exception& e)
		{
			string msg = string(e.what());
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, msg.c_str());
			return NULL;
		}

		// Now we pack the values in a Python tuple
		PyObject* result = PyTuple_New(3);
		int success;

		success = PyTuple_SetItem(result, 0, PyString_FromString(endDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "endDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 1, PyFloat_FromDouble(nominal));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "nominal");
			return NULL;
		}

		success = PyTuple_SetItem(result, 2, PyString_FromString(currency.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "currency");
			return NULL;
		}

		return result;
	}

	PyObject* show(std::shared_ptr<qf::SimpleCashflow> cshflwPtr)
	{
		// The types inside the wrapper are:
		// QCDate, double, shared_ptr<QCCurrency>

		qf::SimpleCashflow cshflw = *cshflwPtr;
		return show(cshflw);
	}

	PyObject* show(qf::SimpleMultiCurrencyCashflow cshflw)
	{
		// The types inside the wrapper are:
		// QCDate, double, shared_ptr<QCCurrency>, QCDate, std::string, std::string

		auto cashflow = cshflw.wrap();

		// We will first unpack the wrapper
		std::string endDate;
		double nominal;
		std::string currency;
		std::string fxRateFixingDate;
		std::string settlementCurrency;
		std::string fxRateIndexCode;
		double fxRateIndexValue;
		double amount;
		try
		{
			endDate = std::get<0>(*cashflow).description(false);
			nominal = std::get<1>(*cashflow);
			currency = std::get<2>(*cashflow)->getIsoCode();
			fxRateFixingDate = std::get<3>(*cashflow).description(false);
			settlementCurrency = std::get<4>(*cashflow)->getIsoCode();
			fxRateIndexCode = std::get<5>(*cashflow)->getCode();
			fxRateIndexValue = std::get<6>(*cashflow);
			amount = std::get<7>(*cashflow);
		}
		catch (exception& e)
		{
			string msg = string(e.what());
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, msg.c_str());
			return NULL;
		}

		// Now we pack the values in a Python tuple
		PyObject* result = PyTuple_New(8);
		int success;

		success = PyTuple_SetItem(result, 0, PyString_FromString(endDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "endDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 1, PyFloat_FromDouble(nominal));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "nominal");
			return NULL;
		}

		success = PyTuple_SetItem(result, 2, PyString_FromString(currency.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "currency");
			return NULL;
		}

		success = PyTuple_SetItem(result, 3, PyString_FromString(fxRateFixingDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "fxRateFixingDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 4, PyString_FromString(settlementCurrency.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "settlementCurrency");
			return NULL;
		}

		success = PyTuple_SetItem(result, 5, PyString_FromString(fxRateIndexCode.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "fxRateIndexCode");
			return NULL;
		}

		success = PyTuple_SetItem(result, 6, PyFloat_FromDouble(fxRateIndexValue));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "fxRateIndexValue");
			return NULL;
		}

		success = PyTuple_SetItem(result, 7, PyFloat_FromDouble(amount));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "amount");
			return NULL;
		}

		return result;
	}

	PyObject* show(std::shared_ptr<qf::SimpleMultiCurrencyCashflow> cshflwPtr)
	{
		qf::SimpleMultiCurrencyCashflow cshflw = *cshflwPtr;
		return show(cshflw);
	}

	PyObject* show(qf::IcpClpCashflow cshflw)
	{
		// The types inside the wrapper are:
		//QCDate,                 /* Start Date */
		//QCDate,                 /* End Date */
		//QCDate,                 /* Settlement Date */
		//double,                 /* Nominal */
		//double,                 /* Amortization */
		//bool,                   /* Amortization is cashflow */
		//shared_ptr<QCCurrency>, /* Nominal Currency (always CLP) */
		//double,                 /* Start date ICP value */
		//double,                 /* End date ICP value */
		//double,                 /* Rate */
		//double,                 /* Interest */
		//double,                 /* Spread */
		//double                  /* Gearing */
		
		auto cashflow = cshflw.wrap();

		// We will first unpack the wrapper
		std::string startDate;
		std::string endDate;
		std::string settlementDate;
		double nominal;
		double amortization;
		long doesAmortize = 0;
		double cashflowAmount;
		std::string nominalCurrency;
		double startDateICP;
		double endDateICP;
		double rateValue;
		double interest;
		double spread;
		double gearing;
		try
		{
			startDate =       std::get<0>(*cashflow).description(false);
			endDate =         std::get<1>(*cashflow).description(false);
			settlementDate =  std::get<2>(*cashflow).description(false);
			nominal =         std::get<3>(*cashflow);
			amortization =    std::get<4>(*cashflow);
			if               (std::get<5>(*cashflow))
			{
				doesAmortize = 1;
			}
			cashflowAmount = cshflw.amount();
			nominalCurrency = std::get<6>(*cashflow)->getIsoCode();
			startDateICP =    std::get<7>(*cashflow);
			endDateICP =      std::get<8>(*cashflow);
			rateValue =       std::get<9>(*cashflow);
			interest =        std::get<10>(*cashflow);
			spread =          std::get<11>(*cashflow);
			gearing =         std::get<12>(*cashflow);
		}
		catch (exception& e)
		{
			string msg = string(e.what());
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, msg.c_str());
			return NULL;
		}

		// Now we pack the values in a Python tuple
		PyObject* result = PyTuple_New(15);
		int success;

		success = PyTuple_SetItem(result, 0, PyString_FromString(startDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "startDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 1, PyString_FromString(endDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "endDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 2, PyString_FromString(settlementDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "settlementDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 3, PyFloat_FromDouble(nominal));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "nominal");
			return NULL;
		}

		success = PyTuple_SetItem(result, 4, PyFloat_FromDouble(amortization));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "amortization");
			return NULL;
		}

		success = PyTuple_SetItem(result, 5, PyBool_FromLong(doesAmortize));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "doesAmortize");
			return NULL;
		}

		success = PyTuple_SetItem(result, 6, PyFloat_FromDouble(cashflowAmount));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "cashflowAmount");
			return NULL;
		}

		success = PyTuple_SetItem(result, 7, PyString_FromString(nominalCurrency.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "currency");
			return NULL;
		}

		success = PyTuple_SetItem(result, 8, PyFloat_FromDouble(startDateICP));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "startDateICP");
			return NULL;
		}

		success = PyTuple_SetItem(result, 9, PyFloat_FromDouble(endDateICP));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "endDateICP");
			return NULL;
		}

		success = PyTuple_SetItem(result, 10, PyFloat_FromDouble(rateValue));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "rateValue");
			return NULL;
		}

		success = PyTuple_SetItem(result, 11, PyFloat_FromDouble(interest));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "interest");
			return NULL;
		}

		success = PyTuple_SetItem(result, 12, PyFloat_FromDouble(spread));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "spread");
			return NULL;
		}

		success = PyTuple_SetItem(result, 13, PyFloat_FromDouble(gearing));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "gearing");
			return NULL;
		}

		success = PyTuple_SetItem(result, 14, PyString_FromString("LinAct360"));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "type_of_rate");
			return NULL;
		}


		return result;
	}

	PyObject* show(std::shared_ptr<qf::IcpClpCashflow> cshflwPtr)
	{
		qf::IcpClpCashflow cshflw = *cshflwPtr;
		return show(cshflw);
	}

	PyObject* show(qf::IcpClfCashflow cshflw)
	{
		// The types inside the wrapper are:
		//QCDate,                 /* Start Date */
		//QCDate,                 /* End Date */
		//QCDate,                 /* Settlement Date */
		//double,                 /* Nominal */
		//double,                 /* Amortization */
		//bool,                   /* Amortization is cashflow */
		//shared_ptr<QCCurrency>, /* Nominal Currency (always CLP) */
		//double,                 /* Start date ICP value */
		//double,                 /* End date ICP value */
		//double,                 /* Start date UF value */
		//double,                 /* End date UF value */
		//double,                 /* Rate */
		//double,                 /* Interest */
		//double,                 /* Spread */
		//double                  /* Gearing */

		auto cashflow = cshflw.wrap();

		// We will first unpack the wrapper
		std::string startDate;
		std::string endDate;
		std::string settlementDate;
		double nominal;
		double amortization;
		long doesAmortize = 0;
		double cashflowAmount;
		std::string nominalCurrency;
		double startDateICP;
		double endDateICP;
		double startDateUF;
		double endDateUF;
		double rateValue;
		double interest;
		double spread;
		double gearing;
		try
		{
			startDate = std::get<0>(*cashflow).description(false);
			endDate = std::get<1>(*cashflow).description(false);
			settlementDate = std::get<2>(*cashflow).description(false);
			nominal = std::get<3>(*cashflow);
			amortization = std::get<4>(*cashflow);
			if (std::get<5>(*cashflow))
			{
				doesAmortize = 1;
			}
			cashflowAmount = cshflw.amount();
			nominalCurrency = std::get<6>(*cashflow)->getIsoCode();
			startDateICP = std::get<7>(*cashflow);
			endDateICP = std::get<8>(*cashflow);
			startDateUF = std::get<9>(*cashflow);
			endDateUF = std::get<10>(*cashflow);
			rateValue = std::get<11>(*cashflow);
			interest = std::get<12>(*cashflow);
			spread = std::get<13>(*cashflow);
			gearing = std::get<14>(*cashflow);
		}
		catch (exception& e)
		{
			string msg = string(e.what());
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, msg.c_str());
			return NULL;
		}

		// Now we pack the values in a Python tuple
		PyObject* result = PyTuple_New(17);
		int success;

		success = PyTuple_SetItem(result, 0, PyString_FromString(startDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "startDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 1, PyString_FromString(endDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "endDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 2, PyString_FromString(settlementDate.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "settlementDate");
			return NULL;
		}

		success = PyTuple_SetItem(result, 3, PyFloat_FromDouble(nominal));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "nominal");
			return NULL;
		}

		success = PyTuple_SetItem(result, 4, PyFloat_FromDouble(amortization));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "amortization");
			return NULL;
		}

		success = PyTuple_SetItem(result, 5, PyBool_FromLong(doesAmortize));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "doesAmortize");
			return NULL;
		}

		success = PyTuple_SetItem(result, 6, PyFloat_FromDouble(cashflowAmount));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "cashflowAmount");
			return NULL;
		}

		success = PyTuple_SetItem(result, 7, PyString_FromString(nominalCurrency.c_str()));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "currency");
			return NULL;
		}

		success = PyTuple_SetItem(result, 8, PyFloat_FromDouble(startDateICP));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "startDateICP");
			return NULL;
		}

		success = PyTuple_SetItem(result, 9, PyFloat_FromDouble(endDateICP));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "endDateICP");
			return NULL;
		}

		success = PyTuple_SetItem(result, 10, PyFloat_FromDouble(startDateUF));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "startDateUF");
			return NULL;
		}

		success = PyTuple_SetItem(result, 11, PyFloat_FromDouble(endDateUF));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "endDateUF");
			return NULL;
		}

		success = PyTuple_SetItem(result, 12, PyFloat_FromDouble(rateValue));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "rateValue");
			return NULL;
		}

		success = PyTuple_SetItem(result, 13, PyFloat_FromDouble(interest));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "interest");
			return NULL;
		}

		success = PyTuple_SetItem(result, 14, PyFloat_FromDouble(spread));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "spread");
			return NULL;
		}

		success = PyTuple_SetItem(result, 15, PyFloat_FromDouble(gearing));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "gearing");
			return NULL;
		}

		success = PyTuple_SetItem(result, 16, PyString_FromString("LinAct360"));
		if (success != 0)
		{
			PyObject* qcfError = PyErr_NewException("QC_Financial Error", NULL, NULL);
			PyErr_SetString(qcfError, "type_of_rate");
			return NULL;
		}

		return result;
	}

	PyObject* show(std::shared_ptr<qf::IcpClfCashflow> cshflwPtr)
	{
		qf::IcpClfCashflow cshflw = *cshflwPtr;
		return show(cshflw);
	}

	class QCInterpolatorWrap : public QCInterpolator, public boost::python::wrapper<QCInterpolator>
	{
		double interpolateAt(long value)
		{
			return this->get_override("interpolateAt")();
		}
		
		double derivativeAt(long value)
		{
			return this->get_override("derivativeAt")();
		}

		double secondDerivativeAt(long value)
		{
			return this->get_override("secondDerivativeAt")();
		}
	};

	class InterestRateCurveWrap : public qf::InterestRateCurve,
		                          public boost::python::wrapper<qf::InterestRateCurve>
	{
	public:
		InterestRateCurveWrap(std::shared_ptr<QCInterpolator> curve, QCInterestRate intRate) :
			InterestRateCurve(curve, intRate)
		{
		}

		double getRateAt(long d)
		{
			return this->get_override("getRateAt")();
		}

		double getDiscountFactorAt(long d)
		{
			return this->get_override("getDiscountFactorAt")();
		}

		double getForwardRateWithRate(QCInterestRate& intRate, long d1, long d2)
		{
			return this->get_override("getForwardRateWithRate")();
		}

		double getForwardRate(long d1, long d2)
		{
			return this->get_override("getForwardRate")();
		}

		double getForwardWf(long d1, long d2)
		{
			return this->get_override("getForwardWf")();
		}

		double dfDerivativeAt(unsigned int index)
		{
			return this->get_override("dfDerivativeAt")();
		}

		double fwdWfDerivativeAt(unsigned int index)
		{
			return this->get_override("fwdWfDerivativeAt")();
		}
	};
}


#endif //WRAPPERS_H
