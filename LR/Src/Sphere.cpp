#include "Sphere.hpp"
#include <cmath>

// constante réelle très petite pour éviter :
// une auto-intersection avec la sphère de départ du rayon, qui conduit a une erreur d'évaluation de l’intensité lumineuse portée par le rayon
#define SP_EPSILON 0.0001

Sphere::Sphere() : Objet(){
  rayon = 1.0;
}


Sphere::Sphere(float xc, float yc, float zc, float r, Materiau m) : Objet(m), centre(xc, yc, zc) {
  rayon = r;
}

Sphere::~Sphere(){}


bool Sphere::intersecte(const Rayon& r, Intersection& inter){
    /*
        a = xd² + yd² + zd²
        b = 2.( xd.(x0-xc) + yd.(y0-yc) + zd.(z0-zc) )
        c = x0² + y0² + z0² + xc² + yc² + zc² - 2.(x0.xc + y0.yc + z0.zc) - r²
    */

    // common variables
    auto origin = r.origine;
    auto center = this -> centre;
    auto rdir = r.direction;

    // a variables
    auto xd2 = rdir.dx * rdir.dx;
    auto yd2 = rdir.dy * rdir.dy;
    auto zd2 = rdir.dz * rdir.dz;

    auto a = xd2 + yd2 + zd2;

    // b variables
    auto x0_xc = origin.X - center.X;
    auto y0_yc = origin.Y - center.Y;
    auto z0_zc = origin.Z - center.Z;

    auto b = 2 * (rdir.dx * (x0_xc) + rdir.dy * (y0_yc) + rdir.dz * (z0_zc));

    // c variables
    auto x02 = origin.X * origin.X;
    auto y02 = origin.Y * origin.Y;
    auto z02 = origin.Z * origin.Z;
    auto xc2 = center.X * center.X;
    auto yc2 = center.Y * center.Y;
    auto zc2 = center.Z * center.Z;

    auto c = x02 + y02 + z02 + xc2 + yc2 + zc2  - 2 * (origin.X * center.X + origin.Y * center.Y + origin.Z * center.Z )  - this ->rayon * this -> rayon;

    /*
     * la fonction intersecte renvoie false si pas d'intersection;
     * et true + les coordonnées du point d'intersection (classe Intersection) si une intersection est trouvée
     * Voir diaporama 48 : https://www-lisic.univ-littoral.fr/~renaud/Cours/M1/IntroRecherche/InitRechercheSeance1V1.1.pdf
     */

    auto discriminant = b * b - 4 * a * c;

    if (discriminant < SP_EPSILON){
        return false;
    }

    double t1, t2;
    float t;

    t1 = (-b - sqrt(discriminant)) / (2 * a);
    t2 = (-b + sqrt(discriminant)) / (2 * a);

    t = min(t1, t2);
    // Validité : si t < 0, intersection non valide (derrière l'origine du rayon)
    if (t < 0 ) {
        return false;
    }

    auto x = origin.X + t * rdir.dx;
    auto y = origin.Y + t * rdir.dy;
    auto z = origin.Z + t * rdir.dz;

    inter = Intersection(Point(x, y, z), this, t);

    return true;
}


bool Sphere::coupe(const Rayon& r){
    /*
        a = xd² + yd² + zd²
        b = 2.( xd.(x0-xc) + yd.(y0-yc) + zd.(z0-zc) )
        c = x0² + y0² + z0² + xc² + yc² + zc² - 2.(x0.xc + y0.yc + z0.zc) - r²
    */

    // common variables
    auto origin = r.origine;
    auto center = this -> centre;
    auto rdir = r.direction;

    // a variables
    auto xd2 = rdir.dx * rdir.dx;
    auto yd2 = rdir.dy * rdir.dy;
    auto zd2 = rdir.dz * rdir.dz;

    auto a = xd2 + yd2 + zd2;

    // b variables
    auto x0_xc = origin.X - center.X;
    auto y0_yc = origin.Y - center.Y;
    auto z0_zc = origin.Z - center.Z;

    auto b = 2 * (rdir.dx * (x0_xc) + rdir.dy * (y0_yc) + rdir.dz * (z0_zc));

    // c variables
    auto x02 = origin.X * origin.X;
    auto y02 = origin.Y * origin.Y;
    auto z02 = origin.Z * origin.Z;
    auto xc2 = center.X * center.X;
    auto yc2 = center.Y * center.Y;
    auto zc2 = center.Z * center.Z;

    auto c = x02 + y02 + z02 + xc2 + yc2 + zc2  - 2 * (origin.X * center.X + origin.Y * center.Y + origin.Z * center.Z )  - this ->rayon * this -> rayon;

    /*
     * en vous inspirant de ce que vous avez développé pour la méthode intersecte, en vous rappelant que ici, aucun point d’intersection ne doit être calculé.
     */

    auto discriminant = b * b - 4 * a * c;

    if (discriminant < SP_EPSILON){
        return false;
    }

    double t1, t2;
    float t;

    t1 = (-b - sqrt(discriminant)) / (2 * a);
    t2 = (-b + sqrt(discriminant)) / (2 * a);

    t = min(t1, t2);
    // Validité : si t < 0, intersection non valide (derrière l'origine du rayon)
    if (t < 0 ) {
        return false;
    }

    return true;
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

