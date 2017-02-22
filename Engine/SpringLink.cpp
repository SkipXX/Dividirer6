#include "SpringLink.h"



SpringLink::SpringLink()
{
}

SpringLink::SpringLink(Vec2 * p, float c, float l)
	:
	linkedPoint(p),
	springConstant(c),
	springLength(l)
{
}


SpringLink::~SpringLink()
{
}
