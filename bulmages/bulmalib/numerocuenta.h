#ifndef NUMEROCUENTA_H
#define NUMEROCUENTA_H

#include <qstring.h>

/**\class numerocuenta
\brief Código Cuenta Cliente
@author David Gutiérrez Rubio

Esta clase implementa el Código Cuenta Cliente (CCC) de un banco. Su función más importante es
 \ref cuentaesvalida
 */
class numerocuenta
  {
  public:
    numerocuenta(QString,QString,QString,QString);//Banco, entidad, dc y numcuenta.
    numerocuenta(QString,QString,QString);//Banco, entidad y numcuenta. El dc lo calcula automaticamente.
    numerocuenta(QString);//Banco, entidad, dc y numcuenta todo seguido, con guiones opcionales.
    ~numerocuenta();
    QString getdc();//Calcula y devuelve los digitos de control del ccc
/**  \brief Comprueba si es un número de cuenta válido-
\retval true Si es un código CCC correcto.
\retval false En caso contrario.
 */
    bool cuentaesvalida(){return((getdc()==dc));};
///Devuelve el CCC separando sus miembros por la cadena \c sep    
    QString getcodigo(QString sep=""){return(banco+sep+entidad+sep+dc+sep+numcuenta);};
    QString getnumcuenta(){return(numcuenta);};///Devuelve el número de cuenta
    QString getbanco(){return(banco);};///Devuelve el número de banco
    QString getentidad(){return(entidad);};///Devuelve el número de entidad
 
/**\brief Fija el número de cuenta

Si el argumento tiene menos de 10 dígitos, se rellena con ceros a la izquierda
*/    
    void setnumcuenta(QString cad){numcuenta=cad.rightJustify(10,'0');};

/**\brief Fija el número de banco

Si el argumento tiene menos de 4 dígitos, se rellena con ceros a la izquierda
*/    
    void setbanco(QString cad){banco=cad.rightJustify(4,'0');;};
    /**\brief Fija el número de entidad

Si el argumento tiene menos de 4 dígitos, se rellena con ceros a la izquierda
*/    

    void setentidad(QString cad){entidad=cad.rightJustify(4,'0');;};
    
      
    private:
    QString banco;
    QString entidad;
    QString dc;
    QString numcuenta;

  };

#endif
