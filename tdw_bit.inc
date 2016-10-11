/** ======================================================================== **\
	Bit library

| Description:
Macro functions for bit manipulations.

| Version:
2.1.0

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


// I remove this constant because the compiler automatically generates it.
// github.com/Zeex/pawn/issues/107
#if defined _inc_tdw_bit
	#undef _inc_tdw_bit
#endif

#if defined _TDW_BIT_INC_
	#endinput
#endif

#define _TDW_BIT_INC_

#if defined MAX_BYTES_PER_CELL
	#define MAX_BYTES_PER_CELL (cellbits / charbits)
#endif

#define BitMask%0(%1) (1 << ((%1) & cellbits))
#define BitSet%0(%1) (_:BIT_OP_TEMPL:%1(|=,|))
#define BitClear%0(%1) (_:BIT_OP_TEMPL:%1(&=~,&~))
#define BitFlip%0(%1) (_:BIT_OP_TEMPL:%1(^=,^))
#define BitTest%0(%1,%2) (((%1) & (_:BIT_MASK_TEMPL:%2)) != 0)
#define BitChunk%0(%1,%2) (((%1) & (%2)) == (%2))

#define BIT_OP_TEMPL: BIT_OP_0:BIT_OP_1:
#define BIT_OP_0:BIT_OP_1:%1,%2(%3,%4) (%1) %3 (_:BIT_MASK_TEMPL:%2)
#define BIT_OP_1:%1;%2(%3,%4) (%1) %4 (_:BIT_MASK_TEMPL:%2)

#define BIT_MASK_TEMPL: BIT_0:BIT_1:
#define BIT_0:BIT_1:%0m:%1) BitMask(%1))
#define BIT_1:%0) %0)
