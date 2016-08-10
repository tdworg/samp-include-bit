/** ======================================================================== **\
	Bit library

| Description:
Macro functions for bit manipulations.

| Version:
1.1.2

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
#if defined _inc_bit
	#undef _inc_bit
#endif

#if defined _TDW_BIT_INC_
	#endinput
#endif

#define _TDW_BIT_INC_


#define BitMask(%0) (1 << ((%0) % cellbits))

#define BitVSet(%0,%1)   ((%0) |=  (%1))
#define BitVClear(%0,%1) ((%0) &= ~(%1))
#define BitVFlip(%0,%1)  ((%0) ^=  (%1))
#define BitVTest(%0,%1)  (((%0) & (%1)) != 0)

#define BitMSet(%0,%1)   ((%0) |=  BitMask((%1)))
#define BitMClear(%0,%1) ((%0) &= ~BitMask((%1)))
#define BitMFlip(%0,%1)  ((%0) ^=  BitMask((%1)))
#define BitMTest(%0,%1)  (((%0) & BitMask(%1)) != 0)

#define MAX_BYTES_PER_CELL (cellbits / charbits)

#define BitChunk(%0,%1) (((%0) & (%1)) == (%1))
