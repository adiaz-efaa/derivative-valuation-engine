#ifndef QCDVEPYBINDHELPERFUNCTIONS_H
#define QCDVEPYBINDHELPERFUNCTIONS_H

#include <map>

#include "QC_DVE_PYBIND.h"
#include "time/QCDate.h"
#include "QCInterestRateLeg.h"
#include "QCHelperFunctions.h"

using namespace std;

namespace QCDvePyBindHelperFunctions
{
	typedef tuple<string, string, string, string> string4;
	
	//start_date, end_date, value, yf, wf
	typedef tuple<
		QCDate,
		QCDate,
		double,
		string,
		string
	> ZeroRate;

	enum QCZeroRate
	{
		qcZeroRateStartDate,
		qcZeroRateEndDate,
		qcZeroRateValue,
		qcZeroRateYf,
		qcZeroRateWf
	};
	
	typedef tuple<
		string,								//0		receive or pay
		QCDate,								//1		start_date
		QCDate,								//2		end_date
		unsigned int,						//3		settlement_lag
		QCInterestRateLeg::QCStubPeriod,	//4		stub period
		string,								//5		periodicity
		QCDate::QCBusDayAdjRules,			//6		end_date_adjustment
		QCInterestRateLeg::QCAmortization,	//7		amortization
		double,								//8		rate
		double,								//9		notional
		string,								//10	yf
		string								//11	wf
	> SwapIndex;

	typedef tuple<
		double,								//0 notional1 (strong currency)
		double,								//1 notional2 (weak currency)
		QCDate,								//2 end_date
		QCCurrencyConverter::QCCurrencyEnum,	//3 present value currency
		QCCurrencyConverter::QCCurrencyEnum,	//4 notional1 currency (strong)
		QCCurrencyConverter::QCCurrencyEnum,	//5 notional2 currency (weak)
		QCCurrencyConverter::QCFxRateEnum,		//6 fxRate1 (strong a present value)
		QCCurrencyConverter::QCFxRateEnum		//7 fxRate2 (weak a presente value)
	> FwdIndex;

	enum QCFwdIndex
	{
		qcFwdNotionalStrong,
		qcFwdNotionalWeak,
		qcFwdEndDate,
		qcFwdPresentValueCurr,
		qcFwdStrongCurr,
		qcFwdWeakCurr,
		qcFwdFxRateStrong2PV,
		qcFwdFxRateWeak2PV
	};

	typedef tuple<
		string,								//0		receive or pay
		QCDate,								//1		start_date
		QCDate,								//2		end_date
		unsigned int,						//3		settlement_lag
		QCInterestRateLeg::QCStubPeriod,	//4		stub period
		string,								//5		periodicity
		QCDate::QCBusDayAdjRules,			//6		end_date_adjustment
		string,								//7		fixing periodicity
		QCInterestRateLeg::QCStubPeriod,	//8		fixing stub period
		unsigned int,						//9		fixing lag
		QCInterestRateLeg::QCAmortization,	//10	amortization
		double,								//11	rate
		double,								//12	spread
		double,								//13	notional
		string,								//14	yf
		string								//15	wf
	> FloatIndex;

	enum QCFloatIndex
	{
		qcReceivePay,
		qcStartDate,
		qcEndDate,
		qcSettlementLag,
		qcStubPeriod,
		qcPeriodicity,
		qcEndDateAdjustment,
		qcFixingPeriodicity,
		qcFixingStubPeriod,
		qcFixingLag,
		qcAmortization,
		qcRate,
		qcSpread,
		qcNotional,
		qcYearFraction,
		qcWealthFactor
	};

	typedef tuple<
		long,
		double,
		string,
		string
	> SpreadIndex;

	enum QCSpreadIndex
	{
		qcSpreadMaturity, //en dias
		qcSpreadValue,
		qcSpreadYf,
		qcSpreadWf
	};

	void buildSpreadIndexVector(PyObject* spreadIndex, QCDate valueDate, vector<QCDate>& holidays,
		vector<SpreadIndex>& spreadIndexVector)
	{
		//En spread index viene una lista con tuplas con la siguiente estructura:
		//('TABCLPCAMCLP_2Y', '2Y', 'ACT/360', 'LIN', 0.0111)

		size_t numSpreads = PyList_Size(spreadIndex);
		spreadIndexVector.resize(numSpreads);
		for (size_t i = 0; i < numSpreads; ++i)
		{	
			string tenor{ PyString_AsString(PyTuple_GetItem(PyList_GetItem(spreadIndex, i), 1)) };
			QCDate endDate{ valueDate.addMonths(QCHelperFunctions::tenor(tenor)).
				businessDay(holidays, QCDate::QCBusDayAdjRules::qcFollow) };
			
			SpreadIndex temp;
			
			get<qcSpreadMaturity>(temp) = valueDate.dayDiff(endDate);

			get<qcSpreadValue>(temp) = PyFloat_AsDouble(PyTuple_GetItem(PyList_GetItem(
				spreadIndex, i), 4));
			
			string yf = string(PyString_AsString(PyTuple_GetItem(PyList_GetItem(spreadIndex, i), 2)));
			QCHelperFunctions::lowerCase(yf);
			get<qcSpreadYf>(temp) = yf;
			
			string wf = string(PyString_AsString(PyTuple_GetItem(PyList_GetItem(spreadIndex, i), 3)));
			QCHelperFunctions::lowerCase(wf);
			get<qcSpreadYf>(temp) = wf;

			spreadIndexVector.at(i) = temp;
		}
	}

	void buildAuxTenorsAndRates(PyObject* auxCurveValues, vector<long>& auxTenors, vector<double>& auxRates)
	{
		//cout << "Enter buildAuxTenorsAndRates" << endl;
		size_t numValues = PyList_Size(auxCurveValues);
		//cout << "\tnumValues: " << numValues << endl;
		auxTenors.resize(numValues);
		auxRates.resize(numValues);
		for (size_t i = 0; i < numValues; ++i)
		{
			auxTenors.at(i) = PyInt_AsLong(PyTuple_GetItem(PyList_GetItem(auxCurveValues, i), 2));
			auxRates.at(i) = PyFloat_AsDouble(PyTuple_GetItem(PyList_GetItem(auxCurveValues, i), 3));
		}
	}

	void buildAuxTenorsAndRates(PyObject* auxCurveValues, vector<long>& auxTenors, vector<double>& auxRates,
		string queCurva)
	{
		// cout << "Enter buildAuxTenorsAndRates" << endl;
		size_t numValues = PyList_Size(auxCurveValues);
		// cout << "\tnumValues: " << numValues << endl;
		string name;
		for (size_t i = 0; i < numValues; ++i)
		{
			name = PyString_AsString(PyTuple_GetItem(PyList_GetItem(auxCurveValues, i), 1));
			if (name == queCurva)
			{
				auxTenors.push_back(PyInt_AsLong(PyTuple_GetItem(PyList_GetItem(auxCurveValues, i), 2)));
				//cout << "tenors " << auxTenors.at(i) << endl;
				auxRates.push_back(PyFloat_AsDouble(PyTuple_GetItem(PyList_GetItem(auxCurveValues, i), 3)));
				//cout << "rates " << auxRates.at(i) << endl;
			}
		}
	}

	void buildZeroRateVector(PyObject* input, QCDate& processDate, vector<QCDate>& dateVector,
		vector<ZeroRate>& zeroRateVector)
	{
		//(nombre, start_date_rule, tenor, calendar, yf, wf, valor)
		size_t numRates = PyList_Size(input);
		zeroRateVector.resize(numRates);
		// cout << "Enter buildZeroRateVector" << endl;
		// cout << "Process date: " << processDate.description() << endl;
		for (size_t i = 0; i < numRates; ++i)
		{
			ZeroRate temp;

			// cout << "\t" << PyString_AsString(PyTuple_GetItem((PyList_GetItem(input, i)), 0)) << endl;
			//Build start date
			string rule = PyString_AsString(PyTuple_GetItem((PyList_GetItem(input, i)), 1));
			unsigned int lag = QCHelperFunctions::tenor(rule);
			QCHelperFunctions::lowerCase(rule);
			if (rule.substr(rule.size() - 1, 1) == "w")
			{
				get<0>(temp) = processDate.addWeeks(dateVector, lag, QCDate::QCBusDayAdjRules::qcFollow);
			}
			else
			{
				get<0>(temp) = processDate.shift(dateVector, lag, QCDate::QCBusDayAdjRules::qcFollow);
			}
			// cout << "\tlag: " << lag << endl;
			// cout << "\tbuildZeroRateVector: start_date " + get<0>(temp).description() << endl;

			//Build end date
			rule = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 2));
			lag = QCHelperFunctions::tenor(rule);
			QCHelperFunctions::lowerCase(rule);
			// cout << "\tend date rule: " << rule << endl;;
			if (rule.substr(rule.size() - 1, 1) == "d")
			{
				lag = QCHelperFunctions::tenor(rule);
				get<1>(temp) = get<0>(temp).addDays(lag).businessDay(dateVector,
					QCDate::QCBusDayAdjRules::qcFollow);
			}
			else if (rule.substr(rule.size() - 1, 1) == "w")
			{
				lag = QCHelperFunctions::tenor(rule);
				get<1>(temp) = get<0>(temp).addWeeks(dateVector, lag, QCDate::QCBusDayAdjRules::qcFollow);
			}
			else
			{
				lag = QCHelperFunctions::tenor(rule);
				get<1>(temp) = get<0>(temp).addMonths(lag).businessDay(dateVector,
					QCDate::QCBusDayAdjRules::qcFollow);
			}
			
			// cout << "\tlag: " << lag << endl;
			// cout << "\tbuildZeroRateVector: end_date " + get<1>(temp).description() << endl;

			//Build value
			get<2>(temp) = PyFloat_AsDouble(PyTuple_GetItem(PyList_GetItem(input, i), 6));
			// cout << "\tbuildZeroRateVector: rate " << get<2>(temp) << endl;
			
			//Build yf
			string tempStr = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 4));
			QCHelperFunctions::lowerCase(tempStr);
			get<3>(temp) = tempStr;
			// cout << "\tbuildZeroRateVector: yf " << get<3>(temp) << endl;

			//Build wf
			tempStr = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 5));
			QCHelperFunctions::lowerCase(tempStr);
			get<4>(temp) = tempStr;
			// cout << "\tbuildZeroRateVector: wf " << get<4>(temp) << endl;

			//Agrega temp al vector resultado
			zeroRateVector.at(i) = temp;
		}
	}

	void buildFwdVector(PyObject* input, vector<FwdIndex>& fwdIndexVector)
	{
		size_t numFwds = PyList_Size(input);
		fwdIndexVector.resize(numFwds);
		QCCurrencyConverter conv;
		for (size_t i = 0; i < numFwds; ++i)
		{
			FwdIndex temp;
			get<0>(temp) = 1.0;
			get<1>(temp) = PyFloat_AsDouble(PyTuple_GetItem(PyList_GetItem(input, i), 1));
            auto stringDate = string(PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 2)));
            get<2>(temp) = QCDate{stringDate};
			get<3>(temp) = conv.getWeakCurrencyEnum(
				string(PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 3))));
			get<4>(temp) = conv.getStrongCurrencyEnum(
				string(PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 3))));
			get<5>(temp) = get<3>(temp);
			get<6>(temp) = conv.getStandardFxRate(get<4>(temp), get<3>(temp));
			get<7>(temp) = conv.getStandardFxRate(get<3>(temp), get<3>(temp));
			fwdIndexVector.at(i) = temp;
		}
	}

	void buildFwdVector(PyObject* input, vector<FwdIndex>& fwdIndexVector, double fx)
	{
		//('FwdUSDCLP12M', 11.95, '2018-04-02', 'USDCLP', 'FWDPOINTS') esto entra
		// cout << "Enter buildFwdVectorFX" << endl;
		size_t numFwds = PyList_Size(input);
		fwdIndexVector.resize(numFwds);
		QCCurrencyConverter conv;
		for (size_t i = 0; i < numFwds; ++i)
		{
			FwdIndex temp;
			get<qcFwdNotionalStrong>(temp) = 1.0;
			// cout << "buildFwdVector: strong notional " << get<qcFwdNotionalStrong>(temp) << endl;
			if (string(PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 4))) == "FWDPOINTS")
			{
				get<qcFwdNotionalWeak>(temp) = fx + PyFloat_AsDouble(PyTuple_GetItem(
					PyList_GetItem(input, i), 1));
				// cout << "buildFwdVector: weak notional " << get<qcFwdNotionalWeak>(temp) << endl;
			}
			else
			{
				get<qcFwdNotionalWeak>(temp) = PyFloat_AsDouble(PyTuple_GetItem(PyList_GetItem(input, i), 1));
				// cout << "buildFwdVector: weak notional " << get<qcFwdNotionalWeak>(temp) << endl;
			}
            auto stringDate = string(PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 2)));
            get<qcFwdEndDate>(temp) = QCDate{stringDate};
			// cout << "buildFwdVector: end date " << get<qcFwdEndDate>(temp).description() << endl;
			
			get<qcFwdPresentValueCurr>(temp) = conv.getWeakCurrencyEnum(
				string(PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 3))));
			// cout << "buildFwdVector: pv currency " << get<qcFwdPresentValueCurr>(temp) << endl;
			
			get<qcFwdStrongCurr>(temp) = conv.getStrongCurrencyEnum(
				string(PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 3))));
			// cout << "buildFwdVector: strong currency " << get<qcFwdStrongCurr>(temp) << endl;
			
			get<qcFwdWeakCurr>(temp) = get<qcFwdPresentValueCurr>(temp);
			// cout << "buildFwdVector: weak currency " << get<qcFwdWeakCurr>(temp) << endl;
			
			get<qcFwdFxRateStrong2PV>(temp) = conv.getStandardFxRate(get<qcFwdStrongCurr>(temp),
				get<qcFwdWeakCurr>(temp));
			// cout << "buildFwdVector: fx  strong 2 pv " << get<qcFwdFxRateStrong2PV>(temp) << endl;
			
			get<qcFwdFxRateWeak2PV>(temp) = conv.getStandardFxRate(get<qcFwdWeakCurr>(temp),
				get<qcFwdWeakCurr>(temp));
			// cout << "buildFwdVector: fx weak 2 pv " << get<qcFwdFxRateWeak2PV>(temp) << endl << endl;
			
			fwdIndexVector.at(i) = temp;
		}
	}

	void buildFixedRateIndexVector(PyObject* input, QCDate& processDate, vector<QCDate>& dateVector,
		vector<SwapIndex>& swapIndexVector)
	{
		//Esto es un ejemplo de lo que entra:
		//('SwapCamCLP9M' 0, False 1, 2 (start_date_lag) 2, '9M' 3, None (start_date) 4, None (end_date) 5,
		//'SCL' 6, 0L 7, 'CORTO INICIO' 8, '2Y' 9, 'FOLLOW' 10, 'BULLET' 11, 1.0 12, 'LIN' 13, 
		//'ACT/360' 14, 'CAMARACLP' 15, 0.0291 16)

		//Se quiere obtener
		//string (R) 0, QCDate (start) 1, QCDate (end) 2, unsigned int (sett lag) 3, enum stub 4,
		//string (periodicity) 5, enum follow 6, enum amort 7, double (rate) 8, double (notional) 9,
		//enum wf 10, enum yf 11
		size_t numSwaps = PyList_Size(input);
		swapIndexVector.resize(numSwaps);
		// cout << "buildFixedRateIndexVector" << endl;
		for (size_t i = 0; i < numSwaps; ++i)
		{
			SwapIndex temp;
			// cout << endl;
			
			//Receive or pay
			get<0>(temp) = "R";
			// cout << "\tbuildFixedRateIndexVector: receive or pay " << get<0>(temp) << endl;

			//Build start date (start_date_lag 2)
			unsigned int lag = PyInt_AsLong(PyTuple_GetItem(PyList_GetItem(input, i), 2));
			get<1>(temp) = processDate.shift(dateVector, lag, QCDate::QCBusDayAdjRules::qcFollow);
			// cout << "\tbuildFixedRateIndexVector: start date " << get<1>(temp).description() << endl;

			//Build end date (5)
			long condition =  PyInt_AsLong(PyTuple_GetItem(PyList_GetItem(input, i), 1));
			if (condition)
			{
				string tempDate = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 5));
				get<2>(temp) = QCDate{ tempDate };
				// cout << "\tbuildFixedRateIndexVector: end date " << get<2>(temp).description() << endl;
			}
			else
			{
				unsigned int m = QCHelperFunctions::tenor(
					PyString_AsString(
					PyTuple_GetItem(
					PyList_GetItem(input, i), 3)));
				get<2>(temp) = get<1>(temp).addMonths(m).businessDay(dateVector, QCDate::QCBusDayAdjRules::qcNo);
				// cout << "\tbuildFixedRateIndexVector: end date " << get<2>(temp).description() << endl;
			}

			//Build settlement lag (7)
			get<3>(temp) = PyInt_AsLong(PyTuple_GetItem(PyList_GetItem(input, i), 7));
			// cout << "\tbuildFixedRateIndexVector: settlement lag " << get<3>(temp) << endl;

			//Build enum stub (8)
			string stub = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 8));
			get<4>(temp) = QCHelperFunctions::stringToQCStubPeriod(stub);
			// cout << "\tbuildFixedRateIndexVector: enum stub " << get<4>(temp) << endl;

			//Build periodicity (9)
			get<5>(temp) = string(PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 9)));
			// cout << "\tbuildFixedRateIndexVector: periodicity " << get<5>(temp) << endl;

			//Build enum end date adjustment (10)
			string endDatAdj = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 10));
			get<6>(temp) = QCHelperFunctions::stringToQCBusDayAdjRule(endDatAdj);
			// cout << "\tbuildFixedRateIndexVector: enum end date adj " << get<6>(temp) << endl;

			//Build enum amort (11)
			string amort = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 11));
			get<7>(temp) = QCHelperFunctions::stringToQCAmortization(amort);
			// cout << "\tbuildFixedRateIndexVector: enum amort " << get<7>(temp) << endl;
			 
			//Build rate (16)
			get<8>(temp) = PyFloat_AsDouble(PyTuple_GetItem(PyList_GetItem(input, i), 16));
			// cout << "\tbuildFixedRateIndexVector: rate " << get<8>(temp) << endl;

			//Build notional (12)
			get<9>(temp) = PyFloat_AsDouble(PyTuple_GetItem(PyList_GetItem(input, i), 12));
			// cout << "\tbuildFixedRateIndexVector: notional " << get<9>(temp) << endl;

			//Build enum wf (14)
			string yf = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 14));
			QCHelperFunctions::lowerCase(yf);
			get<10>(temp) = yf;
			// cout << "\tbuildFixedRateIndexVector: enum yf " << get<10>(temp) << endl;

			//Build enum yf (13)
			string wf = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 13));
			QCHelperFunctions::lowerCase(wf);
			get<11>(temp) = wf;
			// cout << "\tbuildFixedRateIndexVector: enum wf " << get<11>(temp) << endl;

			swapIndexVector.at(i) = temp;
		}
	}

	void buildFloatingRateIndexVector(PyObject* input, QCDate& processDate, vector<QCDate>& dateVector,
		vector<FloatIndex>& floatIndexVector, string receivePay = "R")
	{
		// cout << "Enter buildFloatingRateIndexVector" << endl;
		//Esto es un ejemplo de lo que entra:
		//('BasisCamCLPLibor3MUSD10Y' 0, False 1, 2 2, '10Y' 3, None 4, None 5, 'NEW YORK' 6,
		//0 7, 'CORTO INICIO' 8, '3M' 9, 'FOLLOW' 10, 'BULLET' 11, 0.0 12, 0.0 13, '3M' 14,
		//'CORTO INICIO' 15, 'LONDON' 16, 2 17, 1.0 18, 'LIN' 19, '30/360' 20,
		//'CLD' 21, 'LIBORUSD3MCLASSIC' 22, 0.006 23)
		 
		//Se quiere obtener:
		//string (0) receive or pay, QCDate (1) start_date, QCDate (2) end_date,
		//unsigned int (3) settlement_lag, QCInterestRateLeg::QCStubPeriod (4) stub period
		//string (5) periodicity, QCDate::QCBusDayAdjRules (6) end_date_adjustment
		//string (7) fixing periodicity, QCInterestRateLeg::QCStubPeriod (8) fixing stub period
		//unsigned int (9) fixing lag, string (10) fixing calendar,
		//QCInterestRateLeg::QCAmortization (10) amortization, double (11) rate,
		//double (12) spread, double (13) notional, string (14) yf, string (15) wf

		size_t numSwaps = PyList_Size(input);
		floatIndexVector.resize(numSwaps);
		for (size_t i = 0; i < numSwaps; ++i)
		{
			FloatIndex temp;
			// cout << endl;
			
			size_t numFields = PyTuple_Size(PyList_GetItem(input, i));

			//Receive or pay
			get<qcReceivePay>(temp) = receivePay;
			// cout << "\tbuildFloatingRateIndexVector: receive or pay " << get<0>(temp) << endl;

			//Build start date (start_date_lag 2)
			unsigned int lag = PyInt_AsLong(PyTuple_GetItem(PyList_GetItem(input, i), 2));
			get<qcStartDate>(temp) = processDate.shift(dateVector, lag, QCDate::QCBusDayAdjRules::qcFollow);
			// cout << "\tbuildFloatingRateIndexVector: start date " << get<qcStartDate>(temp).description() << endl;

			//Build end date (5)
			long condition = PyInt_AsLong(PyTuple_GetItem(PyList_GetItem(input, i), 1));
			if (condition)
			{
				string tempDate = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 5));
				get<qcEndDate>(temp) = QCDate{ tempDate };
				// cout << "\tbuildFloatingRateIndexVector: end date " << get<qcEndDate>(temp).description() << endl;
			}
			else
			{
				unsigned int m = QCHelperFunctions::tenor(
					PyString_AsString(
					PyTuple_GetItem(
					PyList_GetItem(input, i), 3)));
				get<qcEndDate>(temp) = get<1>(temp).addMonths(m).businessDay(dateVector, QCDate::QCBusDayAdjRules::qcFollow);
				// cout << "\tbuildFloatingRateIndexVector: end date " << get<qcEndDate>(temp).description() << endl;
			}

			//Build settlement lag (7)
			get<qcSettlementLag>(temp) = PyInt_AsLong(PyTuple_GetItem(PyList_GetItem(input, i), 7));
			// cout << "\tbuildFloatingRateIndexVector: settlement lag " << get<qcSettlementLag>(temp) << endl;

			//Build enum stub (8)
			string stub = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 8));
			get<qcStubPeriod>(temp) = QCHelperFunctions::stringToQCStubPeriod(stub);
			// cout << "\tbuildFloatingRateIndexVector: enum stub " << get<qcStubPeriod>(temp) << endl;

			//Build periodicity (9)
			get<qcPeriodicity>(temp) = string(PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 9)));
			// cout << "\tbuildFloatingRateIndexVector: periodicity " << get<qcPeriodicity>(temp) << endl;

			//Build enum end date adjustment (10)
			string endDatAdj = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 10));
			get<qcEndDateAdjustment>(temp) = QCHelperFunctions::stringToQCBusDayAdjRule(endDatAdj);
			// cout << "\tbuildFloatingRateIndexVector: enum end date adj " << get<qcEndDateAdjustment>(temp) << endl;
			
			//Build fixing periodicity (14)
			get<qcFixingPeriodicity>(temp) = string(PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 14)));
			// cout << "\tbuildFloatingRateIndexVector: fixing periodicity " << get<qcFixingPeriodicity>(temp) << endl;

			//Build fixing stub period (15) 
			string fixingStub = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 15));
			get<qcFixingStubPeriod>(temp) = QCHelperFunctions::stringToQCStubPeriod(fixingStub);
			// cout << "\tbuildFloatingRateIndexVector: enum fixing stub " << get<qcFixingStubPeriod>(temp) << endl;

			//Build fixing lag (17)
			get<qcFixingLag>(temp) = PyInt_AsLong(PyTuple_GetItem(PyList_GetItem(input, i), 17));
			// cout << "\tbuildFloatingRateIndexVector: fixing lag " << get<qcFixingLag>(temp) << endl;

			//Build enum amort (11)
			string amort = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 11));
			get<qcAmortization>(temp) = QCHelperFunctions::stringToQCAmortization(amort);
			// cout << "\tbuildFloatingRateIndexVector: enum amort " << get<qcAmortization>(temp) << endl;

			//Build rate (12)
			get<qcRate>(temp) = PyFloat_AsDouble(PyTuple_GetItem(PyList_GetItem(input, i), 12));
			// cout << "\tbuildFloatingRateIndexVector: rate " << get<qcRate>(temp) << endl;

			//Build spread (24)
			if (numFields == 25)
			{
				get<qcSpread>(temp) = PyFloat_AsDouble(PyTuple_GetItem(PyList_GetItem(input, i), 24));
			}
			else
			{
				get<qcSpread>(temp) = 0.0;
			}
			// cout << "\tbuildFloatingRateIndexVector: spread " << get<qcSpread>(temp) << endl;

			//Build notional (18)
			get<qcNotional>(temp) = PyFloat_AsDouble(PyTuple_GetItem(PyList_GetItem(input, i), 18));
			// cout << "\tbuildFloatingRateIndexVector: notional " << get<qcNotional>(temp) << endl;

			//Build enum wf (19)
			string wf = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 19));
			QCHelperFunctions::lowerCase(wf);
			get<qcWealthFactor>(temp) = wf;
			// cout << "\tbuildFloatingRateIndexVector: enum wf " << get<qcWealthFactor>(temp) << endl;

			//Build enum yf (20)
			string yf = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 20));
			QCHelperFunctions::lowerCase(yf);
			get<qcYearFraction>(temp) = yf;
			// cout << "\tbuildFloatingRateIndexVector: enum yf " << get<qcYearFraction>(temp) << endl;

			floatIndexVector.at(i) = temp;
		}
	}

	void buildFloatingRateIndexVector2(PyObject* input, QCDate& processDate, vector<QCDate>& dateVector,
		vector<FloatIndex>& floatIndexVector)
	{
		// cout << "Enter buildFloatingRateIndexVector" << endl;
		//Esto es un ejemplo de lo que entra:
		//('BasisCamCLPLibor3MUSD10Y' 0, False 1, 2 2, '10Y' 3, None 4, None 5, 'NEW YORK' 6,
		//0 7, 'CORTO INICIO' 8, '3M' 9, 'FOLLOW' 10, 'BULLET' 11, 0.0 12, 0.0 13, '3M' 14,
		//'CORTO INICIO' 15, 'LONDON' 16, 2 17, 1.0 18, 'LIN' 19, '30/360' 20,
		//'CLD' 21, 'LIBORUSD3MCLASSIC' 22, 0.006 23)

		//Se quiere obtener:
		//string (0) receive or pay, QCDate (1) start_date, QCDate (2) end_date,
		//unsigned int (3) settlement_lag, QCInterestRateLeg::QCStubPeriod (4) stub period
		//string (5) periodicity, QCDate::QCBusDayAdjRules (6) end_date_adjustment
		//string (7) fixing periodicity, QCInterestRateLeg::QCStubPeriod (8) fixing stub period
		//unsigned int (9) fixing lag, string (10) fixing calendar,
		//QCInterestRateLeg::QCAmortization (10) amortization, double (11) rate,
		//double (12) spread, double (13) notional, string (14) yf, string (15) wf

		size_t numSwaps = PyList_Size(input);
		floatIndexVector.resize(numSwaps);
		for (size_t i = 0; i < numSwaps; ++i)
		{
			FloatIndex temp;
			// cout << endl;

			size_t numFields = PyTuple_Size(PyList_GetItem(input, i));
			
			string receivePay = "P";
			
			//Receive or pay
			get<qcReceivePay>(temp) = receivePay;
			// cout << "\tbuildFloatingRateIndexVector: receive or pay " << get<0>(temp) << endl;

			//Build start date (start_date_lag 2)
			unsigned int lag = PyInt_AsLong(PyTuple_GetItem(PyList_GetItem(input, i), 2));
			get<qcStartDate>(temp) = processDate.shift(dateVector, lag, QCDate::QCBusDayAdjRules::qcFollow);
			// cout << "\tbuildFloatingRateIndexVector: start date " << get<qcStartDate>(temp).description() << endl;

			//Build end date (5)
			long condition = PyInt_AsLong(PyTuple_GetItem(PyList_GetItem(input, i), 1));
			if (condition)
			{
				string tempDate = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 5));
				get<qcEndDate>(temp) = QCDate{ tempDate };
				// cout << "\tbuildFloatingRateIndexVector: end date " << get<qcEndDate>(temp).description() << endl;
			}
			else
			{
				unsigned int m = QCHelperFunctions::tenor(
					PyString_AsString(
					PyTuple_GetItem(
					PyList_GetItem(input, i), 3)));
				get<qcEndDate>(temp) = get<1>(temp).addMonths(m).businessDay(dateVector, QCDate::QCBusDayAdjRules::qcFollow);
				// cout << "\tbuildFloatingRateIndexVector: end date " << get<qcEndDate>(temp).description() << endl;
			}

			//Build settlement lag (7)
			get<qcSettlementLag>(temp) = PyInt_AsLong(PyTuple_GetItem(PyList_GetItem(input, i), 7));
			// cout << "\tbuildFloatingRateIndexVector: settlement lag " << get<qcSettlementLag>(temp) << endl;

			//Build enum stub (8)
			string stub = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 8));
			get<qcStubPeriod>(temp) = QCHelperFunctions::stringToQCStubPeriod(stub);
			// cout << "\tbuildFloatingRateIndexVector: enum stub " << get<qcStubPeriod>(temp) << endl;

			//Build periodicity (9)
			get<qcPeriodicity>(temp) = string(PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 9)));
			// cout << "\tbuildFloatingRateIndexVector: periodicity " << get<qcPeriodicity>(temp) << endl;

			//Build enum end date adjustment (10)
			string endDatAdj = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 10));
			get<qcEndDateAdjustment>(temp) = QCHelperFunctions::stringToQCBusDayAdjRule(endDatAdj);
			// cout << "\tbuildFloatingRateIndexVector: enum end date adj " << get<qcEndDateAdjustment>(temp) << endl;

			//Build fixing periodicity (14)
			get<qcFixingPeriodicity>(temp) = string(PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 14)));
			// cout << "\tbuildFloatingRateIndexVector: fixing periodicity " << get<qcFixingPeriodicity>(temp) << endl;

			//Build fixing stub period (15) 
			string fixingStub = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 15));
			get<qcFixingStubPeriod>(temp) = QCHelperFunctions::stringToQCStubPeriod(fixingStub);
			// cout << "\tbuildFloatingRateIndexVector: enum fixing stub " << get<qcFixingStubPeriod>(temp) << endl;

			//Build fixing lag (17)
			get<qcFixingLag>(temp) = PyInt_AsLong(PyTuple_GetItem(PyList_GetItem(input, i), 17));
			// cout << "\tbuildFloatingRateIndexVector: fixing lag " << get<qcFixingLag>(temp) << endl;

			//Build enum amort (11)
			string amort = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 11));
			get<qcAmortization>(temp) = QCHelperFunctions::stringToQCAmortization(amort);
			// cout << "\tbuildFloatingRateIndexVector: enum amort " << get<qcAmortization>(temp) << endl;

			//Build rate (12)
			get<qcRate>(temp) = PyFloat_AsDouble(PyTuple_GetItem(PyList_GetItem(input, i), 12));
			// cout << "\tbuildFloatingRateIndexVector: rate " << get<qcRate>(temp) << endl;

			//Build spread (24)
			if (numFields == 25)
			{
				get<qcSpread>(temp) = PyFloat_AsDouble(PyTuple_GetItem(PyList_GetItem(input, i), 24));
			}
			else
			{
				get<qcSpread>(temp) = 0.0;
			}
			// cout << "\tbuildFloatingRateIndexVector: spread " << get<qcSpread>(temp) << endl;

			//Build notional (18)
			get<qcNotional>(temp) = PyFloat_AsDouble(PyTuple_GetItem(PyList_GetItem(input, i), 18));
			// cout << "\tbuildFloatingRateIndexVector: notional " << get<qcNotional>(temp) << endl;

			//Build enum wf (19)
			string wf = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 19));
			QCHelperFunctions::lowerCase(wf);
			get<qcWealthFactor>(temp) = wf;
			// cout << "\tbuildFloatingRateIndexVector: enum wf " << get<qcWealthFactor>(temp) << endl;

			//Build enum yf (20)
			string yf = PyString_AsString(PyTuple_GetItem(PyList_GetItem(input, i), 20));
			QCHelperFunctions::lowerCase(yf);
			get<qcYearFraction>(temp) = yf;
			// cout << "\tbuildFloatingRateIndexVector: enum yf " << get<qcYearFraction>(temp) << endl;

			floatIndexVector.at(i) = temp;
		}
	}

	void buildQCDateVector(PyObject* input, vector<QCDate>& dateVector)
	{
		size_t sizeInput = PyList_Size(input);
		dateVector.resize(sizeInput);
		for (size_t i = 0; i < sizeInput; ++i)
		{
            auto stringDate = string(PyString_AsString(PyList_GetItem(input, 0)));
            QCDate tempDate{stringDate};
			dateVector.at(i) = tempDate;
		}
	}

	void buildHolidays(PyObject* holidays, map<string, vector<QCDate>>& mapHolidays)
	{
		string lastName, nextName;
		lastName = PyString_AsString(PyList_GetItem(PyList_GetItem(holidays, 0), 0));
		vector<QCDate> temp;
		for (long i = 0; i < PyList_Size(holidays); ++i)
		{
			nextName = PyString_AsString(PyList_GetItem(PyList_GetItem(holidays, i), 0));
			if (nextName == lastName)
			{
                auto stringDate = string(PyString_AsString(
                        PyList_GetItem(PyList_GetItem(holidays, i), 1)));
                temp.push_back(QCDate{stringDate});
				lastName = nextName;
			}
			else
			{
				mapHolidays.insert(pair<string, vector<QCDate>>{lastName, temp});
				temp.clear();
                auto stringDate = string(PyString_AsString(
                        PyList_GetItem(PyList_GetItem(holidays, i), 1)));
                temp.push_back(QCDate{stringDate});
				lastName = nextName;
			}
		}
		mapHolidays.insert(pair<string, vector<QCDate>>{lastName, temp});
		return;
	}

	void buildCurveValues(PyObject* curveValues, map<string, pair<vector<long>, vector<double>>>& crvValues)
	{
		string lastName, nextName;
		lastName = PyString_AsString(PyList_GetItem(PyList_GetItem(curveValues, 0), 0));
		vector<long> tenors;
		vector<double> values;
		for (long i = 0; i < PyList_Size(curveValues); ++i)
		{
			nextName = PyString_AsString(PyList_GetItem(PyList_GetItem(curveValues, i), 0));
			if (nextName == lastName)
			{
				tenors.push_back(PyInt_AsLong(PyList_GetItem(PyList_GetItem(curveValues, i), 1)));
				values.push_back(PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(curveValues, i), 2)));
				lastName = nextName;
			}
			else
			{
				crvValues.insert(pair<string, pair<vector<long>, vector<double>>>{lastName, { tenors, values }});
				tenors.clear();
				values.clear();
				tenors.push_back(PyInt_AsLong(PyList_GetItem(PyList_GetItem(curveValues, i), 1)));
				values.push_back(PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(curveValues, i), 2)));
				lastName = nextName;
			}
		}
		crvValues.insert(pair<string, pair<vector<long>, vector<double>>>{lastName, { tenors, values }});
	}

	void buildCurveCharacteristics(PyObject* curveChars,
		map<string, QCDvePyBindHelperFunctions::string4>& crvChars)
	{
		for (int i = 0; i < PyList_Size(curveChars); ++i)
		{
			string4 temp = make_tuple(PyString_AsString(PyList_GetItem(PyList_GetItem(curveChars, i), 1)),
				PyString_AsString(PyList_GetItem(PyList_GetItem(curveChars, i), 2)),
				PyString_AsString(PyList_GetItem(PyList_GetItem(curveChars, i), 3)),
				PyString_AsString(PyList_GetItem(PyList_GetItem(curveChars, i), 4)));
			crvChars.insert(pair<string, string4>(
				PyString_AsString(PyList_GetItem(PyList_GetItem(curveChars, i), 0)), temp));
		}
	}

	void buildFxRateIndexFxRate(PyObject* fxRateChars,
		map<string, string>& fxRateIndexFxRate)
	{
		for (int i = 0; i < PyList_Size(fxRateChars); ++i)
		{
			fxRateIndexFxRate.insert(pair<string, string>(
				PyString_AsString(PyList_GetItem(PyList_GetItem(fxRateChars, i), 0)),
				PyString_AsString(PyList_GetItem(PyList_GetItem(fxRateChars, i), 1))));
		}
	}

	void buildCustomAmortization(PyObject* customAmort,
		map<unsigned long, vector<tuple<QCDate, double, double>>>& dateNotionalAndAmortByIdLeg)
	{
		unsigned long lastIdLeg = PyInt_AsLong(PyList_GetItem(PyList_GetItem(customAmort, 0), 0));
		QCDate tempDate;
		double tempAmort;
		double tempNotional;
		tuple<QCDate, double, double> tempValue;
		vector<tuple<QCDate, double, double>> tempVector;
		for (unsigned long i = 0; i < PyList_Size(customAmort); ++i)
		{
			unsigned long idLeg = PyInt_AsLong(PyList_GetItem(PyList_GetItem(customAmort, i), 0));;
            auto stringDate = string(PyString_AsString(PyList_GetItem(PyList_GetItem(customAmort, i), 1)));
            tempDate = QCDate{stringDate};
			tempAmort = PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(customAmort, i), 2));
			tempNotional = PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(customAmort, i), 3));
			tempValue = make_tuple(tempDate, tempAmort, tempNotional);
			if (idLeg == lastIdLeg)
			{
				tempVector.push_back(tempValue);
				lastIdLeg = idLeg;
			}
			else
			{
				dateNotionalAndAmortByIdLeg.insert(pair<unsigned long, vector<tuple<QCDate, double, double>>>
					(lastIdLeg, tempVector));
				tempVector.clear();
				tempVector.push_back(tempValue);
				lastIdLeg = idLeg;
			}
		}

		//Insertamos en el ultimo vector en el mapa 
		dateNotionalAndAmortByIdLeg.insert(pair<unsigned long, vector<tuple<QCDate, double, double>>>
			(lastIdLeg, tempVector));
		tempVector.clear();

	}

	void buildFixings(PyObject* fixings, map<QCDate, double>& mapFixings)
	{
		for (unsigned long i = 0; i < PyList_Size(fixings); ++i)
		{
            auto stringDate = string(
                    PyString_AsString(PyList_GetItem(PyList_GetItem(fixings, i), 0)));
            mapFixings.insert(pair<QCDate, double>(QCDate{stringDate},
                                                   PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(fixings, i), 1))));
		}
	}

	void buildManyFixings(PyObject* fixings, map<string, map<QCDate, double>>& mapManyFixings)
	{
		// La list de fixings debe venir ordenada por nombre
		string lastName, nextName;
		lastName = PyString_AsString(PyList_GetItem(PyList_GetItem(fixings, 0), 0));
		map<QCDate, double> tempMap;
		unsigned long numRows = PyList_Size(fixings);
		for (unsigned long i = 0; i < numRows; ++i)
		{
			nextName = PyString_AsString(PyList_GetItem(PyList_GetItem(fixings, i), 0));
            auto stringDate = string(PyString_AsString(PyList_GetItem(PyList_GetItem(fixings, i), 1)));
            if (nextName == lastName)
			{
				tempMap.insert(pair<QCDate, double>(QCDate{stringDate},
					PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(fixings, i), 2))));
				lastName = nextName;
			}
			else
			{
				mapManyFixings.insert(pair<string, map<QCDate, double>>(lastName, tempMap));
				tempMap.clear();
				tempMap.insert(pair<QCDate, double>(QCDate{stringDate},
					PyFloat_AsDouble(PyList_GetItem(PyList_GetItem(fixings, i), 2))));
				lastName = nextName;
			}
		}
		//Un insert final
		mapManyFixings.insert(pair<string, map<QCDate, double>>(lastName, tempMap));
	}

	void buildStringPairStringMap(PyObject* stringMatrix, map<string, pair<string, string>>& mapStringPairString)
	{
		for (unsigned long i = 0; i < PyList_Size(stringMatrix); ++i)
		{
			mapStringPairString.insert(pair<string, pair<string, string>>{
				string(PyString_AsString(PyList_GetItem(PyList_GetItem(stringMatrix, i), 0))),
					make_pair(string(PyString_AsString(PyList_GetItem(PyList_GetItem(stringMatrix, i), 1))),
					string(PyString_AsString(PyList_GetItem(PyList_GetItem(stringMatrix, i), 2))))});
		}
	}
}

#endif //QCDVEPYBINDHELPERFUNCTIONS_H