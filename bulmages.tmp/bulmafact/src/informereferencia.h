// (C) Tomeu Borrás Riera

#ifndef __INFORMEREFERENCIA__
#define __INFORMEREFERENCIA__


#include <qstring.h>
class company;


class InformeReferencia {
private:
	QString m_referencia;
	company* companyact;
public:
	InformeReferencia(company *);
	~InformeReferencia();
	void setreferencia(QString val) {m_referencia = val;};
	void generarinforme();
};

#endif


