/*
    Copyright (c) 2012, pr061012 Team.
    See the LICENSE file for copying permission.
*/

#include "../../../common/BasicDefines.h"

#include "Object.h"

//******************************************************************************
// CONSTRUCTOR/DESTRUCTOR.
//******************************************************************************

uint Object::CURRENT_ID = 0;

Object::Object(ObjectType type, bool solidity, bool immortality) :
    id(CURRENT_ID++),
    type(type),
    shape(Shape(Vector(0, 0), SHP_DEFAULT, SZ_DEFAULT)),
    destroyed(false),
    immortality(immortality),
    solidity(solidity),
    angle(0)
{
}

Object::~Object()
{
}

//******************************************************************************
// DESTROYED FLAG.
//******************************************************************************

void Object::markAsDestroyed()
{
    this -> destroyed = true;
}

bool Object::isDestroyed() const
{
    return this -> destroyed;
}

//******************************************************************************
// IMMORTALITY FLAG.
//******************************************************************************

void Object::makeMortal()
{
    this -> immortality = false;
}

void Object::makeImmortal()
{
    this -> immortality = true;
}

bool Object::isImmortal() const
{
    return this -> immortality;
}

//******************************************************************************
// SOLIDITY.
//******************************************************************************

void Object::makeSolid()
{
    this -> solidity = true;
}

void Object::makeNonSolid()
{
    this -> solidity = false;
}

bool Object::isSolid() const
{
    return this -> solidity;
}

//******************************************************************************
// DANGEROUS.
//******************************************************************************

uint Object::getDangerLevel() const
{
    return this -> danger_level;
}

void Object::setDangerLevel(uint danger_level)
{
    this -> danger_level = danger_level;
}

//******************************************************************************
// OBJECT TYPE AND ID.
//******************************************************************************

ObjectType Object::getType() const
{
    return this -> type;
}

uint Object::getObjectID() const
{
    return this -> id;
}

//******************************************************************************
// OBJECT SHAPE AND ANGLE.
//******************************************************************************

void Object::setShapeType(ShapeType type)
{
    this -> shape.setType(type);
}

void Object::setShapeSize(double size)
{
    this -> shape.setSize(size);
}

Shape Object::getShape() const
{
    return this -> shape;
}

void Object::setAngle(double new_var)
{
    this -> angle = new_var;
}

double Object::getAngle() const
{
    return this -> angle;
}

void Object::setCoords(const Vector& coords)
{
    shape.setCenter(coords);
}

const Vector& Object::getCoords() const
{
    return shape.getCenter();
}

Object::operator const ViewObject *()
{
    Vector v = this -> getShape().getCenter();
    ViewObject* ret = new ViewObject(this -> getType(), v.getX(), v.getY());
    return ret;
}

