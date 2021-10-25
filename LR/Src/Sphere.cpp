#include "Sphere.hpp"
#include <cmath>


Sphere::Sphere() : Objet(){
  rayon = 1.0;
}


Sphere::Sphere(float xc, float yc, float zc, float r, Materiau m) : Objet(m), centre(xc, yc, zc) {
  rayon = r;
}

Sphere::~Sphere(){}


bool Sphere::intersecte(const Rayon& r, Intersection& inter){

  return false;
}


bool Sphere::coupe(const Rayon& r){

  return false;
}

ostream& operator<<(ostream & sortie, Sphere & s){
  // affichage de l'équation de la sphère
  sortie << "Sphere : de rayon " << s.rayon << ", de centre ";
  sortie << s.centre;
  // affichage du matériau de la sphère
  Objet *po = &s;
  sortie << *po << flush;
  return sortie;
}

Vecteur Sphere::getNormale(const Point &p){
  // la normale à la sphère au point P est égale
  // au vecteur CP, avec C le centre de la sphère
  Vecteur n(p.X-centre.X,p.Y-centre.Y, p.Z-centre.Z);
  n.normaliser();
  return n;
}


void Sphere::affiche(ostream& out) {
  out << "Sphere : de rayon " << rayon << ", de centre ";
  out << centre;
  Objet::affiche(out);
}

