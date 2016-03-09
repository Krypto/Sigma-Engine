#include "Color.h"
#include "SIG_Utilities.h"

sig::Color sig::Color::BLACK 		= Color();
sig::Color sig::Color::WHITE 		= Color(1, 1, 1, 1);
sig::Color sig::Color::DARK_GRAY 	= Color(0.3f, 0.3f, 0.3f, 1);
sig::Color sig::Color::GRAY 		= Color(0.5f, 0.5f, 0.5f, 1);
sig::Color sig::Color::RED			= Color(1, 0, 0, 1);
sig::Color sig::Color::GREEN 		= Color(0, 1, 0, 1);
sig::Color sig::Color::BLUE 		= Color(0, 0, 1, 1);
sig::Color sig::Color::DARK_RED 	= Color(0.5f, 0, 0, 1);
sig::Color sig::Color::DARK_GREEN 	= Color(0, 0.5f, 0, 1);
sig::Color sig::Color::DARK_BLUE 	= Color(0, 0, 0.5f, 1);
sig::Color sig::Color::YELLOW		= Color(1, 1, 0, 1);
sig::Color sig::Color::DARK_YELLOW 	= Color(0.5f, 0.5f, 0, 1);
sig::Color sig::Color::PINK 		= Color(0.9f, 0, 0.6f, 1);
sig::Color sig::Color::MAGENTA 		= Color(1, 0, 1, 1);
sig::Color sig::Color::CYAN 		= Color(0, 1, 1, 1);
sig::Color sig::Color::TRANSPARENT 	= Color(0, 0, 0, 0);

sig::Color::Color(float r, float g, float b, float a)
{
	this->r = SIG_CLAMPF(r);
	this->g = SIG_CLAMPF(g);
	this->b = SIG_CLAMPF(b);
	this->a = SIG_CLAMPF(a);
}

sig::Color::Color(unsigned int hex, float alpha)
{
	a = SIG_CLAMPF(alpha);
	
	r = SIG_CLAMPF(float(((hex >> 16) 	& 0xFF) / 255.0));
	g = SIG_CLAMPF(float(((hex >> 	8) 	& 0xFF) / 255.0));
	b = SIG_CLAMPF(float(( hex 			& 0xFF) / 255.0));
}

sig::Color sig::Color::Blend(Color b, float factor)
{
	return Color(
		(this->r * factor) + (b.r * (1 - factor)),
		(this->g * factor) + (b.g * (1 - factor)),
		(this->b * factor) + (b.b * (1 - factor)),
		(this->a * factor) + (b.a * (1 - factor))
	);
}

sig::Color sig::Color::FromColorAlpha(const Color& color, float a)
{
	return Color(color.r, color.g, color.b, a);
}

sig::Color sig::Color::operator -(const sig::Color &_b)
{
	return Color(_b.r - r, _b.g - g, _b.b - b, _b.a);
}

sig::Color sig::Color::Brightness(float factor)
{
	Color ret;
	
	ret.r = SIG_CLAMPF(r * factor);
	ret.g = SIG_CLAMPF(g * factor);
	ret.b = SIG_CLAMPF(b * factor);
	ret.a = a;
	
	return ret;
}
