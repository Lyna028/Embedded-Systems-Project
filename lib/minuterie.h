/*

* Classe pour le controle d'une ressource interne de l'AVR, la minuterie TIMER1
* \file   minuterie.h
* \date   12 mars 2023
*/

#include<stdio.h>
#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>


class Minuterie {
    
public :
    // Constructeur 
    // Configuration des interruptions externes, en mode appuie et relaché
    Minuterie ();

    // Destructeur 
    // Coupe les interruptions externes 
    ~Minuterie();

    // Configuration d'une minuterie TIMER1
    void partirMinuterie ( uint16_t duree );
   
   // Configuration d'un PWM 
    void ajusterPwm (uint8_t top1, uint8_t top2);

};