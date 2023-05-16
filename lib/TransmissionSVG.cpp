#include "TransmissionSVG.h"

int Transmission :: trouverLongueur(char texte[]){
        int longueur;
        for (longueur = 0; texte[longueur] != '\0'; ++longueur) ;
        return longueur;
    }


void Transmission:: transmettreUARTByte ( char texte[], RS232& rs232 ){
        int longueur;
        for (longueur = 0; texte[longueur] != '\0'; ++longueur) ;

       

        for (int i = 0; i < longueur; ++i) {
            rs232.transmissionUART(texte[i]);
            _delay_ms(5);   
        }
         
    } 

    void Transmission :: transmettrePointNoir(int x, int y, RS232& rs232){
        const int longueur = 100;
        char lignePoint[longueur]; 
        sprintf(lignePoint, "<rect x='%d' y='%d' width='5' height='5' stroke='black' stroke-width='1' fill='black'/>\0", x,y);

        
        int longueurCrc32 = trouverLongueur(lignePoint);
        trouverCrc32(lignePoint,longueurCrc32);
        transmettreUARTByte(lignePoint,rs232);
}

    void Transmission :: transmettrePointRouge(int x, int y, RS232& rs232){
        const int longueur = 100;
        char lignePoint[longueur];
        sprintf(lignePoint,"<rect x='%d' y='%d' width='5' height='5' stroke='red' stroke-width='1' fill='red'/>\0",x,y);

        int longueurCrc32 = trouverLongueur(lignePoint);
        trouverCrc32(lignePoint,longueurCrc32);
        transmettreUARTByte(lignePoint, rs232);
    }

    void Transmission :: transmettreAire(int aire,RS232& rs232){
        const int longueur = 100;
        char ligneAire[longueur];
        sprintf(ligneAire,"<text x='96' y='554' font-family='arial' font-size='20' fill='blue'>Aire: %d pouces carres</text>\0", aire);


        int longueurCrc32 = trouverLongueur(ligneAire);
        trouverCrc32(ligneAire,longueurCrc32);
        transmettreUARTByte(ligneAire, rs232); 
        
    }

    void Transmission :: transmettreFin(RS232& rs232){
        //const int longueur = 128;
        char lignesFin[128]= "<text x='96' y='36' font-family='arial' font-size='20' fill='blue'>Section 05 -- equipe116117 -- DOM</text></svg></body></html>";
        
        int longueurCrc32 = trouverLongueur(lignesFin);
        trouverCrc32(lignesFin,longueurCrc32);
        transmettreUARTByte(lignesFin,rs232);
    }

    void Transmission :: transmettreDebut(RS232& rs232){
        char lignesInitiales[210]= "<!DOCTYPE html><html><body><svg width='100%' height='100%' xmlns='http://www.w3.org/2000/svg' viewBox='0 0 1152 576'><rect x='96' y='48' width='960' height='480' stroke='black' stroke-width='1' fill='white'/>\0";
    
    
        int longueurCrc32 = trouverLongueur(lignesInitiales);
        trouverCrc32(lignesInitiales,longueurCrc32);
        transmettreUARTByte(lignesInitiales,rs232);
    }

    void Transmission :: trouverCrc32(const char *texte,int longueur)  {
	unsigned long diviseur = 0xEDB88320;

	for(int i=0;i<longueur;i++) {
		crc32 ^= texte[i];
		for(int j=0;j<8;j++) {
			if((crc32 & 1) != 0){
                crc32 = (crc32 >> 1) ^ diviseur;
            }
            else{
                crc32 =  crc32 >> 1;
            }
		}
	}
    } 

    void Transmission :: transmettreCrc32(RS232& rs232){
        crc32 = crc32 ^ 0xFFFFFFFF;
        const int longueur = 9;
        char intFormater[longueur];
        sprintf(intFormater,"%08lx", crc32  );
        transmettreUARTByte(intFormater,rs232);

    }

 void Transmission :: transmettreRondPoteau(int x, int y, RS232& rs232){
        const int longueur = 100;
        char lignePoteau[longueur];
        sprintf(lignePoteau,"<circle cx='%d' cy='%d' r='10' stroke='black' stroke-width='2' fill='grey' />\0",x,y);

        int longueurCrc32 = trouverLongueur(lignePoteau);
        trouverCrc32(lignePoteau,longueurCrc32);
        transmettreUARTByte(lignePoteau, rs232);
    }


void Transmission :: transmettrePolygone(int tableauPoteaux[], const int nPoteuax, RS232& rs232 ){
        /*char lignesInitiales[63]= "<polygon points='301,123 301,233 301,343 631,233, 521,123 '/>\0";

    
        int longueurCrc32 = trouverLongueur(lignesInitiales);
        trouverCrc32(lignesInitiales,longueurCrc32);
        transmettreUARTByte(lignesInitiales,rs232);*/
    
    
    const int longueur = 190;
    char polygone[longueur] = "<polygon points='\0";
    int longueurCrc32 = trouverLongueur(polygone);
        trouverCrc32(polygone,longueurCrc32);
        transmettreUARTByte(polygone, rs232);
 
    for(int i=0;i<nPoteuax;i++){
         char position[4];
        if(i%2 != 0){
         
        sprintf(position,"%d ", tableauPoteaux[i]);
        
        
        }
       else{
        
        sprintf(position,"%d,", tableauPoteaux[i]);
  
       }
       longueurCrc32 = trouverLongueur(position);
        trouverCrc32(position,longueurCrc32);
        transmettreUARTByte(position, rs232);
    }
    char fin[4] = "'/>";
    // char fin[22] = "'/>\0";
     longueurCrc32 = trouverLongueur(fin);
        trouverCrc32(fin,longueurCrc32);
        transmettreUARTByte(fin, rs232);

}