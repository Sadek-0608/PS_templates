#include <bits/stdc++.h>

struct P {
    double x, y;
    P operator-(const P &b) const {
        return {x - b.x, y - b.y};
    }
    void operator-=(const P &b) {
        x -= b.x;
        y -= b.y;
    }

    // Vector addition: this + b
    P operator+(const P &b) const {
        return {x + b.x, y + b.y};
    }

    // Add and assign: this += b
    void operator+=(const P &b) {
        x += b.x;
        y += b.y;
    }
    // Scalar multiplication: this * k
    P operator*(double k) const {
        return {x * k, y * k};
    }

    // Scalar division: this / k (⚠️ Integer division)
    P operator/(double k) const {
        return {x / k, y / k};
    }

    double operator*(const P &b) const {
        return x * b.y - y * b.x;
    }

    // Dot product: this • b = x*b.x + y*b.y
    // Geometric meaning: projection component, used for angle and length
    double operator&(const P &b) const {
        return x * b.x + y * b.y;
    }
};

bool intersectSegments(P a, P b, P c, P d, P &intersect) {
    double d1 = (b - a) * (d - c);  // cross(b-a, d-c)
    double d2 = (c - a) * (d - c);
    double d3 = (b - a) * (c - a);

    if (fabs(d1) < 1e-9) return false; // Parallel

    double t1 = d2 / d1;
    double t2 = d3 / d1;
    intersect = a + (b - a) * t1;

    if (t1 < -1e-9 || t1 > 1 + 1e-9 || t2 < -1e-9 || t2 > 1 + 1e-9) return false;

    return true;
}

// Polygon Cut (Single Line Cut): Cuts poly by infinite directed line a -> b.
// Keeps the sub-polygon lying to the LEFT (CCW side) of line a -> b. O(N)
vector<P> polygonCut(const vector<P> &poly, P a, P b) {
    vector<P> result;
    int n = poly.size();
    for (int i = 0; i < n; ++i) {
        P curr = poly[i];
        P next = poly[(i + 1) % n];
        double side_curr = (b - a) * (curr - a);
        double side_next = (b - a) * (next - a);
        bool inside_curr = side_curr > 1e-9;
        bool inside_next = side_next > 1e-9;

        if (inside_curr) result.push_back(curr);
        if (inside_curr != inside_next) {
            P r;
            intersectSegments(a, b, curr, next, r);
            result.push_back(r);
        }
    }
    return result;
}

// Polygon Clipping (Sutherland-Hodgman): Intersects subject polygon with clipper.
// NOTE: `clipper` vertices MUST be ordered Counter-Clockwise (CCW). O(N * M)
vector<P> clipPolygon(const vector<P> &subject, const vector<P> &clipper) {
    vector<P> result = subject;
    int n = clipper.size();
    for (int i = 0; i < n; ++i) {
        P a = clipper[i];
        P b = clipper[(i + 1) % n];
        result = polygonCut(result, a, b);  // cut against one edge
    }
    return result;
}
// Returns:
//  0 => outside
//  1 => on the edge
//  2 => inside
int point_in_polygon(const vector<P> &poly, const P &pt) {
    int n = poly.size();
    bool inside = false;

    for (int i = 0; i < n; ++i) {
        P a = poly[i], b = poly[(i + 1) % n];

        // Check if point lies exactly on the edge (segment)
        P ab = b - a;
        P ap = pt - a;
        P bp = pt - b;
        if (fabs(ab * ap) < 1e-9 && (ap & ab) >= 0 && (bp & ab) <= 0)
            return 1;  // On edge

        // Ray casting: check for crossing horizontal ray to the right
        if ((a.y > pt.y) != (b.y > pt.y)) {
            double x_intersect = a.x + (b.x - a.x) * (pt.y - a.y) / (b.y - a.y);
            if (x_intersect > pt.x)
                inside = !inside;
        }
    }
    return inside ? 2 : 0;
}
// Returns true if polygon p is convex (in order, not necessarily CCW)
bool is_convex(const vector<P>& p) {
    bool s[3] = {0, 0, 0};
    int n = p.size();
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        int k = (j + 1) % n;
        double cross = (p[j] - p[i]) * (p[k] - p[i]);  // cross product
        s[sign(cross) + 1] = 1;
        if (s[0] && s[2]) return false;
    }
    return true;
}

// Point in Convex Polygon Test in O(log N)
// Precondition: poly vertices must be given in strict CCW order, with poly[0] as origin.
// Returns: true if point p is strictly inside or on the boundary.
bool pointInConvexPolygon(const vector<P> &poly, const P &p) {
    int n = poly.size();
    if (n < 3) return false;

    // Check if p is to the right of ray poly[0]->poly[1] or poly[0]->poly[n-1]
    if (cross(poly[0], poly[1], p) < -1e-9 || cross(poly[0], poly[n - 1], p) > 1e-9)
        return false;

    // Binary search for the wedge containing p
    int l = 1, r = n - 1;
    while (r - l > 1) {
        int mid = (l + r) / 2;
        if (cross(poly[0], poly[mid], p) >= -1e-9) l = mid;
        else r = mid;
    }

    // Test if p lies inside the triangle formed by poly[0], poly[l], poly[l+1]
    return cross(poly[l], poly[l + 1], p) >= -1e-9;
}

// Maximum Diameter of a Convex Polygon using Rotating Calipers. O(N)
double convexPolygonDiameter(const vector<P> &poly) {
    int n = poly.size();
    if (n <= 1) return 0;
    if (n == 2) return sqrt(dist2(poly[0], poly[1]));

    double max_d2 = 0;
    int j = 1;
    for (int i = 0; i < n; ++i) {
        int ni = (i + 1) % n;
        while (cross(sub(poly[ni], poly[i]), sub(poly[(j + 1) % n], poly[i])) >
               cross(sub(poly[ni], poly[i]), sub(poly[j], poly[i]))) {
            j = (j + 1) % n;
               }
        max_d2 = max({max_d2, dist2(poly[i], poly[j]), dist2(poly[ni], poly[j])});
    }
    return sqrt(max_d2);
}


///////////rectangles//////////////

struct Rect {
    P bl, tr;
};

Rect intersectRect(const Rect &r1, const Rect &r2) {
    long long x1 = max(r1.bl.x, r2.bl.x);
    long long y1 = max(r1.bl.y, r2.bl.y);
    long long x2 = min(r1.tr.x, r2.tr.x);
    long long y2 = min(r1.tr.y, r2.tr.y);

    if (x1 >= x2 || y1 >= y2) return {{0, 0}, {0, 0}}; // No overlap
    return {{x1, y1}, {x2, y2}};
}

// 3. Total Union Area of 2 Rectangles. O(1)
long long unionArea2(const Rect &r1, const Rect &r2) {
    return area(r1) + area(r2) - area(intersectRect(r1, r2));
}

// 4. Minimum Bounding Box enclosing a set of rectangles. O(N)
Rect boundingBox(const vector<Rect> &rects) {
    if (rects.empty()) return {{0, 0}, {0, 0}};
    long long min_x = rects[0].bl.x, min_y = rects[0].bl.y;
    long long max_x = rects[0].tr.x, max_y = rects[0].tr.y;
    for (const auto &r : rects) {
        min_x = min(min_x, r.bl.x);
        min_y = min(min_y, r.bl.y);
        max_x = max(max_x, r.tr.x);
        max_y = max(max_y, r.tr.y);
    }
    return {{min_x, min_y}, {max_x, max_y}};
}

// 5. Point Containment Test. O(1)
bool containsPoint(const Rect &r, const P &p) {
    return p.x >= r.bl.x && p.x <= r.tr.x && p.y >= r.bl.y && p.y <= r.tr.y;
}

// 6. Check if R1 is strictly inside R2. O(1)
bool containsRect(const Rect &r2, const Rect &r1) {
    return r1.bl.x >= r2.bl.x && r1.tr.x <= r2.tr.x &&
           r1.bl.y >= r2.bl.y && r1.tr.y <= r2.tr.y;
}