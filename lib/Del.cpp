/*
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Auteurs : - Lyna Kettani, 2124257
             - Delany Cruz Pulgarin, 2218061
             - Rose Lizé, 2130028
             - Emilia Rus, 2220849
 * Cours inf1995
 * \fichier   del.cpp
 * \date   12 mars 2023

 * Cette classe permet de controler la DEL, pour l'allumer d'une couleur spécifique, la faire clignoter ou encore l'éteindre. 

*/


#include <avr/io.h> 
#include "Del.h"
#define F_CPU 8000000UL
#include <util/delay.h>

const uint8_t DELAIS_AMBRE = 1;
const uint16_t DELAIS_DEMI_SECONDE = 500;

Del::Del()
{
    DDRC |= (1 << DDC2) | (1 << DDC3);
}

Del::~Del()
{
    PORTC &= ~(1 << PORTC2);
    PORTC &= ~(1 << PORTC3);
}

void Del::allumerRouge()
{
    PORTC &= ~(1 << PORTC2);
    PORTC |= (1 << PORTC3);
}

void Del::allumerVert()
{

    PORTC |= (1 << PORTC2);
    PORTC &= ~(1 << PORTC3);
}

void Del::eteindre()
{
    PORTC &= ~(1 << PORTC2);
    PORTC &= ~(1 << PORTC3);
}

void Del::allumerAmbre()
{
    allumerRouge();
    _delay_ms(DELAIS_AMBRE);
    allumerVert();
    _delay_ms(DELAIS_AMBRE);
}

void Del::clignoterRouge(uint8_t delais)
{
    for (uint8_t i = 0; i < delais ; i++)
    {
        allumerRouge();
        _delay_ms(DELAIS_DEMI_SECONDE);
        eteindre();
        _delay_ms(DELAIS_DEMI_SECONDE);
    }
}

void Del::clignoterVert(uint8_t delais)
{
    for (uint8_t i = 0 ; i < delais ; i++)
    {
        allumerVert();
        _delay_ms(DELAIS_DEMI_SECONDE);
        eteindre();
        _delay_ms(DELAIS_DEMI_SECONDE);
    }
}

void Del::clignoterAmbre(uint8_t delais)
{
    for (uint8_t i=0 ; i < delais ; i++)
    {
        for (uint16_t j=0 ; j < 250 ; j++)
        {
            allumerAmbre();
        }
        eteindre();
        _delay_ms(DELAIS_DEMI_SECONDE);
    }
}