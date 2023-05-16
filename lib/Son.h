
/*

* Classe pour la sonorité du robot
* \file   Son.h
* \date   19 mars 2023
*/

#ifndef SON_H
#define SON_H
#include <avr/io.h>


const uint8_t indiceFrequence = 37;
const uint8_t premiereFrequence = 45;


void initialiserSon(uint8_t operande);
void arreterSon();
void jouerAigu(uint8_t repetitions);
void jouerGrave();


#endif