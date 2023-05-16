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
#include <stdio.h>
#include <math.h>
#define NOMBRE_POTEAUX_MAX 8

using namespace std;

struct Point {
    int x, y;
};

// Calcule les points de l'enveloppe (sens horaire) et retourne le nombre de points dans l'enveloppe.
int enveloppeConvexe(Point tableauPoint[NOMBRE_POTEAUX_MAX], Point pointsEnveloppe[NOMBRE_POTEAUX_MAX], const int nPoints);

// Calcule l'aire d'une enveloppe convexe.
int calculerAire(Point* enveloppe, int nPoints);

// Imprime les points d'un tableau de points.
void imprimerPoints(Point coordonnees[NOMBRE_POTEAUX_MAX]);