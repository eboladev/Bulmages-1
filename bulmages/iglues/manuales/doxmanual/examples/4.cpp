/** \class coche
\brief Clase que almacena las características de un coche.

Esta clase almacena las características a bote pronto más importantes de un coche, 
como el número de ruedas o marchas, y devuelve dichos valores cuando se le pide
*/

class coche
{
private:
int nruedas;

int nmarchas;
float cv;
float pma;

public:

int get_nruedas();///< Devuelve el número de ruedas, sin contar la de recambio
/** \brief Fija el número de ruedas

Aquí va una disertación filosófica de \e 17 \e horas sobre la importancia
del número de ruedas \b en los coches
*/
void set_nruedas(int);
/**\overload */
void set_nruedas(float);
/** Devuelve el número de marchas, sin contar el muñeco de Elvis.
\todo Solucionar un grave fallo aritmético al contar las ruedas.
*/ 
int get_nmarchas();
void set_nmarchas(int);


/** \fn set_nmarchas(int)
¡Leñe, casi se me olvidaba comentar la función \ref set_nmarchas, menos mal 
que \c Doxygen me permite poner los comentarios donde me dé la gana!
\bug Si cambio de quinta a primera yendo a 160Km/h, el coche
hace un ruidito extraño.
*/
}