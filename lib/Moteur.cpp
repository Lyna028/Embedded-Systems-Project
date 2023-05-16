/*

* Ecole Polytechnique de Montreal
* Departement de genie informatique
* Auteurs : - Lyna Kettani, 2124257
             - Delany Cruz Pulgarin, 2218061
             - Rose Lizé, 2130028
             - Emilia Rus, 2220849

* Cours inf1900
* \fichier   moteur.cpp
* \date   12 mars 2023
* Cette classe a pour but de générer un signal PWM qui servira a controler la vitesse du moteur pour faire tourner les roues.

*/

#include<stdio.h>
#include <avr/io.h>
#define F_CPU 8000000
#include "Moteur.h"

// const float FACTEUR_POURCENTAGE = 2.55;

// ENABLE SUR PINS B3 (A) ET B4 (B) (pwm)
// DATA SUR B5 et B2 (avancer vs reculer)

// constructeur : initialisation

Moteur::Moteur() {

    DDRB |= (1 << DDB3) | (1 << DDB4) | (1 << DDB5) | (1 << DDB6);
    PORTB |= (1<< PB5)|(1<< PB6);
    TCNT0 = 0;

    TCCR0A |= (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);

    TCCR0B |= (1 << CS01);

    //TCCR2C = 0;

}

//Faire avancer les roues à l'adie d'un signal PWM

void Moteur::avancer () {

    setAvancer();
    OCR0A = 255;
    OCR0B = 255;

}

void Moteur::avancer (uint8_t top) {

    setAvancer();
    OCR0A = top * 255/100;
    OCR0B = top * 255/100;

}

// Faire reculer les roues à l'aide du signal PWM

void Moteur::reculer (uint8_t top) {

    setReculer();        
    OCR0A = top * 255/100;
    OCR0B = top * 255/100;

}

//Faire avancer les roues à l'aide d'un signal PWM

void Moteur::rouler(uint8_t topDroit, uint8_t topGauche) {

    setAvancer();
    OCR0A = (topDroit) * 255/100;
    OCR0B = (topGauche) * 255/100;

}

void Moteur::arreter() {

    OCR0A = 0;
    OCR0B = 0;

}

void Moteur::tournerGauche(uint8_t top) {

    setAvancer();
    OCR0A = top * 255/100;
    OCR0B = 0;

}

void Moteur::tournerDroite(uint8_t top){

    setAvancer();
    OCR0A = 0;
    OCR0B = top * 255/100;

}

void Moteur::setAvancer(){

    PORTB &= ~(1 << PB6);
    PORTB &= ~(1 << PB5);
    //TCNT0 = 0;
}

void Moteur::setReculer(){

    PORTB |= (1<< PB6)|(1<< PB5);
    //TCNT0 = 0;
}

