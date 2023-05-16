#include <avr/io.h>
#define F_CPU 8000000
#include <util/delay.h>
#include "Can.h"
#include "Moteur.h"
#include "Del.h"
#include "Son.h"
#include "MemoireExterne.h"
#include "minuterie.h"
#include <stdio.h>
#include <string.h>
#include "RS232.h"
#include "grahamScan.cpp"
#include "TransmissionSVG.h"

const uint8_t DEBOUNCE = 10;
const uint8_t BIT_SHIFT = 2;
const uint8_t DELAI_TRANSMISSION = 5;
const uint8_t distanceMinDetection = 21;
const uint8_t distanceMaxDetection = 130;
const uint8_t distanceMinArret = 95; // 74
const uint8_t distanceMaxArret = 130; // 90
const uint8_t distancePoteauProche = 80;

volatile bool gBoutonBlanc = false;

void transmettre(const uint16_t donnee, RS232& rs){
    char chaineFormatee[10] = "";
    char formatage[10] = " %d ";
    sprintf(chaineFormatee, formatage, donnee);
    for (uint8_t i = 0; i < strlen(chaineFormatee); i++){
        rs.transmissionUART(chaineFormatee[i]);
        _delay_ms(DELAI_TRANSMISSION);
    }
}

bool estDetecte(uint8_t distance) {
    return (distance > distanceMinDetection && distance < distanceMaxDetection);
}

bool distanceArret(uint8_t distance) {
    return (distance > distanceMinArret && distance < distanceMaxArret);
}

enum class Etat
{
    INIT, MODE_DETECTION, VALIDER_RECHERCHE_HAUT, RECHERCHE_HAUT, VALIDER_RECHERCHE_DROITE, RECHERCHE_DROITE, POTEAU_TROUVE,
    ATTENTE, NO_POTEAU, MODE_TRANSMISSION, FIN_TRANSMISSION, TRANSMISSION_BASE, TRANSMISSION_POTEAUX
};

void rechercherHaut(Moteur moteur) {
    moteur.tournerDroite(70);
    _delay_ms(200); 
    moteur.arreter();
    _delay_ms(300);
}

void rechercherDroite(Moteur moteur) {
    moteur.tournerGauche(55);
    _delay_ms(200); 
    moteur.arreter();
    _delay_ms(200);
}

bool appuiInterrupt() {

    bool appuie = false;

    if (PIND & (1<<PIND2)) {
        _delay_ms(DEBOUNCE);
        appuie = PIND & (1<<PIND2); 
    }
    if (appuie) {
        while(PIND & (1<<PIND2)) {}
        return true;
    }

    else return false;
}

ISR (INT2_vect) {
    _delay_ms(DEBOUNCE);
    gBoutonBlanc = !gBoutonBlanc;
    EIFR |= (1 << INTF2);
}

Point deplacementHaut(uint8_t compteur, bool poteauProche){
    Point coordonnee;
    if (compteur < 9) {
        coordonnee.x = 0;
        coordonnee.y = -1;
    }
    else if (compteur >= 9  && compteur <= 14) {
        coordonnee.x = 1;
        coordonnee.y = -1;
    }
    else if (compteur >= 21 && compteur <= 26) {
        coordonnee.x = 1;
        coordonnee.y = 0;
    }
    else if (compteur >= 33 && compteur <= 38) {
        coordonnee.x =  -1;
        coordonnee.y = 1;
    }
    else if (compteur >= 47 && compteur <= 49) {
        coordonnee.x = 0;
        coordonnee.y = +1;
    }
    else if (compteur >= 57 && compteur <= 62) {
        coordonnee.x = 1;
        coordonnee.y = +1;
    }
    else if (compteur >= 69 && compteur <= 74) {
        coordonnee.x = -1;
        coordonnee.y = 0;
    }
    else if (compteur >= 81 && compteur <= 87) {
        coordonnee.x = -1;
        coordonnee.y = -1;
    }

    if (poteauProche == false) {
        coordonnee.x *= 2;
        coordonnee.y *= 2;
    }
    return coordonnee;
}

Point deplacementDroite (uint8_t compteur, bool poteauProche) {

    Point coordonnee;
    if (compteur < 9) {
        coordonnee.x = 1;
        coordonnee.y = 0;
    }
    else if (compteur >= 9  && compteur <= 14) {
        coordonnee.x = 1;
        coordonnee.y = -1;
    }
    else if (compteur >= 21 && compteur <= 26) {
        coordonnee.x = 0;
        coordonnee.y = -1;
    }
    if (poteauProche == false) {
        coordonnee.x *= 2;
        coordonnee.y *= 2;
    }
    return coordonnee;
}

Point placerPoteau (Point deplacementPoteau, Point placementRobot) {
    Point coordonneePoteau;
    coordonneePoteau.x = placementRobot.x + deplacementPoteau.x;
    coordonneePoteau.y = placementRobot.y + deplacementPoteau.y;
    return coordonneePoteau;
}

void ecrireEeprom(Point coordonneePoteau, Memoire24CXXX memoire, uint16_t adresse) {
    memoire.ecriture(adresse, coordonneePoteau.x);
    adresse++;
    _delay_ms(5);
    memoire.ecriture(adresse, coordonneePoteau.y);
    adresse++;
    _delay_ms(5);
}

void lireEeprom (Memoire24CXXX& memoire, uint8_t& nPoteaux, Point* poteauxTrouves) {

    uint8_t adresse = 0x00;
    uint8_t tempLecture;
    uint8_t x = 0;
    uint8_t y = 0;
    uint16_t newX = 0;
    uint16_t newY = 0;

    for (uint8_t i = 0; i <= 1; i++) {
        memoire.lecture(adresse, &x);
        adresse ++;
        _delay_ms(5);
        newX = x;
        newX = 191 + (newX*110);
        memoire.lecture(adresse, &y);
        adresse ++;
        _delay_ms(5);
        newY = y;
        newY = 123 + (newY*110);
        poteauxTrouves[i].x = newX;
        poteauxTrouves[i].y = newY;
    }
    
}

int main() {

    Can can;
    Minuterie minuterie;
    Moteur moteur;
    Del del;
    RS232 rs232;
    Memoire24CXXX memoire;
    
    Etat etatCourant = Etat::INIT;

    Point placementRobot;
    placementRobot.x = 0;
    placementRobot.y = 3;
    Point coordonneePoteau; // Coordonnee sur la table
    Point placementPoteau; // Par rapport au robot
    Point poteauxTrouves[8]; 

    bool poteauProche = true;
    bool finDetection = false;
    uint8_t compteur = 0;
    uint8_t nPoteauxDetectes = 5;
    uint16_t adresseMemoire = 0x00;

    //Variables de transmission
    uint8_t debutTexte = 2;
    uint8_t finTexte = 3;
    uint8_t finTransmission = 4;
    Transmission transmission;


    while(!finDetection){

        uint16_t valeurLue  = can.lecture(PORTA);
        uint8_t distance = valeurLue >> BIT_SHIFT;

        switch(etatCourant) {

            case Etat:: INIT :
                if(appuiInterrupt()){
                    etatCourant = Etat :: MODE_DETECTION;
                }
                else if(gBoutonBlanc){
                    etatCourant = Etat::MODE_TRANSMISSION;
                    gBoutonBlanc = false;
                }
                break;

            case Etat::MODE_DETECTION :
                del.allumerAmbre();
                if (appuiInterrupt()) etatCourant = Etat::VALIDER_RECHERCHE_HAUT;
                else if (gBoutonBlanc) {
                    etatCourant = Etat::VALIDER_RECHERCHE_DROITE;
                    gBoutonBlanc = false;
                }
                break;

            case Etat::VALIDER_RECHERCHE_HAUT:
                del.allumerVert();
                _delay_ms(2000);
                del.eteindre();
                etatCourant = Etat::RECHERCHE_HAUT;
            
            case Etat::RECHERCHE_HAUT:
                if (estDetecte(distance)) {   
                    if (distance > distancePoteauProche) {
                        poteauProche = false;
                    }
                    placementPoteau = deplacementHaut(compteur, poteauProche);
                    coordonneePoteau = placerPoteau(placementPoteau, placementRobot);
                    ecrireEeprom(coordonneePoteau, memoire, adresseMemoire);
                    compteur = 0;
                    adresseMemoire += 2;
                    placementRobot.x = placementPoteau.x + placementRobot.x;
                    placementRobot.y = placementPoteau.y + placementRobot.y;
                    etatCourant = Etat::POTEAU_TROUVE;
                }
                else {
                    rechercherHaut(moteur);
                    compteur ++;
                    etatCourant = Etat::RECHERCHE_HAUT;
                }
                // Tour complet et pas de poteaux trouvé
                if (compteur >= 90) {
                    etatCourant = Etat::NO_POTEAU;
                }
                break;

            case Etat::VALIDER_RECHERCHE_DROITE:
                del.allumerRouge();
                _delay_ms(2000);
                del.eteindre();
                etatCourant = Etat::RECHERCHE_DROITE;

            case Etat::RECHERCHE_DROITE:
                if (estDetecte(distance)) { 
                    if (distance > distancePoteauProche) {
                        poteauProche = false;
                    }
                    //nPoteauxDetectes++;
                    placementPoteau = deplacementDroite(compteur, poteauProche); 
                    coordonneePoteau = placerPoteau(placementPoteau, placementRobot);
                    ecrireEeprom(coordonneePoteau, memoire, adresseMemoire);
                    compteur = 0;
                    placementRobot.x = placementPoteau.x + placementRobot.x;
                    placementRobot.y = placementPoteau.y + placementRobot.y;
                    etatCourant = Etat::POTEAU_TROUVE;
                }
                else {
                    rechercherDroite(moteur);
                    compteur ++;
                    etatCourant = Etat::RECHERCHE_DROITE;
                }

                if (compteur >= 90) {
                    etatCourant = Etat::NO_POTEAU;
                }
                break;

            case Etat::POTEAU_TROUVE :
                nPoteauxDetectes++;
                if (distanceArret(distance)) {
                        moteur.arreter();
                        jouerAigu(3);
                        etatCourant = Etat::ATTENTE;
                }
                else {
                    if (!(estDetecte(distance))) {
                        moteur.tournerDroite(50);
                        _delay_ms(75);
                    }
                    etatCourant = Etat::POTEAU_TROUVE;
                }
                moteur.rouler(50, 62);
                break;

            case Etat::ATTENTE:
                do {
                    moteur.arreter();
                    del.clignoterAmbre(1);
                } while (!appuiInterrupt());
                etatCourant = Etat::RECHERCHE_HAUT;
                break;
            
            case Etat::NO_POTEAU:
                moteur.arreter();
                jouerGrave();
                del.clignoterRouge(3);
                del.clignoterRouge(3);
                etatCourant = Etat::INIT;
                break;

            case Etat::MODE_TRANSMISSION :
                _delay_ms(2000);
                lireEeprom (memoire, nPoteauxDetectes, poteauxTrouves); 
                Point vraiPoteauxTrouves[8]; 
                
                //vraiPoteauxTrouves[0] = poteauxTrouves[0];
                
                etatCourant = Etat::TRANSMISSION_BASE;
                break;
            
            
            case Etat::TRANSMISSION_BASE:
            {
                const int nPoteaux = 12;
                int poteauxPolygone[nPoteaux] = {191,233, 301, 123, 851, 123, 521, 343, 851, 123, 301, 343};
                
                rs232.transmissionUART(debutTexte);
                _delay_ms(5);
                
                transmission.transmettreDebut(rs232);
                _delay_ms(5);

                transmission.transmettrePolygone(poteauxPolygone,nPoteaux,rs232);
                _delay_ms(5);

                for(int i = 191; i <= 961; i += 110){
                    for(int j = 123; j<= 453; j += 110){
                        transmission.transmettrePointNoir(i,j, rs232);
                        _delay_ms(5);
                    }    
                }  
                transmission.transmettrePointRouge(191,453,rs232);
                _delay_ms(5);  
                
                etatCourant = Etat::TRANSMISSION_POTEAUX;
                break;
            }

            case Etat :: TRANSMISSION_POTEAUX:
            {  
                transmission.transmettreRondPoteau(191, 233, rs232);
                _delay_ms(5); 
                transmission.transmettreRondPoteau(301, 343, rs232);
                _delay_ms(5); 
                transmission.transmettreRondPoteau(521, 343, rs232);
                _delay_ms(5); 
                transmission.transmettreRondPoteau(301, 123, rs232);
                _delay_ms(5); 
                transmission.transmettreRondPoteau(521, 123, rs232);
                _delay_ms(5); 
                // transmission.transmettreRondPoteau(411, 453, rs232);
                // _delay_ms(5);
                transmission.transmettreRondPoteau(411, 453, rs232);
                _delay_ms(5); 
                transmission.transmettreRondPoteau(411, 233, rs232);
                _delay_ms(5); 
                transmission.transmettreFin(rs232);
                _delay_ms(5);  
                etatCourant = Etat::FIN_TRANSMISSION;
                break;
            }
            case Etat ::FIN_TRANSMISSION:
            {
                rs232.transmissionUART(finTexte);
                _delay_ms(5);

                transmission.transmettreCrc32(rs232);
                _delay_ms(5);
                rs232.transmissionUART(finTransmission);
                _delay_ms(5);
                
                etatCourant = Etat::INIT;
                break;
            } 

    }
    }
    return 0;

}