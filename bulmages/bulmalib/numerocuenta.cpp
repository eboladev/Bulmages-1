#include "numerocuenta.h"
#include <qstring.h>

/**\brief Constructor con los 4 parámetros
\param bancopar Código banco (4 dígitos)
\param entidadpar Código entidad (4 dígitos)
\param dcpar Dígitos de Control (2 dígitos)
\param numcuentapar Código de cuenta (10 dígitos)
*/
numerocuenta::numerocuenta(QString bancopar,QString entidadpar,QString dcpar,QString numcuentapar)
{
setbanco(bancopar);
setentidad(entidadpar);
setnumcuenta(numcuentapar);
dc=dcpar;
}
/**\overload
En este caso los dígitos de control se calculan automáticamente
*/
numerocuenta::numerocuenta(QString bancopar,QString entidadpar,QString numcuentapar)
{
setbanco(bancopar);
setentidad(entidadpar);
setnumcuenta(numcuentapar);
dc=getdc();
}
/**\overload
Si la cuenta se presenta como una única cadena, extrae los 4 elementos de ella.
*/
numerocuenta::numerocuenta(QString cuenpar)
{
QString tem=cuenpar;
tem.remove("-");//[TODO] Que elimine tambien otro tipo de caracteres como espacios, etc.
banco=tem.mid(0,4);
entidad=tem.mid(4,4);
dc=tem.mid(8,2);
numcuenta=tem.mid(10,10);
}




numerocuenta::~numerocuenta()
{}

/**\brief Devuelve los dígitos de control del número de cuenta
*/
QString numerocuenta::getdc()
{
QString resul;
int i;
int sum=0;

/*
Unidades =  6
Decenas  =  3
Centenas =  7
Millares =  9
DiezMil. = 10
CienMil. =  5
Millones =  8
DiezMill.=  4
CienMill.=  2
MilMill. =  1*/

const int we[]={1,2,4,8,5,10,9,7,3,6};
//if (banco.length()!=4) 

for (i=0;i<4;i++) 
sum+=QString(banco[i]).toInt()*we[i+2];

for (i=0;i<4;i++) 
sum+=QString(entidad[i]).toInt()*we[i+6];


sum=(sum % 11);
sum=11-sum;

if (sum==11) sum=0;
if (sum==10) sum=1;



int sum2=0;
for (i=0;i<10;i++)
sum2+=QString(numcuenta[i]).toInt()*we[i];

sum2=(sum2 % 11);
sum2=11-sum2;

if (sum2==11) sum2=0;
if (sum2==10) sum2=1;


resul.sprintf("%d%d",sum,sum2);


return(resul);




}
