/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2023
* Author: Megan Clinch 041043369, Cameron Ewing 041037946
* Professors: Paulo Sousa
************************************************************

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@                                                               @@@@
@@@																	@@@
@@																	 @@
@	  __   __   ______     __         __   __   ______     ______	  @
@    /\ \ / /  /\  ___\   /\ \       /\ \ / /  /\  ___\   /\__  _\    @
@	 \ \ \'/   \ \  __\   \ \ \____  \ \ \'/   \ \  __\   \/_/\ \/    @
@	  \ \__|    \ \_____\  \ \_____\  \ \__|    \ \_____\    \ \_\    @
@  	   \/_/      \/_____/   \/_____/   \/_/      \/_____/     \/_/    @
@																	  @
@                                                                     @
@           A L G O N Q U I N   C O L L E G E  -  2 0 2 3 W           @
@@																     @@
@@@					M E G A N   &   C A M E R O N					@@@
@@@@															   @@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

*/

/*
************************************************************
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'F', /* Fixed mode (constant size) */
	MODE_ADDIT = 'A', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'M'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls (not need to change) */
#define READER_ERROR (-1)						/* General error message */
#define READER_TERMINATOR '\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for BOA */
/* BITS									(7654.3210) */
#define READER_DEFAULT_FLAG 0x00 		/* (0000 0000)_2 = (000)_10 */

/* BIT 7: FUL = Full */ 
#define SET_FUL_BIT 0x80				/* (1000 0000)_2 = (0x80)_16 = (128) */
#define RESET_FUL_BIT 0x7F				/* (0111 1111)_2 = (0x7F)_16 = (127) */
#define CHECK_FUL_BIT SET_FUL_BIT		/* Refer to slides for explanation */

/* BIT 6: EMP: Empty */
#define SET_EMP_BIT 0x40				/* (0100 0000)_2 = (0x40)_16 = (64) */
#define RESET_EMP_BIT 0xBF				/* (1011 1111)_2 = (0xBF)_16 = (191) */
#define CHECK_EMP_BIT SET_EMP_BIT		/* Refer to slides for explanation */

/* BIT 5: REL = Realocation */
#define SET_REL_BIT 0x20				/* (0010 0000)_2 = (0x20)_16 = (32) */
#define RESET_REL_BIT 0xDF				/* (1101 1111)_2 = (0xDF)_16 = (223) */
#define CHECK_REL_BIT SET_REL_BIT		/* Refer to slides for explanation */

/* BIT 4: END = EndOfBuffer */
#define SET_END_BIT 0x10				/* (0001 0000)_2 = (0x10)_16 = (16) */
#define RESET_END_BIT 0xEF				/* (1110 1111)_2 = (0xEF)_16 = (239) */
#define CHECK_END_BIT SET_END_BIT		/* Refer to slides for explanation */

#define NCHAR				128			/* Chars from 0 to 127 */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	entero mark;			/* the offset to the mark position (in chars) */
	entero read;			/* the offset to the get a char position (in chars) */
	entero wrte;			/* the offset to the add chars (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	char*			content;			/* pointer to the beginning of character array (character buffer) */
	entero			size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	entero			increment;			/* character array increment factor */
	entero			mode;				/* operational mode indicator */
	byte			flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;			/* Offset / position field */
	entero			histogram[NCHAR];	/* Statistics of chars */
	entero			numReaderErrors;	/* Number of errors from Reader */
} BufferReader, * ReaderPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
ReaderPointer	readerCreate		(entero, entero, entero);
ReaderPointer	readerAddChar		(ReaderPointer const, char);
boolean			readerClear		    (ReaderPointer const);
boolean			readerFree		    (ReaderPointer const);
boolean			readerIsFull		(ReaderPointer const);
boolean			readerIsEmpty		(ReaderPointer const);
boolean			readerSetMark		(ReaderPointer const, entero);
entero			readerPrint		    (ReaderPointer const);
entero			readerLoad			(ReaderPointer const, FILE* const);
boolean			readerRecover		(ReaderPointer const);
boolean			readerRetract		(ReaderPointer const);
boolean			readerRestore		(ReaderPointer const);
/* Getters */
char			readerGetChar		(ReaderPointer const);
char*			readerGetContent	(ReaderPointer const, entero);
entero			readerGetPosRead	(ReaderPointer const);
entero			readerGetPosWrte	(ReaderPointer const);
entero			readerGetPosMark	(ReaderPointer const);
entero			readerGetSize		(ReaderPointer const);
entero			readerGetInc		(ReaderPointer const);
entero			readerGetMode		(ReaderPointer const);
byte			readerGetFlags		(ReaderPointer const);
entero			readerShowStat		(ReaderPointer const);
entero			readerNumErrors		(ReaderPointer const);

#endif
