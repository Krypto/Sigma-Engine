#include "Rect.h"

sig::Rect::Rect(float x, float y, float w, float h)
{
    this->x = x;
    this->y = y;
    this->width = w;
    this->height = h;
}

bool sig::Rect::Intersects(Rect b)
{
    return (x > b.x && x + width < b.x + b.width && y > b.y && y + height < b.y + b.height);
}

bool sig::Rect::HasPoint(float x, float y)
{
    return (x > this->x && x < this->x + this->width && y > this->y && y < this->y + this->height);
}

sig::Rect sig::Rect::Inflated(float px, float py)
{
	Rect ret = Rect(x, y, width, height);
	ret.x -= px;
	ret.y -= py;
	ret.width += px * 2;
	ret.height += py * 2;
	
	return ret;
}
