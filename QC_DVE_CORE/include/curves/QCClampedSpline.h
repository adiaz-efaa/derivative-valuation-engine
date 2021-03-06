#ifndef QCCLAMPEDSPLINE_H
#define QCCLAMPEDSPLINE_H

#include "curves/QCCurve.h"
#include "QCInterpolator.h"

class QCClampedSpline : public QCInterpolator
{
public:
    /*!
    * Constructor
    * @param shared_ptr<QCCurve<long>> curva. Objeto que contiene las abscisas y ordenadas de la curva
    */
	QCClampedSpline(shared_ptr<QCCurve<long>> curva) : QCInterpolator(curva)
	{
		setCoefficients();
	};

	//Methods
	double interpolateAt(long value) override;
	double derivativeAt(long value) override;
	double secondDerivativeAt(long value) override;

	//Destructor
	~QCClampedSpline(void);

private:
	vector<double> _b, _c, _d;		//Coeficientes para el spline
	void setCoefficients(void);		//Calcula los coeficientes del spline
};

#endif //QCCLAMPEDSPLINE_H

