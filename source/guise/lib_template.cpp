#include "guise/lib_template.hpp"

LibTemplate::LibTemplate() :
    m_x(0),
    m_y(0)
{ }

void LibTemplate::setX(const int x)
{
    m_x = x;
}

void LibTemplate::setY(const int y)
{
    m_y = y;
}

int LibTemplate::getSum() const
{
    return m_x + m_y;
}