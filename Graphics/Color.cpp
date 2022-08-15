#include "Color.h"

MyEngine::Color::Color()
	:color(0)
{
}

MyEngine::Color::Color(unsigned int val)
	:color(val)
{
}

MyEngine::Color::Color(BYTE r, BYTE g, BYTE b)
	: Color(r, g, b, 255)
{
}

MyEngine::Color::Color(BYTE r, BYTE g, BYTE b, BYTE a)
{
	rgba[0] = r;
	rgba[1] = g;
	rgba[2] = b;
	rgba[3] = a;
}

MyEngine::Color::Color(const Color& src)
{
	this->color = src.color;
}

MyEngine::Color& MyEngine::Color::operator=(const Color& src)
{
	if (src != *this)
	{
		this->color = src.color;
		return *this;
	}
}

bool MyEngine::Color::operator==(const Color& rhs) const
{
	return (this->color == rhs.color);
}

bool MyEngine::Color::operator!=(const Color& rhs) const
{
	return (this->color != rhs.color);
}

constexpr BYTE MyEngine::Color::GetR() const
{
	return rgba[0];
}

void MyEngine::Color::SetR(BYTE r)
{
	rgba[0] = r;
}

constexpr BYTE MyEngine::Color::GetG() const
{
	return rgba[1];
}

void MyEngine::Color::SetG(BYTE g)
{
	rgba[1] = g;
}

constexpr BYTE MyEngine::Color::GetB() const
{
	return rgba[2];
}

void MyEngine::Color::SetB(BYTE b)
{
	rgba[2] = b;
}

constexpr BYTE MyEngine::Color::GetA() const
{
	return rgba[3];
}

void MyEngine::Color::SetA(BYTE a)
{
	rgba[3] = a;
}
