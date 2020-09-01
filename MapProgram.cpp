#include "MapProgram.h"


MapProgram::MapProgram()
{
	this->initWindow();
}

MapProgram::~MapProgram()
{
	delete this->window;
}

void MapProgram::initWindow()
{
	this->videoMode.height = 600;
	this->videoMode.width = 800;
	this->window = new sf::RenderWindow(this->videoMode, "Electoral Map", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

void MapProgram::initGrid(sf::RectangleShape grid[][rows], sf::Vector2f pixel_size)
{
	//sf::RectangleShape grid[25][39];								//CHANGE THE CONSTANTS
	for (int i = 0; i < columns; i++) {
		for (int j = 0; j < rows; j++) {
			grid[i][j].setPosition(j * pixel_size.x, i * pixel_size.y);
			grid[i][j].setSize(pixel_size);
			//grid[i][j].setFillColor(tossup);
			this->window->draw(grid[i][j]);
			this->addToState(grid[i][j], i, j);
		}
	}
}

void MapProgram::initStates()
{
}

const bool MapProgram::running() const
{
	return this->window->isOpen();
}

void MapProgram::pollEvents()
{
	while (this->window->pollEvent(this->event)) {
		if (this->event.type == sf::Event::Closed) {
			this->window->close();
			break;
		}
	}
	//INPUT CHECKS
	/*if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i click_pos = sf::Mouse::getPosition();
	}*/
}

void MapProgram::update()
{
	this->pollEvents();
	this->mouse_pos = sf::Mouse::getPosition();
	std::cout << mouse_pos.x;
	std::cout << " ";
	std::cout << mouse_pos.y;
	std::cout << "\n";
}

void MapProgram::render()
{
	this->window->clear();
	this->renderGrid();
	this->window->display();
}

void MapProgram::renderGrid()
{
	for (int i = 0; i < this->columns; i++) {
		for (int k = 0; k < this->rows; k++) {
			this->window->draw(this->grid[i][k]);
		}
	}
}

MapProgram::State::State(int votes)
{
	this->votes = votes;
}

void MapProgram::addToState(sf::RectangleShape pixel, int column, int row) {
	if (column == 0) {
		if ((row >= 2) && (row <= 6)) {
			pixel.setFillColor(tossup);					//CHANGE COLOR
		}
	}
}