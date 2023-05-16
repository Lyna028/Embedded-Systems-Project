/*
* Fichier : GrahamScan.cpp
* Source : Convex hull using Grahzm Scan, GeeksForGeeks.
  Lien : https://www.geeksforgeeks.org/convex-hull-using-graham-scan/

* Le but est d'implémenter l'algorithme Graham Scan pour pouvoir trier les point qui seront inclus dans l'enveloppe 
concexe de l'image SVG. Les points sont lus de la mémoire EEPROM et sont placés dans un vecteur qui sera passé en parametre 
à la fonction responsable de l'implémentation de l'algorithme.

*/

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <math.h>
using namespace std;

struct Point {
    int x, y;
};

// Compare deux point en fonction de leurs angles polaire (par rapport au point de référence) 
bool comparaison(Point point1, Point point2) {
    if (point1.y < point2.y) {
        return true;
    }
    if (point1.y == point2.y && point1.x < point2.x) {
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

    return x1 * y2 - x2 * y1;
}

void swap(Point& a, Point& b) {
    Point temp = a;
    a = b;
    b = temp;
}

// Algorithme de Graham Scan.
void enveloppeConvexe(Point tableauPoint[8], const int nPoints) {

    // Indice du point le plus bas 
    int plusBasPoint = 0;
    for (int i = 0; i < nPoints; i ++) {
        if (tableauPoint[i].y < tableauPoint[plusBasPoint].y) {
            plusBasPoint = i;
        }
    }

    // Le point le plus bas en permier position du vecteur de points 
    swap(tableauPoint[0], tableauPoint[plusBasPoint]);

    // Trie des points en fonction de leurs angle polaire
    for (int i = 1; i < nPoints; i++) {
        for (int j = i + 1; j <= nPoints; j++) {
            if (comparaison(tableauPoint[j], tableauPoint[i])) {
                swap(tableauPoint[i], tableauPoint[j]);
            }
        }
    }
    
    Point pointsEnveloppe[8];
    int m = 0;
    pointsEnveloppe[m++] = tableauPoint[0];
    pointsEnveloppe[m++] = tableauPoint[1];

    // Ajouter le reste des points a l enveloppe convexe
    for (int i = 2; i < nPoints; i++ ) {
        while (m >= 2) {
            Point p1 = pointsEnveloppe[m-1];
            Point p2 = pointsEnveloppe[m-2];
            if (produitCroise(p2, p1, tableauPoint[i]) > 0) {
                break;
            }
            m--;
        }
        pointsEnveloppe[m++] = tableauPoint[i];
    }
}