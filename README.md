# Custom Interpreter

This was a fun university junior year C project that interprets and executes a custom language.
We were given a grammar and tasked to tokenize and parse code written in the provided text files.
The output should match each text files corresponding expected file.

Features of this program:
* Reads in one of the provided test files
* Parse the input file looking for tokens and build the statements
* Interpret and execute the custom code
* Output the result

##### To test
###### Linux
* Clone the project and move to its directory
* Run `make` to build the executable
* Run the program and redirect input from one of the test files
* e.g `./main.out < tests/switch_while.txt`
* Verify output

###### Windows
This project was written for Linux so running on Visual Studio may require an installation of Windows Subsystem for Linux (WSL)
* Clone the project
* Open the solution file in Visual Studio
* Configure a connection to your WSL installation
* Run the program
* You can change input parameters from `Debug > matrix_multiplication Properties > Debugging > Program Arguments`