/*
* Fichier : GrahamScan.cpp
* Source : Convex hull using Grahzm Scan, GeeksForGeeks.
  Lien : https://www.geeksforgeeks.org/convex-hull-using-graham-scan/

* Le but est d'implémenter l'algorithme Graham Scan pour pouvoir trier les point qui seront inclus dans l'enveloppe 
concexe de l'image SVG. Les points sont lus de la mémoire EEPROM et sont placés dans un vecteur qui sera passé en parametre 
à la fonction responsable de l'implémentation de l'algorithme.

*/

#include "Enveloppe.hpp"

using namespace std;

// Crée un tableau test.
void creerTableau(Point coordonnees[NOMBRE_POTEAUX_MAX])
{
    coordonnees[0] = Point{301, 123};
    coordonnees[1] = Point{631, 233};
    coordonnees[2] = Point{301, 343};
    coordonnees[3] = Point{411, 233};
    coordonnees[4] = Point{521, 123};
    coordonnees[5] = Point{301, 233};
    coordonnees[6] = Point{0, 0};
    coordonnees[7] = Point{0, 0};
}

// Imprime les points d'un tableau de points.
void imprimerPoints(Point coordonnees[NOMBRE_POTEAUX_MAX])
{
    int i = 0;
    while(coordonnees[i].x != 0 && coordonnees[i].y != 0)
    {
        printf("Index: %d (%d, %d)\n", i, coordonnees[i].x, coordonnees[i].y);
        i++;
    }
}

// Compare deux point en fonction de leurs angles polaires (par rapport au point de référence) 
bool comparaison(Point point1, Point point2) {
    if (point1.x < point2.x) {
        return true;
    }
    if (point1.x == point2.x && point1.y < point2.y) {
        return true;
    }
    return false;
}

// Calcule le produit vectoriel de deux vecteurs 
int produitCroise(Point point1, Point point2, Point point3) {
    int x1 = point2.x - point1.x;
    int y1 = point2.y - point1.y;
    int x2 = point3.x - point1.x;
    int y2 = point3.y - point1.y;

    return ((x1 * y2) - (x2 * y1));
}

// Echange deux points dans un tableau de points.
void swap(Point* tableauPoint, int indexPoint1, int indexPoint2) {
    Point temp = tableauPoint[indexPoint1];
    tableauPoint[indexPoint1] = tableauPoint[indexPoint2];
    tableauPoint[indexPoint2] = temp;
}

// Calcule les points de l'enveloppe (sens horaire) et retourne le nombre de points dans l'enveloppe.
int enveloppeConvexe(Point tableauPoint[NOMBRE_POTEAUX_MAX], Point pointsEnveloppe[NOMBRE_POTEAUX_MAX], const int nPoints) {
    
    //Trier les points en fonction de leurs angle polaire.
    Point pointsTrie[NOMBRE_POTEAUX_MAX] = {};
    for (int i = 0; i < nPoints; i++) {
        for (int j = i + 1; j <= nPoints; j++) {
            if (comparaison(tableauPoint[i], tableauPoint[j])) {
                swap(tableauPoint, i, j);
            }
        }
    }
    
    // Inverser ordre.
    int index = 0;
    for(int i = nPoints - 1; i >= 0 ; i--){
        if (tableauPoint[i].x != 0 && tableauPoint[i].y != 0){
            pointsTrie[index] = tableauPoint[i];
            index++;
        }
    }    

    // Selectionner les points de l'enveloppe convexe
    int tailleEnveloppe = 0;
    for (int i = 0; i < nPoints; i++) {
        while (tailleEnveloppe >= 2 && produitCroise(pointsEnveloppe[tailleEnveloppe - 2], pointsEnveloppe[tailleEnveloppe - 1], pointsTrie[i]) <= 0) {
            tailleEnveloppe--;
        }
        pointsEnveloppe[tailleEnveloppe++] = pointsTrie[i];
    }
    int m = tailleEnveloppe;
    for (int i = nPoints - 2; i >= 0; i--) {
        while ((tailleEnveloppe >= m + 1) && produitCroise(pointsEnveloppe[tailleEnveloppe - 2], pointsEnveloppe[tailleEnveloppe - 1], pointsTrie[i]) <= 0){
            tailleEnveloppe--;
        }
        pointsEnveloppe[tailleEnveloppe++] = pointsTrie[i];
    }
    return --tailleEnveloppe;
}

// Calcule l'aire d'une enveloppe convexe.
int calculerAire(Point* enveloppe, int nPoints){
    // Shoelace theorem.
    int aire = 0;
    for (int i = 0; i < nPoints; i++){
        Point pointA = enveloppe[i];
        Point pointB = enveloppe[(i+1)%nPoints];

        aire += (pointA.x * pointB.y - pointA.y * pointB.x);
    }
    aire /= 200;
    return aire;
}