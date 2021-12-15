#include "Triangle.hpp"
#include "Plan.hpp"

#define TRI_EPSILON 0.0001

Triangle::Triangle() : Objet(){
  s[0].set(-1, 0, -1);
  s[1].set(1, 0, -1);
  s[2].set(0, 0, 1);
  n.set(0, 0, 1);
}

Triangle::Triangle(const Point p[3], Materiau m) : Objet(m) {
  for(int i=0; i<3; i++)
    s[i] = p[i];

  // calcul de la normale à partir du produit vectoriel AB^AC
  // cette normale doit ensuite être normalisée..

}

Triangle::~Triangle(){}


bool Triangle::intersecte(const Rayon& r, Intersection& inter){
    auto origin = r.origine;
    auto direction = r.direction;

    /*
     * https://www-lisic.univ-littoral.fr/~renaud/Cours/M1/IntroRecherche/tpInitRecherche02.pdf
     * t = (d-n*P)/(n*D)
     * P vecteur origin
     * d = n*x
     * D direction
     */

    auto P = Vecteur(origin.X, origin.Y, origin.Z);
    auto x = s[0];
    auto d = this->n * Vecteur(x.X, x.Y, x.Z);
    //auto D = Vecteur(direction.X, direction.Y, direction.Z);

    auto nD = this->n * direction;
    if (nD == 0)
        return false;

    auto t = (d - this->n * P) / nD;

    if (t < TRI_EPSILON)
        return false;


}

bool Triangle::coupe(const Rayon& r){
  return false;
}


Vecteur Triangle::getNormale(const Point &p){
  return n;
}

ostream& operator<<(ostream & sortie, Triangle & t){
  sortie << "triangle : ";
  for(int i=0; i<3; i++)
    sortie << t.s[i] << " - ";
  sortie << endl;

  return sortie;
    
}

void Triangle::affiche(ostream& out){
  out << "triangle : ";
  for(int i=0; i<3; i++)
    out << s[i] << " - ";
  out << endl;
}

