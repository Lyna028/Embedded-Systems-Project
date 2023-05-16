#include "RS232.h"
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

class Transmission{


    public: 

    Transmission(){crc32 = 0xFFFFFFFF;}
    int trouverLongueur(char texte[]);

    void transmettreUARTByte ( char texte[], RS232& rs232 ); 


    void transmettrePointNoir(int x, int y, RS232& rs232);

    void transmettrePointRouge(int x, int y, RS232& rs232);
    void transmettreAire(int aire,RS232& rs232);

    void transmettreFin(RS232& rs232);

    void transmettreDebut(RS232& rs232);

    void trouverCrc32(const char *texte,int longueur) ;

    void transmettreCrc32(RS232& rs232);

    void transmettreRondPoteau(int x, int y, RS232& rs232);

    void transmettrePolygone(int tableauPoteaux[],const int nPoteaux,RS232& rs232 );

    private:
    unsigned long int crc32;


};
