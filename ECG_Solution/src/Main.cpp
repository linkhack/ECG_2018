/*
* Copyright 2018 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/


#include "Utils.h"
#include


/* --------------------------------------------- */
// Prototypes
/* --------------------------------------------- */

void keyCallback(GLFWindow* window, );


/* --------------------------------------------- */
// Global variables
/* --------------------------------------------- */



/* --------------------------------------------- */
// Main
/* --------------------------------------------- */

int main(int argc, char** argv)
{
	/* --------------------------------------------- */
	// Load settings.ini
	/* --------------------------------------------- */

	INIReader reader("assets/config.ini");
	
	int width = reader.GetInteger("window", "width", 800);
	int height = reader.GetInteger("window", "height", 800);
	int refreshRate = reader.GetInteger("window", "refresh_rate", 120);
	std::string windowTitle = reader.Get("window", "title", "ECG");


	/* --------------------------------------------- */
	// Create Window Context
	/* --------------------------------------------- */

	if (!glfwInit()) {
		EXIT_WITH_ERROR("Failed to init glfw");
	}

	/* --------------------------------------------- */
	// Init framework
	/* --------------------------------------------- */

	if (!initFramework()) {
		EXIT_WITH_ERROR("Failed to init framework")
	}



	/* --------------------------------------------- */
	// Initialize scene and render loop
	/* --------------------------------------------- */
	{

	}


	/* --------------------------------------------- */
	// Destroy framework
	/* --------------------------------------------- */

	destroyFramework();


	/* --------------------------------------------- */
	// Destroy context and exit
	/* --------------------------------------------- */


	return EXIT_SUCCESS;
}




