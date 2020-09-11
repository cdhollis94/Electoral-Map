#pragma once

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>


class MapProgram {
public:
	struct Pixel;
	class State;

	sf::ContextSettings settings;
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	MapProgram();
	virtual ~MapProgram();
	void initWindow();
	sf::Event event;
	void initOutline(int half_thickness);
	int half_border_thickness = 10;

	struct Outlines {
		sf::RectangleShape outline_left;
		sf::RectangleShape outline_up;
		sf::RectangleShape outline_right;
		sf::RectangleShape outline_down;
		sf::RectangleShape outline_left2;
		sf::RectangleShape outline_up2;
		sf::RectangleShape outline_right2;
		sf::RectangleShape outline_down2;
	};

	Outlines borders;
	sf::RectangleShape demBar = sf::RectangleShape(sf::Vector2f(0.f, 20.f));
	sf::RectangleShape repBar = sf::RectangleShape(sf::Vector2f(0.f, 20.f));
	sf::RectangleShape winBarLeft = sf::RectangleShape(sf::Vector2f(3.f, 20.f));
	sf::RectangleShape winBarRight = sf::RectangleShape(sf::Vector2f(3.f, 20.f));

	static const int rows = 39;
	static const int columns = 35;
	sf::Vector2i mouse_pos;
	sf::Vector2f click_pos;
	float dem_total = 0;
	float rep_total = 0;
	int undecided_total = 538;
	std::string int_to_string(int num);

	struct Pixel {
		sf::RectangleShape pixel_shape;
		State* parent_state;
	};

	Pixel grid[columns][rows];
	sf::Vector2f pixel_size = sf::Vector2f(15.0f, 15.0f);
	void initGrid(Pixel grid[][rows], sf::Vector2f pixel_size);
	const bool running() const;
	void pollEvents();
	void checkClickLeft();
	void checkClickRight();
	void hover_over_state(sf::Text &name, sf::Text &votes);
	void update(sf::Text &name, sf::Text &votes, sf::Text &dem_votes, sf::Text &rep_votes, sf::Text &winner);
	void render(sf::Text name, sf::Text votes, sf::Text dem_votes, sf::Text rep_votes, sf::Text winner);
	void renderGrid();

	sf::Color blank = sf::Color(255, 255, 255);
	sf::Color tossup = sf::Color(169, 169, 169);
	sf::Color dem = sf::Color(0, 0, 255);
	sf::Color rep = sf::Color(255, 0, 0);


	class State {
	public:
		Pixel* state_pixels[45];
		int pixel_count = 0;
		int votes = 0;
		State(int votes, std::string name);
		std::string result = "Tossup";
		std::string name = "";
	};

	State alabama = State(9, "Alabama");
	State alaska = State(3, "Alaska");
	State arizona = State(11, "Arizona");
	State arkansas = State(6, "Arkansas");
	State california = State(55, "California");
	State colorado = State(9, "Colorado");
	State connecticut = State(7, "Connecticut");
	State delaware = State(3, "Delaware");
	State florida = State(29, "Florida");
	State georgia = State(16, "Georgia");
	State hawaii = State(4, "Hawaii");
	State idaho = State(4, "Idaho");
	State illinois = State(20, "Illinois");
	State indiana = State(11, "Indiana");
	State iowa = State(6, "Iowa");
	State kansas = State(6, "Kansas");
	State kentucky = State(8, "Kentucky");
	State louisiana = State(8, "Louisiana");
	State maine = State(4, "Maine");
	State maryland = State(10, "Maryland");
	State massachussets = State(11, "Massachussetts");
	State michigan = State(16, "Michigan");
	State minnesota = State(10, "Minnesota");
	State mississippi = State(6, "Mississippi");
	State missouri = State(10, "Missouri");
	State montana = State(3, "Montana");
	State nebraska = State(5, "Nebraska");
	State nevada = State(6, "Nevada");
	State new_hampshire = State(4, "New Hampshire");
	State new_jersey = State(14, "New Jersey");
	State new_mexico = State(5, "New Mexico");
	State new_york = State(29, "New York");
	State north_carolina = State(15, "North Carolina");
	State north_dakota = State(3, "North Dakota");
	State ohio = State(18, "Ohio");
	State oklahoma = State(7, "Oklahoma");
	State oregon = State(7, "Oregon");
	State pennsylvania = State(20, "Pennsylvania");
	State rhode_island = State(4, "Rhode Island");
	State south_carolina = State(9, "South Carolina");
	State south_dakota = State(3, "South Dakota");
	State tennessee = State(11, "Tennessee");
	State texas = State(38, "Texas");
	State utah = State(6, "Utah");
	State vermont = State(3, "Vermont");
	State virginia = State(13, "Virginia");
	State washington = State(12, "Washington");
	State west_virginia = State(5, "West Virginia");
	State wisconsin = State(10, "Wisconsin");
	State wyoming = State(3, "Wyoming");

	void addToState(Pixel &pixel, int column, int row);
};
