#include <iostream>
#include "../../model/Utilities/Shape/Shape.h"
#include <cmath>
#include <assert.h>

using namespace std;

int main()
{
    Shape sh1(Vector(1, 1), SQUARE, 10);
    Shape sh2(Vector(-5, -7.5), CIRCLE, 12);
    Shape sh0;

    //Accessors tests
    //
    //Getters
    //
    assert((sh1.getSize() == 10) && (sh2.getSize() == 12));
    assert(sh1.getCenter() + sh2.getCenter() == Vector(-4, -6.5));
    assert(sh1.getType() == SQUARE && sh2.getType() == CIRCLE);

    //Setters
    //
    sh1.setCenter(Vector(15, -3.14));
    sh2.setCenter(Vector(2.71, 0));
    sh1.setSize(20);
    sh2.setType(SQUARE);
    sh1.setType(CIRCLE);

    assert(sh1.getSize() == 20);
    assert(sh1.getCenter() + sh2.getCenter() == Vector(17.71, -3.14));
    assert(sh1.getType() == CIRCLE && sh2.getType() == SQUARE);

    // Corner methods
    assert(sh1.getLeftBottom() == Vector(5, -13.14));
    assert(sh2.getRightTop() == Vector(8.71, 6));

    // Hit-test methods
    //
    // Vectors

    assert(sh1.hitTest(Vector(8, -10.14))
           && !sh1.hitTest(Vector(7, -10.14)));

    assert(sh2.hitTest(sh2.getCenter() + Vector(5, -4)) &&
           !sh2.hitTest(sh2.getCenter() + Vector(7, 0)) &&
           !sh2.hitTest(sh2.getCenter() + Vector(2, -9)));

    assert(sh2.hitTest(sh2.getCenter()) &&
           sh2.hitTest(sh2.getRightTop()));

    // Shapes

    Shape sh3(Vector(0, -3.14), CIRCLE, 10);
    Shape sh4(Vector(2.71, 10), SQUARE, 8);

    assert(sh1.hitTest(sh3) && sh3.hitTest(sh1));
    assert(sh2.hitTest(sh4) && sh4.hitTest(sh2));
    assert(sh1.hitTest(sh2) && sh2.hitTest(sh1));
    assert(sh2.hitTest(sh3) && sh3.hitTest(sh2));
    assert(!sh1.hitTest(sh4) && !sh4.hitTest(sh1));
    assert(!sh3.hitTest(sh4) && !sh4.hitTest(sh3));

    sh3.setCenter(Vector(-8, 6));
    sh4.setCenter(Vector(-1 ,14));
    assert(sh3.hitTest(sh4) && sh4.hitTest(sh3));
    assert(sh3.hitTest(sh2) && sh2.hitTest(sh3));

    sh4.setCenter(Vector(-1000, 0));
    assert(!sh4.hitTest(sh2) && !sh2.hitTest(sh4));

    sh4.setCenter(Vector(-7.29, 10));
    assert(sh4.hitTest(sh2) && sh2.hitTest(sh4));

    sh4.setCenter(Vector(-7.29, 11));
    assert(!sh4.hitTest(sh2) && !sh2.hitTest(sh4));

    sh4.setCenter(Vector(8, 9));
    assert(sh4.hitTest(sh2) && sh2.hitTest(sh4));

    sh4.setCenter(Vector(-7, 9));
    assert(sh4.hitTest(sh2) && sh2.hitTest(sh4));
    
    assert(!sh1.hitTest(sh3) && !sh3.hitTest(sh1));

    // Intersect
    sh1.setCenter(Vector(0, 0));
    sh1.setSize(2);
    sh2.setSize(1);

    sh2.setCenter(Vector(0, 0));
    assert(!sh1.intersect(sh2) &&
           sh2.intersect(sh1) & (LEFT | BOTTOM | RIGHT | TOP));

    sh2.setCenter(Vector(5, 5));
    assert(sh1.intersect(sh2) & NO_INTERSECTION &&
           sh2.intersect(sh1) & NO_INTERSECTION);

    sh2.setSize(10);
    assert(sh1.intersect(sh2) & (TOP | RIGHT) &&
           sh2.intersect(sh1) & (LEFT | BOTTOM));

    sh2.setSize(1);
    sh2.setCenter(Vector(0, -1));
    assert(sh1.intersect(sh2) & BOTTOM &&
           sh2.intersect(sh1) & (TOP | LEFT | RIGHT));

    return 0;
}

