/** \file 4.cpp \brief Fichero con clases �tiles para automovilistas

Este fichero implementa clases �tiles para automovilistas, como son la
clase \c coche, la clase \c multa, la clase \c macarra, y la clase \c listillo.
\todo Implementar clases \c multa, \c macarra y \c listillo.
*/


/** \class coche
\brief Clase que almacena las caracter�sticas de un coche.
\author Cornelio Zigzag�n

Esta clase almacena las caracter�sticas a bote pronto 
m�s importantes de un coche, 
como el n�mero de ruedas o marchas, 
y devuelve dichos valores cuando se le pide
*/
class coche
{
private:
int nruedas;
int nmarchas;
float cv;
float pma;

public:
///Devuelve el n�mero de ruedas, sin contar la de recambio
int get_nruedas();
/** \brief Fija el n�mero de ruedas

Aqu� va una disertaci�n filos�fica de 
\e 17 \e horas sobre la importancia
del n�mero de ruedas \b en los coches
*/
void set_nruedas(int);
/**\overload */
void set_nruedas(float);
/** Devuelve el n�mero de marchas, sin contar el mu�eco de Elvis.
\todo Solucionar un grave fallo aritm�tico al contar las ruedas.
*/ 
int get_nmarchas();
void set_nmarchas(int);
/** \fn set_nmarchas(int)
�Le�e, casi se me olvidaba comentar la funci�n 
\ref set_nmarchas, menos mal que \c Doxygen me 
permite poner los comentarios donde me d� la gana!
\bug Si cambio de quinta a primera yendo a 160Km/h, el coche
hace un ruidito extra�o.
*/
}