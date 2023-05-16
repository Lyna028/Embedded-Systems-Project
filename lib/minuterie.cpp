/*
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Auteurs : - Lyna Kettani, 2124257
             - Delany Cruz Pulgarin, 2218061
             - Rose Lizé, 2130028
             - Emilia Rus, 2220849
 * Cours inf1995
 * \fichier   minuterie.cpp
 * \date   12 mars 2023

 * Cette classe a pour but le controle d'une ressource interne de l'AVR, la minuterie TIMER1.

*/

#include<stdio.h>
#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include "minuterie.h"

Minuterie :: Minuterie() {
    cli ();
    //DDRA |= (1 << DDA0) | (1 <<DDA1); // PIN A0 Et A1 en sortie
    DDRD &= ~(1 << DDB2); // PIN B2 en entrée
    EIMSK |= (1 << INT2) ; // Permet les interruptions externes 
    EICRA = (1<<ISC21) ; // sensibilisation aux changement de niveaux du buton-poussoir, ici falling edge (p.68)
    sei ();
}

Minuterie :: ~Minuterie () {
    EIMSK =0; // Coupe les interruptions externes 
}

void Minuterie :: partirMinuterie ( uint16_t duree ) {
    // mode CTC du timer 1 avec horloge divisée par 1024
    TCNT2 = 0 ;
    OCR2A = duree;
    TCCR2A = (1 << WGM22);
    TCCR2B |= (1<<CS22) | (1<<CS20);
    //TCCR1C = 0;
    //TIMSK1 = (1<< OCIE1A);
}

void Minuterie :: ajusterPwm (uint8_t top1, uint8_t top2) {
    // Le top pour avoir une interruption, pour les deux roues.
    OCR2A = 0xff * top1;
    OCR2B = 0xff * top2;
    // division d'horloge par 8 - implique une fréquence de PWM fixe
    TCCR2A |= (1 << WGM10) | (COM1A1 <<1) | (1 << COM1B1);
    TCCR2B |= (1 << CS11);
    //TCCR1C = 0;
}

