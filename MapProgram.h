#pragma once

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


class MapProgram {
public:
	sf::ContextSettings settings;
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	MapProgram();
	virtual ~MapProgram();
	void initWindow();
	sf::Event event;

	static const int rows = 39;
	static const int columns = 25;
	sf::Vector2i mouse_pos;
	sf::RectangleShape grid[columns][rows];
	sf::Vector2f pixel_size = sf::Vector2f(15.0f, 15.0f);
	void initGrid(sf::RectangleShape grid[][rows], sf::Vector2f pixel_size);
	void initStates();
	const bool running() const;
	void pollEvents();
	void update();
	void render();
	void renderGrid();

	sf::Color blank = sf::Color(255, 255, 255);
	sf::Color tossup = sf::Color(169, 169, 169);
	sf::Color dem = sf::Color(0, 0, 255);
	sf::Color rep = sf::Color(255, 0, 0);


	class State {
	public:
		sf::RectangleShape state_pixels[45];
		int votes = 0;
		State(int votes);
	};

	//State alabama(9), alaska(3), arizona(11), arkansas(6), california(55), colorado(9), connecticut(7), delaware(3), florida(29), georgia(16), hawaii(4), idaho(4), illinois(20), indiana(11), iowa(6),
		//kansas(6), kentucky(8), louisiana(8), maine(4), maryland(10), massachussets(11), michigan(16), minnesota(10), mississippi(6), missouri(10), montana(3), nebraska(5), nevada(6), new_hampshire(4),
		//new_mexico(5), new_york(29), north_carolina(15), north_dakota(3), ohio(18), oklahoma(7), oregon(7), pennsylvania(20), rhode_island(4), south_carolina(9), south_dakota(3), tennessee(11),
		//texas(38), utah(6), vermont(3), virginia(13), washington(12), west_virginia(5), wisconsin(10), wyoming(3);

	void addToState(sf::RectangleShape pixel, int column, int row);
};
