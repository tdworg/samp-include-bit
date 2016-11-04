/** ======================================================================== **\
	Bit Array

| Version:
2.1.1

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

// Functions that compute the index and the offset of an array.
#define GetBitIndex%0(%1,%2) ((%2) / (tagof(%1) - BIT_ST_TAG))
#define GetBitOffset%0(%1,%2) ((%2) % (tagof(%1) - BIT_ST_TAG))

// All tags for bit-arrays.
#define TDW_BITSET_TYPES {TDW_BITSET_1, TDW_BITSET_2, TDW_BITSET_4, \
	TDW_BITSET_8, TDW_BITSET_16}

// In order to register tags i use const operator to define them. If use
// the macros, the result would have been different.
const
	BIT_ST_NONE:BIT_ST = BIT_ST_NONE:0,
	BIT_ST_TAG = tagof(BIT_ST),
	TDW_BITSET_1:n_bit1 = TDW_BITSET_1:0,
	TDW_BITSET_2:n_bit2 = TDW_BITSET_2:0,
	TDW_BITSET_4:n_bit4 = TDW_BITSET_4:0,
	TDW_BITSET_8:n_bit8 = TDW_BITSET_8:0,
	TDW_BITSET_16:n_bit16 = TDW_BITSET_16:0,
	BIT_1_TAG = tagof(n_bit1),
	BIT_2_TAG = tagof(n_bit2),
	BIT_4_TAG = tagof(n_bit4),
	BIT_8_TAG = tagof(n_bit8),
	BIT_16_TAG = tagof(n_bit16),
	BIT_ST_TAG_B32 = 5;

// This conditionals checks if the tag has already been defined before
// this library. The conditionals is very simple. If the tag id is less than
// the id of the special tag, the tag was already used even before the inclusion
// of this library.
#if BIT_1_TAG < BIT_ST_TAG
	#error tag 'TDW_BITSET_1' already defined!
#elseif BIT_2_TAG < BIT_ST_TAG
	#error tag 'TDW_BITSET_2' already defined!
#elseif BIT_4_TAG < BIT_ST_TAG
	#error tag 'TDW_BITSET_4' already defined!
#elseif BIT_8_TAG < BIT_ST_TAG
	#error tag 'TDW_BITSET_8' already defined!
#elseif BIT_16_TAG < BIT_ST_TAG
	#error tag 'TDW_BITSET_16' already defined!
#endif


enum
	E_BITS
{
	BIT1,
	BIT2,
	BIT4,
	BIT8,
	BIT16,
	BIT32
};


#define Bit:%2[%3] TDW_BITSET_8:TDW_BITSET_T0:%2|||%3|||
#define TDW_BITSET_8:TDW_BITSET_T0:%2<%1>|||%3|||\
	TDW_BITSET_%1:%2[BIT_PART_SIZE(%3,%1)]
#define TDW_BITSET_T0:%2|||%3||| %2[BIT_PART_SIZE(%3,8)]

#define BIT_PART_SIZE(%3,%1) ((%3) + BIT_SIZE_PART_%1) >>> _:(BIT32 - BIT%1)
#define BIT_SIZE_PART_1 (31)
#define BIT_SIZE_PART_2 (15)
#define BIT_SIZE_PART_4 (7)
#define BIT_SIZE_PART_8 (3)
#define BIT_SIZE_PART_16 (1)



stock
	BitSet_Set(TDW_BITSET_TYPES:barray[], idx, value, size = sizeof barray,
		tag = tagof(barray))
{
	new shift = tag - BIT_ST_TAG - 1,
		m = BIT_ST_TAG_B32 - shift,
		var = (idx & ((1 << m) - 1)) << shift,
		lim = (1 << (1 << shift)) - 1;

	idx = idx >>> m;

	if (!(-1 < idx < size))
		return 0;

	(_:barray[idx] &= ~(lim << var));
	(_:barray[idx] |= ((lim & value) << var));

	return 1;
}


stock
	BitSet_Get(TDW_BITSET_TYPES:barray[], idx, size = sizeof barray,
		tag = tagof(barray))
{
	new shift = tag - BIT_ST_TAG - 1,
		m = BIT_ST_TAG_B32 - shift,
		var = (idx & ((1 << m) - 1)) << shift,
		lim = (1 << (1 << shift)) - 1;

	idx = idx >>> m;

	if (!(-1 < idx < size))
		return 0;

	return ((_:barray[idx] >>> var) & lim);
}


stock
	BitSet_Print(TDW_BITSET_TYPES:barray[], size = sizeof barray,
		tag = tagof(barray))
{
	new shift = tag - BIT_ST_TAG - 1,
		m = BIT_ST_TAG_B32 - shift,
		lim = (1 << (1 << _: shift)) - 1,
		index;

	do
	{
		printf("%d",
			(_:barray[index >>> m] >>> ((index & ((1 << m) - 1)) << shift)) &
			lim
		);
	}
	while ((++index >>> m) < size);

	return 0;
}


stock
	BitSet_Count(TDW_BITSET_TYPES:barray[], idx, size = sizeof barray,
		tag = tagof(barray))
{
	new shift = tag - BIT_ST_TAG - 1,
		m = BIT_ST_TAG_B32 - shift,
		var = (idx & ((1 << m) - 1)) << _: shift,
		lim = (1 << (1 << _: shift)) - 1;

	idx = idx >>> m;

	if (!(-1 < idx < size))
	{
		return 0;
	} {} // Zero....

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
	return 0;
}



#undef BIT_ST
#undef n_bit1
#undef n_bit2
#undef n_bit4
#undef n_bit8
#undef n_bit16
#undef BIT_ST_TAG_B32
