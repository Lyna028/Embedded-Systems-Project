#ifndef CONTROLE_DEL_H
#define CONTROLE_DEL_H

#include <avr/io.h> 

/*
 * Classe ControleDel:
 *   Le constructeur initialise le controleur.
 *   La méthode eteindre éteint la DEL, allumerVert l'allume en vert
 *   et allumerRouge l'allume en rouge. La méthode allumerAmbre alterne
 *   entre le vert et le rouge pour produire une lumière ambre. 
 */

class Del
{
public:

   Del();
   ~Del();

   // Eteint la DEL
   void eteindre();
   // Allume la DEL en vert
   void allumerVert();
   // Allume la DEL en rouge
   void allumerRouge();
   // Allume la DEL en ambre en alternant le vert et le rouge
   void allumerAmbre();
   // Faire clignoter la del
   void clignoterRouge(uint8_t delais);
   void clignoterVert(uint8_t delais);
   void clignoterAmbre(uint8_t delais);

private:
   // Donnees membres - aucun

};

#endif /* CONTROLE_DEL_H */