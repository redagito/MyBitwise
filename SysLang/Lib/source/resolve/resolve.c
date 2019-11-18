#include "syl/resolve/resolve.h"

#include "syl/common/map.h"
#include "syl/type.h"

static void complete_type(type_t* type)
{

}

// Actually part of type.h/c
// Moved to resolve to avoid cyclic dependency
// Maps base pointer to type to same type with const qualifier
static map_t cached_const_types;

/**
 * Returns const type of base type
 */
static type_t* type_const(type_t* base)
{
	if (base->type == TYPE_CONST)
	{
		// Base already has const qualifier
		return base;
	}

	// Same type as base with const qualifier may already be cached
	type_t* type = map_get(&cached_const_types, base);
	if (type == NULL)
	{
		// Build new type with const qualifier
		complete_type(base);
		type = type_alloc(TYPE_CONST);
		type->non_modifiable = true;
		type->size = base->size;
		type->align = base->align;
		type->base = base;
		map_put(&cached_const_types, base, type);
	}
	return type;
}