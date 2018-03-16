#ifndef IBORCASHFLOW_H
#define IBORCASHFLOW_H

#include<tuple>
#include<memory>
#include<string>

#include "Cashflow.h"
#include "QCDate.h"
#include "QCInterestRate.h"
#include "QCCurrency.h"
#include "InterestRateIndex.h"

namespace QCode
{
	namespace Financial
	{
		typedef std::tuple<
			QCDate,                  /* Start date */
			QCDate,                  /* End date */
			QCDate,                  /* Fixing date */
			QCDate,                  /* Settlement date */
			double,                  /* Nominal */
			double,                  /* Amortization */
			double,                  /* Interest */
			bool,                    /* Amortization is cashflow */
			double,                  /* Total cashflow */
			shared_ptr<QCCurrency>,  /* Nominal currency */
			std::string,             /* Interest rate index code */
			QCInterestRate,          /* Interest rate */
			double,                  /* Spread */
			double                   /* Gearing */
		> IborCashflowWrapper;

		/**
		 * @class	IborCashflow
		 *
		 * @brief	An Ibor based cashflow.
		 *
		 * @author	Alvaro D�az V.
		 * @date	29/09/2017
		 */
		class IborCashflow : public Cashflow
		{
		public:

			/**
			* @enum	Element
			*
			* @brief	Values that represent the different elements of an IborCashflow
			*/
			enum Element
			{
				///< An enum constant representing the start date
				startDate,

				///< An enum constant representing the end date
				endDate,

				///< An enum constant representing the fixing date
				fixingDate,

				///< An enum constant representing the settlement date
				settlementDate,

				///< An enum constant representing the notional
				notional,

				///< An enum constant representing the amortization
				amortization,

				///< An enum constant representing the interest
				interest,

				///< An enum constant representing the amort is cashflow
				amortIsCashflow,

				///< An enum constant representing the currency
				currency,
				
				///< An enum constant representing the spread
				spread,
				
				///< An enum constant representing the gearing
				gearing
			};

			/**
			 * @fn	IborCashflow::IborCashflow(std::shared_ptr<InterestRateIndex> index,
			 *                                 const QCDate& startDate, const QCDate& endDate,
			 *                                 const QCDate& fixingDate, const QCDate& settlementDate,
			 *                                 double nominal, double amortization, bool doesAmortize,
			 *                                 const QCInterestRate& rate, std::shared_ptr<QCCurrency> currency,
			 *                                 double spread, double gearing);
			 *
			 * @brief	Constructor
			 *
			 * @author	Alvaro D�az V.
			 * @date	29/09/2017
			 *
			 * @param	iborIndexCode 	The associated Ibor code.
			 * @param	startDate	  	The start date.
			 * @param	endDate		  	The end date.
			 * @param	fixingDate	  	The fixing date.
			 * @param	settlementDate	The settlement date.
			 * @param	nominal		  	The nominal.
			 * @param	amortization  	The amortization.
			 * @param	doesAmortize  	Indicates whether amortization is cashflow or not.
			 * @param	rate		  	The rate.
			 * @param	currency	  	The currency.
			 * @param	spread		  	The spread.
			 * @param	gearing		  	The gearing.
			 */
			IborCashflow(std::shared_ptr<InterestRateIndex> index,
						 const QCDate& startDate,
						 const QCDate& endDate,
						 const QCDate& fixingDate,
						 const QCDate& settlementDate,
						 double nominal,
						 double amortization,
						 bool doesAmortize,
						 std::shared_ptr<QCCurrency> currency,
						 double spread,
						 double gearing);

			/**
			 * @fn	double IborCashflow::amount();
			 *
			 * @brief	Gets the amount
			 *
			 * @author	Alvaro D�az V.
			 * @date	29/09/2017
			 *
			 * @return	A double.
			 */
			double amount();

			/**
			 * @fn	shared_ptr<QCCurrency> IborCashflow::ccy();
			 *
			 * @brief	Gets the ccy
			 *
			 * @author	Alvaro D�az V.
			 * @date	29/09/2017
			 *
			 * @return	A shared_ptr&lt;QCCurrency&gt;
			 */
			shared_ptr<QCCurrency> ccy();

			/**
			 * @fn	QCDate IborCashflow::date();
			 *
			 * @brief	Gets the date
			 *
			 * @author	Alvaro D�az V.
			 * @date	29/09/2017
			 *
			 * @return	A QCDate.
			 */
			QCDate date();

			/**
			* @fn	void iborCashflow::setNominal(double nominal);
			*
			* @brief	Sets the nominal amount.
			*
			* @author	Alvaro D�az V.
			* @date	05/10/2017
			*
			* @param	nominal	The nominal.
			*/
			void setNominal(double nominal);

			/**
			* @fn	void iborCashflow::setAmortization(double amortization);
			*
			* @brief	Sets the amortization
			*
			* @author	Alvaro D�az V.
			* @date	05/10/2017
			*
			* @param	amortization	The amortization.
			*/
			void setAmortization(double amortization);

			/**
			* @fn	void iborCashflow::setInterestRateValue(double value);
			*
			* @brief	Sets the rate value of the interest rate index 
			*
			* @author	Alvaro D�az V.
			* @date	    05/12/2017
			*
			* @param	value	The value of the interest rate.
			*/
			void setInterestRateValue(double value);

			/**
			 * @fn	shared_ptr<IborCashflowWrapper> IborCashflow::wrap();
			 *
			 * @brief	Wraps the cashflow in a IborCashflowWrapper
			 *
			 * @author	Alvaro D�az V.
			 * @date	29/09/2017
			 *
			 * @return	A shared_ptr&lt;IborCashflowWrapper&gt;
			 */
			shared_ptr<IborCashflowWrapper> wrap();

			/**
			* @fn	    QCDate IborCashflow::getFixingDate();
			*
			* @brief	Returns the fixing date.
			*
			* @author	Alvaro D�az V.
			* @date	    05/12/2017
			*
			* @return	QCDate;
			*/
			QCDate getFixingDate();

			/**
			 * @fn	virtual IborCashflow::~IborCashflow();
			 *
			 * @brief	Destructor
			 *
			 * @author	Alvaro D�az V.
			 * @date	29/09/2017
			 */
			virtual ~IborCashflow();

		private:

			/**
			 * @fn	void IborCashflow::_calculateInterest();
			 *
			 * @brief	Calculates the interest using (rate*gearing + spread)
			 *
			 * @author	Alvaro D�az V.
			 * @date	29/09/2017
			 */
			void _calculateInterest();

			/** @brief	The associated interest rate index */
			std::shared_ptr<InterestRateIndex> _index;

			/** @brief	The start date */
			QCDate _startDate;

			/** @brief	The end date */  
			QCDate _endDate;

			/** @brief	The fixing date */
			QCDate _fixingDate;

			/** @brief	The settlement date */
			QCDate _settlementDate;

			/** @brief	The nominal */
			double _nominal;

			/** @brief	The amortization */
			double _amortization;

			/** @brief	The interest amount calculated */
			double _interest;

			/** @brief	True if amortization is part of the cashflow */
			bool _doesAmortize;

			/** @brief	The currency */
			shared_ptr<QCCurrency> _currency;

			/** @brief	The spread */
			double _spread;

			/** @brief	The gearing */
			double _gearing;
		};

	}
}

#endif //IBORCASHFLOW_H