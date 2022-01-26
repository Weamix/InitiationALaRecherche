#include "Triangle.hpp"
#include "Plan.hpp"

#define TRI_EPSILON 0.0001
#define MOLLER


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
    /* https://www-lisic.univ-littoral.fr/~renaud/Cours/M1/IntroRecherche/document02.pdf
        DOT : vecteur * vecteur
        SUB : vecteur
        CROSS : produitVectoriel()
     * https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    */

    float det,inv_det, u, v;

    /* find vectors for two edges sharing s[0] */
    Vecteur edge1 = Vecteur(s[0], s[1]);
    Vecteur edge2 = Vecteur(s[0], s[2]);

    /* begin calculating determinant - also used to calculate u parameter */
    Vecteur pvec = Vecteur::produitVectoriel(r.direction, edge2);

    /* if determinant is near zero, ray lies in plane of triangle */
    det = edge1*pvec;

    if (det > -TRI_EPSILON && det < TRI_EPSILON)
        return false;
    inv_det = 1.0/det;

    /* calculate distance from s[0] to ray origin */
    Vecteur tvec = Vecteur(s[0], r.origine);

    /* calculate U parameter and test bounds */
    u = (tvec * pvec) * inv_det;
    if (u < 0.0 || u > 1.0)
        return false;

    /* prepare to test V parameter */
    Vecteur qvec = Vecteur::produitVectoriel(tvec,edge1);

    /* calculate V parameter and test bounds */
    v = (qvec * r.direction) * inv_det;
    if (v < 0.0 || u + v > 1.0)
        return false;

    /* calculate t, ray intersects triangle */
    float t = (edge2 * qvec) * inv_det;
    if (t > TRI_EPSILON){
        auto Q = Point(r.origine.X + t * r.direction.dx,
                       r.origine.Y + t * r.direction.dy,
                       r.origine.Z + t * r.direction.dz);

        inter = Intersection(Q,this, t);
        return true;
    }
    else
        return false;
}

bool Triangle::coupe(const Rayon& r){
    float det,inv_det, u, v;

    /* find vectors for two edges sharing s[0] */
    Vecteur edge1 = Vecteur(s[0], s[1]);
    Vecteur edge2 = Vecteur(s[0], s[2]);

    /* begin calculating determinant - also used to calculate u parameter */
    Vecteur pvec = Vecteur::produitVectoriel(r.direction, edge2);

    /* if determinant is near zero, ray lies in plane of triangle */
    det = edge1*pvec;

    if (det > -TRI_EPSILON && det < TRI_EPSILON)
        return false;
    inv_det = 1.0/det;

    /* calculate distance from s[0] to ray origin */
    Vecteur tvec = Vecteur(s[0], r.origine);

    /* calculate U parameter and test bounds */
    u = (tvec * pvec) * inv_det;
    if (u < 0.0 || u > 1.0)
        return false;

    /* prepare to test V parameter */
    Vecteur qvec = Vecteur::produitVectoriel(tvec,edge1);

    /* calculate V parameter and test bounds */
    v = (qvec * r.direction) * inv_det;
    if (v < 0.0 || u + v > 1.0)
        return false;

    /* calculate t, ray intersects triangle */
    float t = (edge2 * qvec) * inv_det;
    if (t > TRI_EPSILON){
        return true;
    }
    else
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
    auto origin = r.origine;
    auto direction = r.direction;

    auto P = Vecteur(origin.X, origin.Y, origin.Z);
    auto A = this->s[0];
    auto d = this->n * Vecteur(A.X, A.Y, A.Z);

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

    return true;
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

