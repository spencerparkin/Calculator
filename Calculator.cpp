// Calculator.cpp

/*
 * Copyright (C) 2013 Spencer T. Parkin
 *
 * This software has been released under the MIT License.
 * See the "License.txt" file in the project root directory
 * for more information about this license.
 *
 */

#include <stdio.h>
#include "Calculator/CalcLib.h"

using namespace CalcLib;

//=========================================================================================
void GetLineOfInput( char* lineInput, int lineInputSize )
{
	// I should have designed CalcLib from the beginning to use std::string.
	fgets( lineInput, lineInputSize - 1, stdin );
	lineInput[ lineInputSize - 1 ] = '\0';
}

//=========================================================================================
void GetUserInput( Calculator& calculator, char* userInput, int userInputSize )
{
	bool isPartial = true;
	int lineCount = 0;
	userInput[0] = '\0';

	// Get lines of input from the user until a complete expression is given.
	// The detection of complete expressions is based entirely on parenthesis,
	// so it's not smart enough to detect certain things like "1 +" as a partial statement.
	while( isPartial )
	{
		// Present the prompt.  Indicate the continuing line of input number.
		printf( "%02d:Calculator: ", lineCount++ );
		fflush( stdout );

		// Get a line of input from the user.
		char lineInput[512];
		GetLineOfInput( lineInput, sizeof( lineInput ) );

		// Build up the full input.
		strcat_s( userInput, userInputSize, lineInput );

		// Detect partial statements.  If we somehow choke on the input, start over.
		if( !calculator.IsPartialMathExpression( userInput, isPartial ) )
		{
			printf( "Calculator choked on your input.  Starting over!\n\n" );
			lineCount = 0;
			userInput[0] = '\0';
		}
	}
}

//=========================================================================================
int main( int argc, char** argv )
{
	// Present greeting.
	printf( "Calculator -- (c) 2013 by Spencer T. Parkin\n\n" );
	printf( "Enter \"quit\" or \"exit\" to quit the program.\n\n" );

	// Create a new calculator in the desired environment.
	const char* envString = "geoalg";	// Default to the "geoalg" environment.
	if( argc > 1 )
		envString = argv[1];
	Calculator calculator( envString );

	// Provide a buffer for the environment to print messages.
	char envPrintBuffer[2048];
	calculator.GetEnvironment()->PrintBuffer( envPrintBuffer, sizeof( envPrintBuffer ) );

	// Calculate stuff until the user is sick of it.
	while( true )
	{
		// Prompt the user for input until we get a complete expression.
		char userInput[1024];
		GetUserInput( calculator, userInput, sizeof( userInput ) );

		// Are they sick of it yet?
		if( 0 == strcmp( userInput, "exit\n" ) || 0 == strcmp( userInput, "quit\n" ) )
			break;

		// Try to do the given calculation.
		char userOutput[2048];
		envPrintBuffer[0] = '\0';
		bool calculated = calculator.Calculate( userInput, userOutput, sizeof( userOutput ) );

		// If something went wrong, tell'em what.  Otherwise, give the answer.
		if( !calculated )
			calculator.PrintErrorReport();
		else
			printf( "\n%s\n\n", userOutput );

		// In all cases, success or failure, we dump the print buffer.
		printf( calculator.GetEnvironment()->PrintBuffer() );
		fflush( stdout );
	}

	// Return success exit code.
	return 0;
}

// Calculator.cpp