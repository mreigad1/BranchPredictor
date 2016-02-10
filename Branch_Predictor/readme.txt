Matthew Reigada
11/30/2015

Branch Prediction Unit Simulator

This program receives input from a formatted text file and simulates
an externally specified CPU’s branch prediction unit under the given
Taken/Not-Taken instruction resolution sequence.

Notes:	No known bugs at current.  All programs receive input from cin and provide output to cout.
	Therefore input & output files are required with <> at program invocation.

Getting Started:

	Branch Prediction:
		(1)	Execute "make" to compile "main.cpp" into "driver"

		(2)	Invoke "./driver < [INPUT_FILE_NAME] > [OUTPUT_FILE_NAME]"
				Note: input.txt contains full example file with 10,000 lines

		(3)	Program will generate output file for comparison.
