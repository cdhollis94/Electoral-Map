#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "MapProgram.h"

int main() {

	MapProgram program;
	program.initGrid(program.grid, program.pixel_size);

	//PROGRAM LOOPS
	while (program.running()) {
		program.update();
		program.render();
	}
	return 0;
}