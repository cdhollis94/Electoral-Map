#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include "MapProgram.h"
#include <array>

int main() {

	MapProgram program;
	program.initGrid(program.grid, program.pixel_size);
	program.initOutline(program.half_border_thickness);
	sf::Font font;
	if (!font.loadFromFile("cambria.ttc")) {
		std::cout << "FONT ERROR";
	}
	sf::Text state_name;
	state_name.setFont(font);
	state_name.setCharacterSize(48);
	state_name.setFillColor(sf::Color::Black);
	state_name.setPosition(program.grid[0][program.rows - 1].pixel_shape.getPosition().x + program.grid[0][program.rows].pixel_shape.getSize().x * 2, program.window->getSize().y/8);

	program.repBar.setFillColor(program.rep);
	program.repBar.setPosition(program.window->getSize().x/2, program.window->getSize().y/8 * 7);
	program.demBar.setFillColor(program.dem);
	program.demBar.setPosition(program.window->getSize().x / 2 - program.demBar.getSize().x, program.window->getSize().y / 8 * 7);
	program.winBarLeft.setPosition(program.window->getSize().x/2 - 270, program.window->getSize().y / 8 * 7);
	program.winBarLeft.setFillColor(sf::Color::White);
	program.winBarRight.setPosition(program.window->getSize().x / 2 + 270, program.window->getSize().y / 8 * 7);
	program.winBarRight.setFillColor(sf::Color::White);

	sf::Text dem_votes;
	dem_votes.setFont(font);
	dem_votes.setCharacterSize(18);
	dem_votes.setFillColor(sf::Color::White);
	dem_votes.setPosition(program.window->getSize().x / 2 - program.demBar.getSize().x, program.window->getSize().y / 8 * 7 - dem_votes.getGlobalBounds().height - 2);

	sf::Text rep_votes;
	rep_votes.setFont(font);
	rep_votes.setCharacterSize(18);
	rep_votes.setFillColor(sf::Color::White);
	rep_votes.setPosition(program.window->getSize().x / 2 + program.repBar.getSize().x, program.window->getSize().y / 8 * 7 - rep_votes.getGlobalBounds().height - 2);

	sf::Text state_votes;
	state_votes.setFont(font);
	state_votes.setCharacterSize(24);
	state_votes.setFillColor(sf::Color::Black);
	state_votes.setPosition(program.grid[0][program.rows - 1].pixel_shape.getPosition().x + program.grid[0][program.rows].pixel_shape.getSize().x * 2 + program.grid[0][0].pixel_shape.getSize().x / 2, program.window->getSize().y / 64 * 15);

	sf::Text winner;
	winner.setFont(font);
	winner.setCharacterSize(64);
	winner.setFillColor(sf::Color::Black);
	winner.setPosition(program.grid[0][program.rows - 1].pixel_shape.getPosition().x + program.grid[0][program.rows].pixel_shape.getSize().x, program.window->getSize().y / 8 * 5);

	//PROGRAM LOOPS
	while (program.running()) {
		program.update(state_name, state_votes, dem_votes, rep_votes, winner);
		program.render(state_name, state_votes, dem_votes, rep_votes, winner);
	}
	return 0;
}