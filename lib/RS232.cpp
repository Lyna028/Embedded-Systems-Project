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

 * Cette classe permet d’utiliser le protocole de communication RS232 par le USART, donc de permettre la transmission des données
 du robot au PC par l'intérmediare d'un cable USB.

*/

#include "RS232.h"

// constructeur: Configuration USART
RS232::RS232() {

    // 2400 bauds. Nous vous donnons la valeur des deux
    // premiers registres pour vous éviter des complications.
    UBRR0H = 0;
    UBRR0L = 0xCF;
    // permettre la réception et la transmission par le UART0 (p. 185)
    UCSR0A = 0;
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
    // Format des trames: 8 bits, 1 stop bits, sans parité (p.187)
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

}

// Du USART vers le PC
void RS232:: transmissionUART ( char donnee ) {
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) ) {};
    /* Put data into buffer, sends the data */
    UDR0 = donnee;
}

uint8_t RS232 :: receptionUART () {
    // Attendre des données 
    while (!(UCSR0A & (1 << RXC0)));

    // Lire les données 
    return UDR0;

}

