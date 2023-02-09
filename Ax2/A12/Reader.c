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
* Purpose: This file is the main code for Reader (.c)
************************************************************
*/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

/*
***********************************************************
* Function name: readerCreate
* Purpose: Creates the buffer reader according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: S22
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to reader)
* Algorithm: Allocation of memory according to inicial (default) values.
* TODO ......................................................
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/
ReaderPointer readerCreate(entero size, entero increment, entero mode) {
	
	ReaderPointer readerPointer;

	/* Defensive programming & adjusting the values according to parameters */
	if (size == 0) {
		size = READER_DEFAULT_SIZE;
		increment = READER_DEFAULT_INCREMENT;
	}

	if (increment == 0) {
		mode = MODE_FIXED;
	}

	if ((mode != MODE_FIXED) && (mode != MODE_ADDIT) && (mode != MODE_MULTI)) {
		return NULL;
	}

	readerPointer = (ReaderPointer)calloc(1, sizeof(BufferReader));

	/* Defensive programming */
	if (!readerPointer) {
		return NULL;
	}

	readerPointer->content = (char*)malloc(size);

	/* Defensive programming */
	if (!readerPointer->content) {
		return NULL;
	}

	/* Initialize the histogram */
	for (int i = 0; i < NCHAR; i++) {
		readerPointer->histogram[i] = 0;
	}

	readerPointer->size = size;
	readerPointer->increment = increment;
	readerPointer->mode = mode;

	/* Initialize flags */
	readerPointer->flags = READER_DEFAULT_FLAG;

	/* The created flag must be signalized as EMP */
	readerPointer->flags |= SET_EMP_BIT;

	return readerPointer;
}

/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
ReaderPointer readerAddChar(ReaderPointer const readerPointer, char ch) {
	
	char* tempReader = NULL;
	entero newSize = 0;

	/* Defensive programming: check buffer and valid char (increment numReaderErrors) */
	if (!readerPointer) {
		return NULL;
	}

	if (ch > NCHAR || ch < 0) {
		readerPointer->numReaderErrors++;
		return readerPointer;
	}

	/* Reset REL */
	readerPointer->flags &= RESET_REL_BIT;

	/* Test the inclusion of chars */
	if (readerPointer->position.wrte * (entero)sizeof(char) < readerPointer->size) {
		/* This buffer is NOT full */
		readerPointer->flags &= RESET_FUL_BIT;
		readerPointer->flags &= RESET_EMP_BIT;
	}else {
		/* Re-set Full flag */
		readerPointer->flags |= SET_FUL_BIT;
		switch (readerPointer->mode) {
		
		case MODE_FIXED:
			return NULL;

		case MODE_ADDIT:
			/* Adjust new size */
			newSize = readerPointer->size + readerPointer->increment;
			/* Defensive programming */
			if (!newSize) {						//I THINK THIS IS WRONG SHOULD CHECK NEWSIZE AGAINST READER_MAX_SIZE AND 0
				readerPointer->numReaderErrors++;
				return NULL;
			}
			break;

		case MODE_MULTI:
			/*  Adjust new size */
			newSize = readerPointer->size * readerPointer->increment;
			/* Defensive programming */
			if (!newSize) {										//FUNKY HERE TOO
				readerPointer->numReaderErrors++;
				return NULL;
			}
			break;

		default:
			return NULL;
		}

		/* New Reader Allocation & Check Realocation */
		tempReader = (char*)realloc(readerPointer->content, sizeof(char)*newSize);
		
		if (!tempReader) {
			return NULL;
		}

		readerPointer->content = tempReader;
		readerPointer->size = newSize;
	}

	/* Add the char */
	readerPointer->content[readerPointer->position.wrte++] = ch;

	/* Updates histogram */
	readerPointer->histogram[(int)ch]++;

	return readerPointer;
}

/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerClear(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		return FALSE;
	}

	/* Adjust flags original */
	readerPointer->flags &= READER_DEFAULT_FLAG;	//Resetting flags to 0x00
	readerPointer->flags |= SET_EMP_BIT; //Setting flags to empty 0x40
	
	return TRUE;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerFree(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		return FALSE;
	}

	/* Free pointers */
	free(readerPointer->content);
	free(readerPointer);

	return TRUE;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerIsFull(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		return FALSE;
	}

	/* Check flag if buffer is FUL */	
	if ((readerPointer->flags & CHECK_FUL_BIT) == CHECK_FUL_BIT) {
		return TRUE;
	}

	return FALSE;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerIsEmpty(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		return FALSE;
	}

	/* Check flag if buffer is EMP */
	if ((readerPointer->flags & CHECK_EMP_BIT) == CHECK_EMP_BIT) {
		return TRUE;
	}

	return FALSE;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerSetMark(ReaderPointer const readerPointer, entero mark) {
	/* Defensive programming */
	if (!readerPointer || mark < 0 || mark > readerPointer->position.wrte) {
		return FALSE;
	}

	/* Adjust mark */
	readerPointer->position.mark = mark;

	return TRUE;
}

/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
entero readerPrint(ReaderPointer const readerPointer) {
	
	entero cont = 0;
	char c;

	/* Defensive programming (including invalid chars) */
	if (!readerPointer) {
		return READER_ERROR;
	}

	c = readerGetChar(readerPointer);

	if (c > NCHAR || c < 0) {
		readerPointer->numReaderErrors++;
		return READER_ERROR;
	}

	/* Check flag if buffer EOB has achieved */
	while (cont < readerPointer->position.wrte) {
		cont++;
		printf("%c", c);
		c = readerGetChar(readerPointer);
		if ((readerPointer->flags & CHECK_END_BIT) == CHECK_END_BIT) {
			return cont;
		}
	}

	return cont;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
entero readerLoad(ReaderPointer const readerPointer, FILE* const fileDescriptor) {
	
	entero size = 0;
	char c;

	/* Defensive programming */	
	if (!readerPointer || !fileDescriptor) {
		return READER_ERROR;
	}
	
	c = (char)fgetc(fileDescriptor);

	while (!feof(fileDescriptor)) {
		//Paulo commented this out during lecture:
		/*if (!readerAddChar(readerPointer, c)) {	
			ungetc(c, fileDescriptor);
			return READER_ERROR;
		}*/
		//And added this:
		readerAddChar(readerPointer, c);
		c = (char)fgetc(fileDescriptor);
		size++;
	}

	/* Defensive programming */
	if (size > READER_MAX_SIZE) {
		return READER_ERROR;
	}

	return size;
}

/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerRecover(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		return FALSE;
	}

	/* Recover positions */
	readerPointer->position.read = 0;
	readerPointer->position.mark = 0;

	return TRUE;
}

/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerRetract(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		return FALSE;
	}

	/* Retract (return 1 pos read) */
	if (readerPointer->position.read > 0) {
		readerPointer->position.read -= 1;
	}

	return TRUE;
}

/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerRestore(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		return FALSE;
	}

	/* Restore positions (read/mark) */
	readerPointer->position.read = readerPointer->position.mark;
	
	return TRUE;
}

/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
char readerGetChar(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		return READER_TERMINATOR;
	}

	/* Check condition to read/wrte */
	if (readerPointer->position.read == readerPointer->position.wrte) {
		/* Set EOB flag */
		readerPointer->flags |= SET_END_BIT;
		return READER_TERMINATOR;
	}else {
		/* Reset EOB flag */
		readerPointer->flags &= RESET_END_BIT;
	}

	return readerPointer->content[readerPointer->position.read++];
}

/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
char* readerGetContent(ReaderPointer const readerPointer, entero pos) {
	/* Defensive programming */
	if(!readerPointer) {
		return NULL;
	}

	if (pos < 0 || pos > readerPointer->position.wrte) {
		return NULL;
	}

	/* Return content (string) */
	return readerPointer->content + pos;
}

/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
entero readerGetPosRead(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		return READER_ERROR;
	}

	/* Return read */
	return readerPointer->position.read;
}

/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
entero readerGetPosWrte(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		return READER_ERROR;
	}

	/* Return wrte */
	return readerPointer->position.wrte;
}

/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
entero readerGetPosMark(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		return READER_ERROR;
	}

	/* Return mark */
	return readerPointer->position.mark;
}

/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
entero readerGetSize(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		return READER_ERROR;
	}

	/* Return size (in bytes, not bits) */
	return readerPointer->size/8;
}

/*
***********************************************************
* Function name: readerGetInc
* Purpose: Returns the buffer increment.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The Buffer increment.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
entero readerGetInc(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		return READER_ERROR;
	}

	/* Return increment */
	return readerPointer->increment;
}

/*
***********************************************************
* Function name: readerGetMode
* Purpose: Returns the operational mode
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Operational mode.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
entero readerGetMode(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		return READER_ERROR;
	}

	/* Return mode */
	return readerPointer->mode;
}

/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
byte readerGetFlags(ReaderPointer const readerPointer) {
	/* Defensive programming */
	if (!readerPointer) {
		return READER_DEFAULT_FLAG;
	}

	/* Return flags */
	return readerPointer->flags;
}

/*
***********************************************************
* Function name: readerShowStat
* Purpose: Returns the number of chars used.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
entero readerShowStat(ReaderPointer const readerPointer) {
	
	int counter = 0;
	
	/* Defensive programming */
	if (!readerPointer) {
		return READER_ERROR;
	}

	/* Updates the histogram */
	for (int i = 0; i < NCHAR; i++) {
		if (readerPointer->histogram[i] > 0)
			counter++;
	}

	return counter;
}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
entero readerNumErrors(ReaderPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (!readerPointer) {
		return READER_ERROR;
	}

	/* Returns numReaderErrors */
	return readerPointer->numReaderErrors;
}
