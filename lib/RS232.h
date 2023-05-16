/*
 * Classe permettant l'acces au convertisseur analogique/numerique
 * du microcontroleur ATMega16 de Atmel.
 *
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours inf1995
 *
 * Matthew Khouzam et Jerome Collin
 * 2005-2006
 *
 * Code qui n'est sous aucune license.
 *
 */

#ifndef LIBRARY_H
#define LIBRARY_H

#include <avr/io.h>

/*
 * Classe RS232:
 *   cavalier DbgEN
 * 2400 bauds, sans parité
 * 
 *
 */

class RS232
{
public:

   RS232();
   void transmissionUART ( char donnee );
   uint8_t receptionUART();

};

#endif 
