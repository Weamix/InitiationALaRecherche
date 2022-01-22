#include "Triangle.hpp"
#include "Plan.hpp"

#define TRI_EPSILON 0.0001
// #define MOLLER

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

    auto vAB= Vecteur(s[0],s[1]);
    auto vAC = Vecteur(s[0],s[2]);
    auto v = Vecteur::produitVectoriel(vAB, vAC);
    v.normaliser();
    n.set(v.dx,v.dy,v.dz);

}

Triangle::~Triangle(){}

#ifdef MOLLER
bool Triangle::intersecte(const Rayon& r, Intersection& inter) {
    return false;
}

bool Triangle::coupe(const Rayon& r){
  return false;
}

#else
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
    auto A = this->s[0];
    auto d = this->n * Vecteur(A.X, A.Y, A.Z);
    // manque un - ?
    //auto D = Vecteur(direction.X, direction.Y, direction.Z);

    auto nD = this->n * direction;
    if (nD == 0)
        return false;

    auto t = (d - this->n * P) / nD;
    if (t < TRI_EPSILON)
        return false;

    auto B= this->s[1];
    auto C = this->s[2];
    auto Q = Point(origin.X + t * direction.dx,
                   origin.Y + t * direction.dy,
                   origin.Z + t * direction.dz);

    /*
     * v(B,A)*v(Q,A) * n > 0
     * v(C,B)*v(Q,B) * n > 0
     * v(A,C)*v(Q,C) * n > 0
     */

    auto vAB= Vecteur(A,B);
    auto vAQ = Vecteur(A,Q);
    if ((Vecteur::produitVectoriel(vAB, vAQ) * this->n) <= 0)
        return false;

    auto vBC = Vecteur(B,C);
    auto vBQ = Vecteur(B,Q);
    if ((Vecteur::produitVectoriel(vBC, vBQ) * this->n) <= 0)
        return false;

    auto vCA = Vecteur(C,A);
    auto vCQ = Vecteur(C,Q);
    if ((Vecteur::produitVectoriel(vCA, vCQ) * this->n) <= 0)
        return false;

    inter = Intersection(Q, this, t);

    return true;
}

bool Triangle::coupe(const Rayon& r){
  return false;
}
#endif

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

