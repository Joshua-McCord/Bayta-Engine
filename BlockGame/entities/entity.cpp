#include "entity.h"

int generate_entity_id()
{
	return rand() % UINT_MAX;
}