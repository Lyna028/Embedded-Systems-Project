/*
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Auteurs : - Lyna Kettani, 2124257
             - Delany Cruz Pulgarin, 2218061
             - Rose Lizé, 2130028
             - Emilia Rus, 2220849
 * Cours inf1900
 * \fichier   son.cpp
 * \date   19 mars 2023

 * Code permettant de jouer et arreter une sonorité.

*/
#include<stdio.h>
#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include "Son.h"

const uint8_t nElementsTableau_ = 37;
const uint8_t premiereValeure_ = 45;

void initialiserSon(uint8_t operande) 
{
    DDRD |= (1 << PD4) | (1 << PD5); 
    PORTD &= ~(1 << PD4);           
    
    const float tableauFrequences[nElementsTableau_] = { 0.11000, 0.11654, 0.12347, 0.13081, 
                                                         0.13859, 0.14683, 0.15556, 0.16481, 
                                                         0.17461, 0.18500, 0.19600, 0.20765, 
                                                         0.22000, 0.23308, 0.24694, 0.26163, 
                                                         0.27718, 0.29366, 0.31113, 0.32963, 
                                                         0.34923, 0.36999, 0.39200, 0.41530, 
                                                         0.44000, 0.46616, 0.49388, 0.52325, 
                                                         0.55437, 0.58733, 0.62225, 0.65926, 
                                                         0.69846, 0.73999, 0.78399, 0.83061, 
                                                         0.88000 };
        
    float tableauNotes[nElementsTableau_] = {};

    for (uint8_t i = 0; i < nElementsTableau_; i++) {
        //OCRNA = (8000/x - 512) * (1/512) ...(x = frequence ocrnx)
        tableauNotes[i] = ((8000 - (512 * tableauFrequences[i])) / (512 * tableauFrequences[i])); 
    }
    
    TCNT1 = 0;
    TCCR1A |= (1 << COM1A0); 
    TCCR1B |= (1 << CS12);   
    TCCR1B |= (1 << WGM12);  
    TCCR1C  = 0;
    OCR1A = tableauNotes[operande - premiereValeure_];
}

/* const float frequences[indiceFrequence] = { 

        110.00, 116.54, 123.47, 130.81, 138.59, 146.83, 
        155.56, 164.81, 174.61, 185.00, 196.00, 207.65, 
        220.00, 233.08, 246.94, 261.63, 277.18, 293.66, 
        311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 
        440.00, 466.16, 493.88, 523.25, 554.37, 587.33, 
        622.25, 659.26, 698.46, 739.99, 783.99, 830.61, 
        880.00 

};

void initialiserSon(uint8_t operande) {

    DDRD |= (1 << PD4) | (1 << PD5); 
    PORTD &= ~(1 << PD4);           
        
    float notes[indiceFrequence] = {};

    for (uint8_t i = 0; i < indiceFrequence; i++) {
        float frequenceMhz = frequences[i]/1000.0;
        notes[i] = ((8000 - (512 * frequenceMhz)) / (512 * frequenceMhz)); 
    }
    
    TCNT1 = 0;
    TCCR1A |= (1 << COM1A0); 
    TCCR1B |= (1 << CS12);   
    TCCR1B |= (1 << WGM12);  
    TCCR1C  = 0;
    OCR1A = notes[operande - premiereFrequence]; 

}
*/

void arreterSon() {
    
    DDRD = 0x00; 
    TCNT1 = 0;
    OCR1A = 0;
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;
}

void jouerAigu(uint8_t repetitions)
{
    uint8_t note = 80;
    for (repetitions; repetitions > 0; repetitions --)
    {
        initialiserSon(note);
        _delay_ms(300);
        arreterSon();
        _delay_ms(300);
    }
}

void jouerGrave()
{
    uint8_t note = 45;
    initialiserSon(note);
    _delay_ms(2000);
    arreterSon();
}
