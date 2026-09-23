#include <bits/stdc++.h>
#define ll long long
#define ld long double
#define int ll

const ld PI = acos(-1);
using namespace std;
typedef ld T;
typedef complex<T> pt;
#define x real()
#define y imag()
const double EPS = 1e-9;

T sq(pt p){
    return p.x * p.x + p.y * p.y;
}

T dot(pt v, pt w){
    // v.w
    return v.x * w.x + v.y * w.y;
}

T cross(pt v, pt w){
    // v*w
    return v.x * w.y - v.y * w.x;
}

int sgn(T val){
    if(val > EPS) return 1;
    if(val < -EPS) return -1;
    return 0;
}

bool isPerp(pt v, pt w){
    //is perpendicular
    return fabs(dot(v, w)) < EPS;
}

pt prep(pt p){
    //returns a perpendicular vector
    return {-p.y, p.x};
}

//////////////// TRASFORMATIONS ////////////////////////
//translates point p with vector v
pt translate(pt v, pt p) {return p+v;}

//scales p by factor around c
pt scale(pt c, ld factor, pt p) {
    return c + (p-c)*factor;
}

// rotates point p around c by a rad
pt rot(pt p, pt c, ld a){
    pt v = p - c;
    pt rotate = {cos(a), sin(a)};
    return c + rotate * v;
}

//known p,q and its transformation, get trans of r
pt linearTransfo(pt p, pt q, pt r, pt fp, pt fq) {
    return fp + (r-p) * (fq-fp) / (q-p);
}

/////////////// Angles //////////////////////////
//let u know where is the point to a vector
//+ > left , - > right , else > on the line
//line ab and point c
T orient(pt a, pt b, pt c){
    return cross(b - a, c - a);
}

//angle between two vectors (minimum one)
T angle(pt v, pt w){
    return acos(clamp(dot(v, w) / abs(v) / abs(w), (T)-1.0, (T)1.0));
}

//conter clockw wise angle
T orientedAngle(pt a, pt b, pt c){
    ld ampli = angle(b - a, c - a);
    if(orient(a, b, c) > 0) return ampli;
    else return 2 * M_PI - ampli;
}

//signed angle from ab to ac
T angleTravelled(pt a, pt b, pt c){
    ld ampli = angle(b - a, c - a);
    if(orient(a, b, c) > 0) return ampli;
    else return -ampli;
}

//check p in between angle(bac) counter clockwise
bool inAngle(pt a, pt b, pt c, pt p) {
    T abp = orient(a, b, p), acp = orient(a, c, p), abc = orient(a, b, c);
    if (abc < 0) swap(abp, acp);
    return (abp >= 0 && acp <= 0) ^ (abc < 0);
}

struct line{
    pt v; T c;

    line(pt v, T c) : v(v), c(c) {}

    // from equation ax+by = c
    line(T a, T b, T _c){
        v = {b, -a};
        c = _c;
    }

    //line from two points
    line(pt p, pt q){
        v = q - p;
        c = cross(v, p);
    }

    //let u know where is the point to the line
    T side(pt p) {return cross(v,p)-c;}
    //distance from p to the vector
    ld dist(pt p) {return abs(side(p)) / abs(v);}
    //squared distance from p to the vector
    double sqDist(pt p) {return side(p)*side(p) / (T)sq(v);}
    //perpendicular line pass through p
    line prepThrought(pt p){ return {p, p + prep(v)};}
    bool cmpProj(pt p, pt q){
        return dot(v, p) < dot(v, q);
    }
    //translate line 
    line translate(pt t) {return {v, c + cross(v,t)};}
    //return line on left of the original line by distance dist
    line shiftLeft(T dist) {return {v, c + dist*abs(v)};}
    //returns point p` that's the projection of p on the line
    //side(p)/abs(v) = the distance between p and the line
    pt proj(pt p) {return p - prep(v)*side(p)/sq(v);}
    //returns point p` that's the reflection of p to the line
    pt refl(pt p) {return p - prep(v) * (T)2.0 * side(p)/sq(v);}
};

//returns true if the two lines intersect
//returns intersection point in pt out
bool inter(line l1, line l2, pt &out) {
    T d = cross(l1.v, l2.v);
    if (fabs(d) < EPS) return false;
    out = (l2.v*l1.c- l1.v*l2.c) / d; // requires floating-point coordinates
    return true;
}

//returns line that bisects an angle between two lines
line bisector(line l1, line l2, bool interior) {
    assert(cross(l1.v, l2.v) != 0); // l1 and l2 cannot be parallel!
    ld sign = interior ? 1 :-1;
    return {l2.v/abs(l2.v) + l1.v/abs(l1.v) * sign,
            l2.c/abs(l2.v) + l1.c/abs(l1.v) * sign};
}

//////////////////////////////////////////  SEGMENTS   //////////////////////////////////////////

//let u know if the point p is in the disk that a,b make as diameter
bool inDisk(pt a, pt b, pt p) {
    return dot(a-p, b-p) <= EPS;
}

//return true if point c is on segment a,b
bool onSegment(pt a, pt b, pt c){
    return orient(a, b, c) == 0 && inDisk(a, b, c);
}

//returns the intersection point of two segments, if it exist
bool properInter(pt a, pt b, pt c, pt d, pt &out) {
    T oa = orient(c,d,a),
            ob = orient(c,d,b),
            oc = orient(a,b,c),
            od = orient(a,b,d);
// Proper intersection exists iff opposite signs
    if (sgn(oa)*sgn(ob) < 0 && sgn(oc)*sgn(od) < 0) {
        out = (a*ob - b*oa) / (ob-oa);
        return true;
    }
    return false;
}

//returns set of intersection range
set<pair<ld,ld>> inters(pt a, pt b, pt c, pt d) {
    set<pair<ld,ld>> s;
    pt out;
    if(a == c || a == d){
        s.insert(make_pair(a.x, a.y));
    }
    if(b == c || b == d){
        s.insert(make_pair(b.x, b.y));
    }
    if(s.size()) return s;

    if (properInter(a,b,c,d,out)) return {make_pair(out.x, out.y)};
    if (onSegment(c,d,a)) s.insert(make_pair(a.x, a.y));
    if (onSegment(c,d,b)) s.insert(make_pair(b.x, b.y));
    if (onSegment(a,b,c)) s.insert(make_pair(c.x, c.y));
    if (onSegment(a,b,d)) s.insert(make_pair(d.x, d.y));

    return s;
}

//returns the distance from point to segment
ld segPoint(pt a, pt b, pt p) {
    if (a != b) {
        line l(a,b);
        if (l.cmpProj(a,p) && l.cmpProj(p,b)) // if closest to projection
            return l.dist(p); // output distance to line
    }
    return min(abs(p-a), abs(p-b)); // otherwise distance to A or B
}


//returns the distance from segment to segment
ld segSeg(pt a, pt b, pt c, pt d) {
    pt dummy;
    if (properInter(a,b,c,d,dummy))
        return 0;
    return min({segPoint(a,b,c), segPoint(a,b,d),
                segPoint(c,d,a), segPoint(c,d,b)});
}

////////////////////// polygons ////////////////////////////

ld areaTriangle(pt a, pt b, pt c) {
    return abs(cross(b-a, c-a)) / 2.0;
}

//area will be negative if the polygon was taken clock wise
ld areaPolygon(vector<pt> p) {
    ld area = 0.0;
    for (int i = 0, n = p.size(); i < n; i++) {
        area += cross(p[i], p[(i+1)%n]); // wrap back to 0 if i == n - 1
    }
    return abs(area) / 2.0;
}

bool above(pt a, pt p) {
    return p.y >= a.y;
}
// check if [PQ] crosses ray from A
bool crossesRay(pt a, pt p, pt q) {
    return (above(a,q)- above(a,p)) * orient(a,p,q) > 0;
}

bool inPolygon(vector<pt> p, pt a, bool strict = true) {
    int numCrossings = 0;
    for (int i = 0, n = p.size(); i < n; i++) {
        if (onSegment(p[i], p[(i+1)%n], a))
            return !strict;
        numCrossings += crossesRay(a, p[i], p[(i+1)%n]);
    }
    return numCrossings & 1; // inside if odd number of crossings
}


// area of regular polygon of n sides
double area(double n,double L) {
    return n * L / (4.00 * tan(PI / n)) * L;
}

// area of regular polygon of n sides using apothem (height from center to any center of its sides)
// can be modified to take b (side lenght) in input
double area_apothem(int n,int h) {
    double b = h * tan(PI/n);
    return b*h*n/2.0;
}

// area of polygon of n sides using circumraduis r
double area_cr(int n,int r) {
    return n*r*r*sinl(2.0*PI/n)/2.0;
}

//area of perfect star inside a regular polygon
double ps_area(int n, int L) {
    return n*L/(2.0*tan(2.0*PI/n))*L;
}

//inner angle of n sided regular polygon
double angle(int n) {
    return (n-2)*PI/n;
}