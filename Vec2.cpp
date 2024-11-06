#include "Vec2.h"
#include <cmath>
#include <iostream>

Vec2::Vec2() : x(0), y(0) {}

Vec2::Vec2(float x, float y) : x(x), y(y) {}

bool Vec2::operator == (const Vec2 & rhs) const
{
    return x == rhs.x && y == rhs.y;
}

bool Vec2::operator != (const Vec2 & rhs) const
{
    return x != rhs.x || y != rhs.y;
}

Vec2 Vec2::operator + (const Vec2 & rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2 & rhs) const  
{
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator / (const float val) const  
{
    return Vec2(x / val, y / val);
}

Vec2 Vec2::operator * (const float val) const  
{
    return Vec2(x * val, y * val);
}

void Vec2::operator += (const Vec2 & rhs)  
{
    this->x += rhs.x;
    this->y += rhs.y;
}

void Vec2::operator -= (const Vec2 & rhs)  
{
    this->x -= rhs.x;
    this->y -= rhs.y;
}

void Vec2::operator *= (const float val)
{
    this->x *= val;
    this->y *= val;
}

void Vec2::operator /= (const float val)
{
    this->x /= val;
    this->y /= val;
}

float Vec2::dist(const Vec2 & rhs) const
{
    float dx = rhs.x - this->x;
    float dy = rhs.y - this->y;
    return std::sqrtf(dx * dx + dy * dy);
}

void Vec2::normalize()
{
    float length = std::sqrtf(x*x + y*y);
    if (length != 0)
    {
        this->x /= length;
        this->y /= length;
    }
}

double Vec2::length()
{
    return std::sqrtf(x*x + y*y);
}

void Vec2::print()
{
    std::cout << "(" << this->x << "," << this->y << ")" << std::endl;
}