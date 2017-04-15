/** ======================================================================== **\
	PackedBitSet

| Description:


| Version:
	3.0.0

| License:
	Copyright (c) 2016 Double V

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1.	The origin of this software must not be misrepresented; you must not
		claim that you wrote the original software. If you use this software
		in a product, an acknowledgement in the product documentation would be
		appreciated but is not required.
	2.	Altered source versions must be plainly marked as such, and must not be
		misrepresented as being the original software.
	3.	This notice may not be removed or altered from any source distribution.
\** ======================================================================== **/


#if defined _inc_tdw_bitset
	#undef _inc_tdw_bitset
#endif

#if defined _TDW_BLIB_BITSET
	#endinput
#endif

#define _TDW_BLIB_BITSET

#tryinclude "tdw_bit"

#if !defined _TDW_BIT_INC_
	#error Library 'tdw_bit' is not found! Download it!
#endif

// All tags for bitset-functions
#define _TDW_BITSET_TYPES_ { TDW_BITSET_1, TDW_BITSET_2, TDW_BITSET_4, TDW_BITSET_8, TDW_BITSET_16 }

#if !defined NULL
	const NULL = 0;
#endif

// Check if the tags are registered
#if defined TDW_BITSET_1_TAG || defined TDW_BITSET_16_TAG
	#error The tags have already registered.
#endif

#if !defined cellbytes
	const cellbytes = cellbits / charbits;
#endif

#define Bit:%1<%2,%3> TDW_BITSET_%3:%1[((%2) + BITSET_SIZE_PART_%3) >>> _:(BIT32 - BIT%3)]

// Get the tag ids to detect them in the functions because the compiler registers
// the tags at compile time
const
	TDW_BITSET_NONE:BITSET_NONE_ = TDW_BITSET_NONE:NULL,
	TDW_BITSET_1:BITSET_TAG_1_   = TDW_BITSET_1:   NULL,
	TDW_BITSET_2:BITSET_TAG_2_   = TDW_BITSET_2:   NULL,
	TDW_BITSET_4:BITSET_TAG_4_   = TDW_BITSET_4:   NULL,
	TDW_BITSET_8:BITSET_TAG_8_   = TDW_BITSET_8:   NULL,
	TDW_BITSET_16:BITSET_TAG_16_ = TDW_BITSET_16:  NULL,
	TDW_BITSET_NONE_TAG          = tagof(BITSET_NONE_  ),
	TDW_BITSET_1_TAG             = tagof(BITSET_TAG_1_ ),
	TDW_BITSET_2_TAG             = tagof(BITSET_TAG_2_ ),
	TDW_BITSET_4_TAG             = tagof(BITSET_TAG_4_ ),
	TDW_BITSET_8_TAG             = tagof(BITSET_TAG_8_ ),
	TDW_BITSET_16_TAG            = tagof(BITSET_TAG_16_);

// We declare the constants with a "const" qualifier (not #define)
// because they computes the values at compile time.
const
	BITSET_SIZE_PART_1           = cellbits - 1,
	BITSET_SIZE_PART_2           = cellbits / 2 - 1,
	BITSET_SIZE_PART_4           = cellbits / 4 - 1,
	BITSET_SIZE_PART_8           = cellbits / 8 - 1,
	BITSET_SIZE_PART_16          = cellbits / 16 - 1;

enum E_TDW_BITSET_SHIFT
{
	BIT1,  // 0
	BIT2,  // 1
	BIT4,  // 2
	BIT8,  // 3
	BIT16, // 4
	BIT32  // 5
};

// Sets a value in bitset.
stock BitSet_Set(_TDW_BITSET_TYPES_:_bitset[], index, value, size = sizeof(_bitset),
	tag = tagof(_bitset))
{
	new
		part_size_in_cell = tag - TDW_BITSET_NONE_TAG,
		array_shift  = _:BIT32 - (--part_size_in_cell),
		bit_shift_in_array = (index & ((1 << array_shift) - 1)) << part_size_in_cell,
		part_limit = (1 << (1 << part_size_in_cell)) - 1;

	if (_:(-1 < (index >>>= array_shift) < size) == 0)
		return 0;

	new
		addr;

	// Get the cell address in the _bitset
	#emit load.s.alt _bitset
	#emit load.s.pri index
	#emit idxaddr
	#emit stor.s.pri addr
	// _bitset[index] = ([addr] & ~(part_limit << bit_shift_in_array)) |
	//   ((part_limit & value) << bit_shift_in_array)
	#emit load.s.pri part_limit
	#emit load.s.alt bit_shift_in_array
	#emit shl
	#emit invert
	#emit lref.s.alt addr
	#emit and
	#emit push.pri
	#emit load.s.pri part_limit
	#emit load.s.alt value
	#emit and
	#emit load.s.alt bit_shift_in_array
	#emit shl
	#emit pop.alt
	#emit or
	#emit sref.s.pri addr

	return 1;
}

// Gets a value from bitset.
stock BitSet_Get(_TDW_BITSET_TYPES_:_bitset[], index, size = sizeof(_bitset),
	tag = tagof(_bitset))
{
	new
		part_size_in_cell = tag - TDW_BITSET_NONE_TAG,
		array_shift  = _:BIT32 - (--part_size_in_cell),
		bit_shift_in_array = (index & ((1 << array_shift) - 1)) << part_size_in_cell,
		part_limit = (1 << (1 << part_size_in_cell)) - 1;

	if (_:(-1 < (index >>>= array_shift) < size) == 0)
		return 0;

	const STACK_BYTES = 4 * cellbytes;

	// Get the value in the _bitset
	#emit load.s.alt _bitset
	#emit load.s.pri index
	#emit lidx
	// (_bitset value >>> bit_shift_in_array) | part_limit
	#emit load.s.alt bit_shift_in_array
	#emit sshr
	#emit load.s.alt part_limit
	#emit and
	#emit stack      STACK_BYTES
	#emit retn

	return 0;  // make compiler happy.
}

// Flips a value
stock BitSet_Flip(_TDW_BITSET_TYPES_:_bitset[], index, mask = -1, size = sizeof(_bitset),
	tag = tagof(_bitset))
{
	new
		part_size_in_cell = tag - TDW_BITSET_NONE_TAG,
		array_shift  = _:BIT32 - (--part_size_in_cell),
		bit_shift_in_array = (index & ((1 << array_shift) - 1)) << part_size_in_cell,
		part_limit = (1 << (1 << part_size_in_cell)) - 1;

	if (_:(-1 < (index >>>= array_shift) < size) == 0)
		return 0;

	new
		addr;

	// Get the cell address in the _bitset
	#emit load.s.alt _bitset
	#emit load.s.pri index
	#emit idxaddr
	#emit stor.s.pri addr
	// (_:_bitset[index] = _bitset[index] ^ ((part_limit & mask) << bit_shift_in_array));
	#emit load.s.pri part_limit
	#emit load.s.alt mask
	#emit and
	#emit load.s.alt bit_shift_in_array
	#emit shl
	#emit lref.s.alt addr
	#emit xor
	#emit sref.s.pri addr

	return 1;
}

// Counts the bits in the cell
stock BitSet_Count(_TDW_BITSET_TYPES_:_bitset[], index, size = sizeof(_bitset),
	tag = tagof(_bitset))
{
	new
		part_size_in_cell = tag - TDW_BITSET_NONE_TAG,
		array_shift  = _:BIT32 - (--part_size_in_cell),
		bit_shift_in_array = (index & ((1 << array_shift) - 1)) << part_size_in_cell,
		part_limit = (1 << (1 << part_size_in_cell)) - 1;

	if (_:(-1 < (index >>>= array_shift) < size) == 0)
		return 0;
	{}

	#emit addr.alt barray
	#emit load.s.pri idx
	#emit lidx
	#emit load.i
	#emit load.s.alt var
	#emit shr
	#emit load.s.alt lim
	#emit and

	#emit push.pri
	#emit shr.c.pri 1
	#emit const.alt 0x55555555
	#emit and
	#emit pop.alt
	#emit sub.alt

	#emit push.pri
	#emit shr.c.pri 2
	#emit const.alt 0x33333333
	#emit and
	#emit swap.pri
	#emit and
	#emit pop.alt
	#emit add

	#emit move.alt
	#emit shr.c.pri 4
	#emit add
	#emit const.alt 0xf0f0f0f
	#emit and
	#emit smul.c 0x1010101
	#emit shr.c.pri 24

	#emit stack 0x10
	#emit retn

	return 0;  // make compiler happy.
}

/// ----

stock BitSet_Any(_TDW_BITSET_TYPES_:_bitset[], index, size = sizeof(_bitset),
	tag = tagof(_bitset))
{
	return BitSet_Get(_bitset, index, size, tag);
}

stock BitSet_None(_TDW_BITSET_TYPES_:_bitset[], index, size = sizeof(_bitset),
	tag = tagof(_bitset))
{
	return BitSet_Get(_bitset, index, size, tag) == 0;
}
