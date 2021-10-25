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
    /*
        a = xd² + yd² + zd²
        b = 2.( xd.(x0-xc) + yd.(y0-yc) + zd.(z0-zc) )
        c = x0² + y0² + z0² + xc² + yx² + zc² - 2.(x0.xc + y0.yc + z0.zc) - r²
    */

    auto origin = r.origine;
    auto center = this -> centre;

    auto rdir = r.direction;
    auto v_rdir = Vecteur(rdir.dx, rdir.dy, rdir.dz);

    auto x0_xc = origin.X - center.X;
    auto y0_yc = origin.Y - center.Y;
    auto z0_xc = origin.Z - center.Z;

    auto oc = Vecteur(x0_xc,y0_yc,z0_xc);

    auto a = v_rdir * v_rdir;

    auto b = 2 * (oc * rdir);

    auto c = 3 - this ->rayon * this -> rayon; // TODO remplacer le 3 par le début?

    auto discriminant = b * b - 4 * a * c;

    if (discriminant<0){
        return false;
    }

    auto t1, t2, t;

    t1 = (-b - sqrt(discriminant)) / (2 * a);
    t2 = (-b + sqrt(discriminant)) / (2 * a);

    t = min(t1, t2);

    return true;
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

