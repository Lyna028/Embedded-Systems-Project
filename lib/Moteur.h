/*

* Classe pour la motricité du robot en utilisant un timer2
* \file   Moteur.h
* \date   12 mars 2023
*/

#ifndef MOTEUR_H
#define MOTEUR_H


#include <avr/io.h>

class Moteur {
public:

    Moteur();

    void avancer ();
    void avancer (uint8_t top);
    void reculer (uint8_t top);
    void arreter();
    void tournerGauche(uint8_t top);
    void tournerDroite(uint8_t top);
    void rouler(uint8_t topDroit, uint8_t topGauche);

private:
    void setAvancer();
    void setReculer();

};

#endif /* MOTEUR_H */