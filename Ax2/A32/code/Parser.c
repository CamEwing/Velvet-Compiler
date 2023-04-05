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
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013]
* Assignment: A32.
* Date: Sep 01 2022
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef PARSER_H_
#include "Parser.h"
#endif

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
 /* TO_DO: This is the function to start the parser - check your program definition */

void startParser() {
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
 /* TO_DO: This is the main code for match - check your definition */
void matchToken(entero tokenCode, entero tokenAttribute) {
	entero matchFlag = 1;
	
	switch (lookahead.code) {
	case KEY_T:
		if (lookahead.attribute.codeType != tokenAttribute)
			matchFlag = 0;
	default:
		if (lookahead.code != tokenCode)
			matchFlag = 0;
	}
	if (matchFlag && lookahead.code == SEOF_T) {
		return;
	}
	if (matchFlag) {
		lookahead = tokenizer();
		while (lookahead.code == COM_T) {
			lookahead = tokenizer();
		}
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else {
		syncErrorHandler(tokenCode);
	}


}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
 /* TO_DO: This is the function to handler error - adjust basically datatypes */
void syncErrorHandler(entero syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
 /* TO_DO: This is the function to error printing - adjust basically datatypes */
void printError() {
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case RTE_T:
		//numScannerErrors++;
		printf("RTE_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case ERR_T:
		numScannerErrors++;
		printf("*ERROR*\t\t%s\n", t.attribute.errLexeme);
		/* Update numScannerErrors */
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case MNID_T:
		printf("MNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case CHN_T:
		printf("CHN_T\t\t%d\t ", (entero)t.attribute.codeType);
		printf("%s\n", readerGetContent(stringLiteralTable, (entero)t.attribute.codeType));
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case LBR_T:
		printf("LBR_T\n");
		break;
	case RBR_T:
		printf("RBR_T\n");
		break;
	case KEY_T:
		printf("KEY_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case COM_T:
		printf("COM_T\n");
		break;
	case COMA_T:
		printf("COMA_T\n");
		break;
	case EOS_T:
		printf("NA\n");
		break;
	case ENID_T:
		printf("ENID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case CNID_T:
		printf("CNID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case ENL_T:
		printf("ENL_T\t\t%d\t\n", (entero)t.attribute.codeType);
		break;
	case DECI_T:
		printf("DECI_T\t\t%f\t\n", (decimal)t.attribute.decimalValue);
		break;

	case ART_OP_T:
		switch (t.attribute.arithmeticOperator) {
		case 0:
			printf("OP_ADD\n");
			break;
		case 1:
			printf("OP_SUB\n");
			break;
		case 2:
			printf("OP_MUL\n");
			break;
		case 3:
			printf("OP_DIV\n");
			break;
		}
		break;

	case EQ_T:
		printf("EQ_T\n");
		break;

	case REL_OP_T:
		switch (t.attribute.relationalOperator) {
		case 0:
			printf("OP_EQ\n");
			break;
		case 1:
			printf("OP_GT\n");
			break;
		case 2:
			printf("OP_LT\n");
			break;
		}
		break;
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> main& { <opt_statements> }
 * FIRST(<program>)= {MNID_T (main&)}
 ************************************************************
 */
void program() {
	while (lookahead.code == COM_T) {
		lookahead = tokenizer();
	}
	switch (lookahead.code) {

	case MNID_T:
		if (strncmp(lookahead.attribute.idLexeme, LANG_MAIN, 5) == 0) {
			matchToken(MNID_T, NO_ATTR);
			matchToken(LPR_T, NO_ATTR);
			matchToken(RPR_T, NO_ATTR);
			matchToken(LBR_T, NO_ATTR);
			
			decision_maker();

			matchToken(RBR_T, NO_ATTR);
			break;
		}
		else {
			printf("you shouldn't be here");
			printError();
		}
	case SEOF_T:
		; // Empty
		break;
	default:
		printError();
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

void decision_maker() {
	while (lookahead.code != RBR_T) {
		if (lookahead.attribute.codeType == 0 || lookahead.attribute.codeType == 1 || lookahead.attribute.codeType == 2) {
			variable_declaration();
		}
		else {
			opt_code_statements();
		}

	}
}

/*
 ************************************************************
 * dataSession
 * BNF: <dataSession> -> data { <opt_varlist_declarations> }
 * FIRST(<program>)= {KEY_T (KW_data)}.
 ***********************************************************
 */
//void dataSession() {
//	matchToken(KEY_T, KW_data);
//	matchToken(LBR_T, NO_ATTR);
//	optVarListDeclarations();
//	matchToken(RBR_T, NO_ATTR);
//	printf("%s%s\n", STR_LANGNAME, ": Data Session parsed");
//}

/*
 ************************************************************
 * Optional Var List Declarations
 * BNF: <opt_varlist_declarations> -> <varlist_declarations> | e
 * FIRST(<opt_varlist_declarations>) = { e, KEY_T (KW_int), KEY_T (KW_real), KEY_T (KW_string)}.
 ***********************************************************
 */
void variable_declaration() {
	switch (lookahead.code) {
	case(KEY_T):
		switch (lookahead.attribute.codeType) {
		case(KW_ent):
			matchToken(KEY_T, KW_ent);
			matchToken(ENID_T, NO_ATTR);
			switch (lookahead.code) {
			case(EQ_T):
				matchToken(EQ_T, NO_ATTR);
				arithmetic_expression();
				matchToken(EOS_T, NO_ATTR);
				printf("%s: Initializing Entero\n", STR_LANGNAME);
				break;
			case(EOS_T):
				matchToken(EOS_T, NO_ATTR);
				printf("%s: Declaring Entero\n", STR_LANGNAME);
				break;
			}
			break;
		case(KW_decimal):
			matchToken(KEY_T, KW_decimal);
			matchToken(ENID_T, NO_ATTR);
			switch (lookahead.code) {
			case(EQ_T):
				matchToken(EQ_T, NO_ATTR);
				arithmetic_expression();
				matchToken(EOS_T, NO_ATTR);
				printf("%s: Initializing Decimal\n", STR_LANGNAME);
				break;
			case(EOS_T):
				matchToken(EOS_T, NO_ATTR);
				printf("%s: Declaring Decimal\n", STR_LANGNAME);
				break;
			}
			break;
		case(KW_chain):
			matchToken(KEY_T, KW_chain);
			matchToken(CNID_T, NO_ATTR);
			switch (lookahead.code) {
			case(EQ_T):
				matchToken(EQ_T, NO_ATTR);
				matchToken(CHN_T, NO_ATTR);
				matchToken(EOS_T, NO_ATTR);
				printf("%s: Initializing Chain\n", STR_LANGNAME);
				break;
			case(EOS_T):
				matchToken(EOS_T, NO_ATTR);
				printf("%s: Declaring Chain\n", STR_LANGNAME);
				break;
			}
			break;
		default:
			;
		}
	default:
		; // Empty
	}
}

void arithmetic_expression() {
	if (lookahead.code == ENL_T) {
		matchToken(ENL_T, NO_ATTR);
		arithmetic_expressions();
	}
	if (lookahead.code == DECI_T) {
		matchToken(DECI_T, NO_ATTR);
		arithmetic_expressions();
	}
	if (lookahead.attribute.arithmeticOperator) {
		unary_arithmetic_expression();
	}
}

void unary_arithmetic_expression() {
	switch (lookahead.attribute.arithmeticOperator) {
	case OP_ADD:
		matchToken(ART_OP_T, NO_ATTR);
		primary_arithmetic_expression();
		break;
	case OP_SUB:
		matchToken(ART_OP_T, NO_ATTR);
		primary_arithmetic_expression();
		break;
	}
}

void arithmetic_expressions() {
	switch (lookahead.attribute.arithmeticOperator) {
	case OP_ADD:
		matchToken(ART_OP_T, NO_ATTR);
		primary_arithmetic_expression();
		printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression parsed");
		break;
	case OP_SUB:
		matchToken(ART_OP_T, NO_ATTR);
		primary_arithmetic_expression();
		printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression parsed");
		break;
	case OP_DIV:
		matchToken(ART_OP_T, NO_ATTR);
		primary_arithmetic_expression();
		printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression parsed");
		break;
	case OP_MUL:
		matchToken(ART_OP_T, NO_ATTR);
		primary_arithmetic_expression();
		printf("%s%s\n", STR_LANGNAME, ": Arithmetic expression parsed");
		break;
	default:
		;
	}
}

void primary_arithmetic_expression() {
	switch (lookahead.code) {
	case ENID_T: //Numeric Variable
		matchToken(ENID_T, NO_ATTR);
		break;
	case DECI_T: //Decimal number
		matchToken(DECI_T, NO_ATTR);
		break;
	case ENL_T: //Entero number
		matchToken(ENL_T, NO_ATTR);
		break;
	default:
		arithmetic_expression();
		break;
	}
}

/*
 ************************************************************
 * codeSession statement
 * BNF: <codeSession> -> code { <opt_statements> }
 * FIRST(<codeSession>)= {KEY_T (KW_code)}.
 ***********************************************************
 */
//void codeSession() {
//	matchToken(KEY_T, KW_code);
//	matchToken(LBR_T, NO_ATTR);
//	optionalStatements();
//	matchToken(RBR_T, NO_ATTR);
//	printf("%s%s\n", STR_LANGNAME, ": Code Session parsed");
//}

/* TO_DO: Continue the development (all non-terminal functions) */

/*
 ************************************************************
 * Optional statement
 * BNF: <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = { ϵ , IVID_T, FVID_T, SVID_T, KEY_T(KW_if),
 *				KEY_T(KW_while), MNID_T(print&), MNID_T(input&) }
 ***********************************************************
 */
void opt_code_statements() {
	switch (lookahead.code) {
	case MNID_T:
		if ((strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) ||
			(strncmp(lookahead.attribute.idLexeme, LANG_READ, 6) == 0)) {
			statements();
			break;
		}
	case KEY_T:
		switch (lookahead.attribute.codeType) {
		case(KW_if):
			selection_statement();
			break;
		case(KW_for):
			iteration_statement();
			break;
		case(KW_when):
			iteration_statement();
			break;
		}
	default:
		; // Empty
	}
	//printf("%s%s\n", STR_LANGNAME, ": Optional statements parsed");
}

/*
 ************************************************************
 * Statements
 * BNF: <statements> -> <statement><statementsPrime>
 * FIRST(<statements>) = { IVID_T, FVID_T, SVID_T, KEY_T(KW_if),
 *		KEY_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
void statements() {
	statement();
	//statementsPrime();
	//printf("%s%s\n", STR_LANGNAME, ": Statements parsed");
}

/*
 ************************************************************
 * Statements Prime
 * BNF: <statementsPrime> -> <statement><statementsPrime> | ϵ
 * FIRST(<statementsPrime>) = { ϵ , IVID_T, FVID_T, SVID_T,
 *		KEY_T(KW_if), KEY_T(KW_while), MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
void statementsPrime() {
	switch (lookahead.code) {
	case MNID_T:
		if ((strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) ||
			(strncmp(lookahead.attribute.idLexeme, LANG_READ, 6) == 0)) {
			statements();
			break;
		}
	default:
		; //empty string
	}
}

/*
 ************************************************************
 * Single statement
 * BNF: <statement> -> <assignment statement> | <selection statement> |
 *	<iteration statement> | <input statement> | <output statement>
 * FIRST(<statement>) = { IVID_T, FVID_T, SVID_T, KEY_T(KW_if), KEY_T(KW_while),
 *			MNID_T(input&), MNID_T(print&) }
 ***********************************************************
 */
void statement() {
	switch (lookahead.code) {
	case KEY_T:
		switch (lookahead.attribute.codeType) {
		default:
			printError();
		}
		break;
	case MNID_T:
		if ((strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) || (strncmp(lookahead.attribute.idLexeme, LANG_READ, 6) == 0)) {
			if (strncmp(lookahead.attribute.idLexeme, LANG_WRTE, 6) == 0) {
				printf("%s%s\n", STR_LANGNAME, ": Print statement parsed");
			}
			else {
				printf("%s%s\n", STR_LANGNAME, ": Input statement parsed");
			}
			matchToken(MNID_T, NO_ATTR);
			matchToken(LPR_T, NO_ATTR);
			switch (lookahead.code) {
			case ENID_T: //Numeric Variable
				matchToken(ENID_T, NO_ATTR);
				break;
			case DECI_T: //Decimal number
				matchToken(DECI_T, NO_ATTR);
				break;
			case ENL_T: //Entero number
				matchToken(ENL_T, NO_ATTR);
				break;
			case CNID_T: //chain variable
				matchToken(CNID_T, NO_ATTR);
				break;
			case CHN_T:	//chain value
				matchToken(CHN_T, NO_ATTR);
				break;
			}
			matchToken(RPR_T, NO_ATTR);
			matchToken(EOS_T, NO_ATTR);
		}
	
		opt_code_statements();
		break;
	default:
		printError();
	}
//	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

void selection_statement() {
	matchToken(KEY_T, KW_if);
	matchToken(LPR_T, NO_ATTR);
	conditional_expression();
	matchToken(RPR_T, NO_ATTR);
	matchToken(LBR_T, NO_ATTR);
	opt_code_statements();
	matchToken(RBR_T, NO_ATTR);
	while (lookahead.attribute.codeType != KW_else) {
		if (lookahead.attribute.codeType == KW_elseif) {
			matchToken(KEY_T, KW_elseif);
			matchToken(LPR_T, NO_ATTR);
			conditional_expression();
			matchToken(RPR_T, NO_ATTR);
			matchToken(LBR_T, NO_ATTR);
			decision_maker();
			matchToken(RBR_T, NO_ATTR);
		}
	}
	if (lookahead.attribute.codeType == KW_else) {
		matchToken(KEY_T, KW_else);
		matchToken(LBR_T, NO_ATTR);
		decision_maker();
		matchToken(RBR_T, NO_ATTR);
	}
}

void iteration_statement() {
	switch (lookahead.attribute.codeType) {
	case(KW_for):
		matchToken(KEY_T, KW_for);
		matchToken(LPR_T, NO_ATTR);
		relational_expression();
		matchToken(RPR_T, NO_ATTR);
		matchToken(LBR_T, NO_ATTR);
		decision_maker();
		matchToken(RBR_T, NO_ATTR);
		break;
	case(KW_when):
		matchToken(KEY_T, KW_when);
		matchToken(LPR_T, NO_ATTR);
		conditional_expression();
		matchToken(RPR_T, NO_ATTR);
		matchToken(LBR_T, NO_ATTR);
		decision_maker();
		matchToken(RBR_T, NO_ATTR);
		break;
	}
	

}

void conditional_expression() {
	relational_expression();
	switch (lookahead.attribute.codeType) {
	case(KW_OR):
		logical_OR_expression();
		
		break;
	case(KW_AND):
		logical_AND_expression();
		break;
	}
	
}

void logical_OR_expression() {
	matchToken(KEY_T, KW_OR);
	relational_expression();
	printf("%s%s\n", STR_LANGNAME, ": Logical OR expression parsed");
}

void logical_AND_expression() {
	matchToken(KEY_T, KW_AND);
	relational_expression();
	printf("%s%s\n", STR_LANGNAME, ": Logical AND expression parsed");
}

void relational_expression() {
	switch (lookahead.code) {
	case ENID_T: //Numeric Variable
		matchToken(ENID_T, NO_ATTR);
		relational_a_expression();
		break;
	case DECI_T: //Decimal number
		matchToken(DECI_T, NO_ATTR);
		relational_a_expression();
		break;
	case ENL_T: //Entero number
		matchToken(ENL_T, NO_ATTR);
		relational_a_expression();
		break;
	case CNID_T: //chain variable
		matchToken(CNID_T, NO_ATTR);
		relational_s_expression();
		break;
	case CHN_T:	//chain value
		matchToken(CHN_T, NO_ATTR);
		relational_s_expression();
		break;
	}


}

void relational_a_expression() {
	switch (lookahead.attribute.relationalOperator) {
	case OP_EQ:
		matchToken(REL_OP_T, NO_ATTR);
		primary_a_relational_expressions();
		printf("%s%s\n", STR_LANGNAME, ": Relational arithmetic expression parsed");
		break;
	case OP_GT: 
		matchToken(REL_OP_T, NO_ATTR);
		primary_a_relational_expressions();
		printf("%s%s\n", STR_LANGNAME, ": Relational arithmetic expression parsed");
		break;
	case OP_LT:
		matchToken(REL_OP_T, NO_ATTR);
		primary_a_relational_expressions();
		printf("%s%s\n", STR_LANGNAME, ": Relational arithmetic expression parsed");
		break;
	}
}

void relational_s_expression() {
	switch (lookahead.attribute.relationalOperator) {
	case OP_EQ:
		matchToken(REL_OP_T, NO_ATTR);
		primary_s_relational_expressions();
		printf("%s%s\n", STR_LANGNAME, ": Relational chain expression parsed");
		break;
	case OP_GT:
		matchToken(REL_OP_T, NO_ATTR);
		primary_s_relational_expressions();
		printf("%s%s\n", STR_LANGNAME, ": Relational chain expression parsed");
		break;
	case OP_LT:
		matchToken(REL_OP_T, NO_ATTR);
		primary_s_relational_expressions();
		printf("%s%s\n", STR_LANGNAME, ": Relational chain expression parsed");
		break;
	}
}


void primary_a_relational_expressions() {
	switch (lookahead.code) {
	case ENID_T: //Numeric Variable
		matchToken(ENID_T, NO_ATTR);
		break;
	case DECI_T: //Decimal number
		matchToken(DECI_T, NO_ATTR);
		break;
	case ENL_T: //Entero number
		matchToken(ENL_T, NO_ATTR);
		break;
	}
}

void primary_s_relational_expressions() {
	switch (lookahead.code) {
	case CNID_T: //chain variable
		matchToken(CNID_T, NO_ATTR);
		break;
	case CHN_T:	//chain value
		matchToken(CHN_T, NO_ATTR);
		break;
	}
}

/*
 ************************************************************
 * Output Statement
 * BNF: <output statement> -> print& (<output statementPrime>);
 * FIRST(<output statement>) = { MNID_T(print&) }
 ***********************************************************
 */
void output_statement() {
	matchToken(MNID_T, NO_ATTR);
	matchToken(LPR_T, NO_ATTR);
	opt_variable_list();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Output statement parsed");
}

/*
 ************************************************************
 * Output Variable List
 * BNF: <opt_variable list> -> <variable list> | ϵ
 * FIRST(<opt_variable_list>) = { IVID_T, FVID_T, SVID_T, ϵ }
 ***********************************************************
 */
void opt_variable_list() {
	switch (lookahead.code) {
	case CHN_T:
		matchToken(CHN_T, NO_ATTR);
		break;
	default:
		;
	}
	printf("%s%s\n", STR_LANGNAME, ": Output variable list parsed");
}