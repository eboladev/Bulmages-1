#ifndef NUMEROCUENTA_H
#define NUMEROCUENTA_H

#include <qstring.h>

/**
@author David Gutierrez
*/
class numerocuenta
  {
  public:
    numerocuenta(QString,QString,QString,QString);//Banco, entidad, dc y numcuenta.
    numerocuenta(QString,QString,QString);//Banco, entidad y numcuenta. El dc lo calcula automaticamente.
    numerocuenta(QString);//Banco, entidad, dc y numcuenta todo seguido, con guiones opcionales.
    ~numerocuenta();
    QString getdc();//Calcula y devuelve los digitos de control del ccc
    
    bool cuentaesvalida(){return((getdc()==dc));};
    QString getcodigo(QString sep=""){return(banco+sep+entidad+sep+dc+sep+numcuenta);};
    QString getnumcuenta(){return(numcuenta);};
    QString getbanco(){return(banco);};
    QString getentidad(){return(entidad);};
    void setnumcuenta(QString cad){numcuenta=cad.rightJustify(10,'0');};
    void setbanco(QString cad){banco=cad.rightJustify(4,'0');;};
    void setentidad(QString cad){entidad=cad.rightJustify(4,'0');;};
    
      
    private:
    QString banco;
    QString entidad;
    QString dc;
    QString numcuenta;

  };

#endif
