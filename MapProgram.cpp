#include "MapProgram.h"
#include <array>
#include <string>
#include <sstream>

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
	this->videoMode.height = 800;
	this->videoMode.width = 1150;
	this->window = new sf::RenderWindow(this->videoMode, "Electoral Map", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(60);
}

void MapProgram::initOutline(int half_thickness)
{
	borders.outline_down.setFillColor(dem);
	borders.outline_down.setSize(sf::Vector2f(this->window->getSize().x, half_thickness));
	borders.outline_down.setPosition(0, this->window->getSize().y - half_thickness);

	borders.outline_left.setFillColor(dem);
	borders.outline_left.setSize(sf::Vector2f(half_thickness, this->window->getSize().y));
	borders.outline_left.setPosition(0, 0);

	borders.outline_right.setFillColor(dem);
	borders.outline_right.setSize(sf::Vector2f(half_thickness, this->window->getSize().y));
	borders.outline_right.setPosition(this->window->getSize().x - half_thickness, 0);

	borders.outline_up.setFillColor(dem);
	borders.outline_up.setSize(sf::Vector2f(this->window->getSize().x, half_thickness));
	borders.outline_up.setPosition(0, 0);

	borders.outline_down2.setFillColor(rep);
	borders.outline_down2.setSize(sf::Vector2f(this->window->getSize().x - half_thickness * 2, half_thickness));
	borders.outline_down2.setPosition(half_thickness, this->window->getSize().y - half_thickness * 2);

	borders.outline_left2.setFillColor(rep);
	borders.outline_left2.setSize(sf::Vector2f(half_thickness, this->window->getSize().y - half_thickness * 2));
	borders.outline_left2.setPosition(half_thickness, half_thickness);

	borders.outline_right2.setFillColor(rep);
	borders.outline_right2.setSize(sf::Vector2f(half_thickness, this->window->getSize().y - half_thickness * 2));
	borders.outline_right2.setPosition(this->window->getSize().x - half_thickness * 2, half_thickness);

	borders.outline_up2.setFillColor(rep);
	borders.outline_up2.setSize(sf::Vector2f(this->window->getSize().x - half_thickness * 2, half_thickness));
	borders.outline_up2.setPosition(half_thickness, half_thickness);
}

std::string MapProgram::int_to_string(int num)
{
	std::stringstream type;
	type << num;
	return type.str();
}

void MapProgram::initGrid(Pixel grid[][rows], sf::Vector2f pixel_size)
{
	Pixel pix;
	for (int i = 0; i < columns; i++) {
		for (int j = 0; j < rows; j++) {
			grid[i][j] = pix;
			grid[i][j].pixel_shape.setPosition(this->half_border_thickness * 4 + j * pixel_size.x, this->half_border_thickness * 4 + i * pixel_size.y);
			grid[i][j].pixel_shape.setSize(pixel_size);
			grid[i][j].pixel_shape.setFillColor(sf::Color::White);
			this->addToState(grid[i][j], i, j);
		}
	}
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
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		checkClickLeft();
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		checkClickRight();
	}
}

void MapProgram::checkClickLeft()
{
	sf::Vector2i click_pos = sf::Mouse::getPosition(*this->window);
	if (((click_pos.x >= grid[0][0].pixel_shape.getPosition().x) && (click_pos.x <= grid[0][0].pixel_shape.getPosition().x + rows * pixel_size.x)) && ((click_pos.y >= grid[0][0].pixel_shape.getPosition().y) && (click_pos.y <= grid[0][0].pixel_shape.getPosition().y + columns * pixel_size.y))) {
		int grid_x = static_cast<int>(click_pos.x - grid[0][0].pixel_shape.getPosition().x) / static_cast<int>(pixel_size.x);
		int grid_y = static_cast<int>(click_pos.y - grid[0][0].pixel_shape.getPosition().y) / static_cast<int>(pixel_size.y);
		if (grid[grid_y][grid_x].pixel_shape.getFillColor() != sf::Color::White) {
			for (int i = 0; i < grid[grid_y][grid_x].parent_state->pixel_count; i++) {
				grid[grid_y][grid_x].parent_state->state_pixels[i]->pixel_shape.setFillColor(dem);
			}
			if (grid[grid_y][grid_x].parent_state->result == "Tossup") {
				dem_total += grid[grid_y][grid_x].parent_state->votes;
			}
			else if (grid[grid_y][grid_x].parent_state->result == "Republican") {
				dem_total += grid[grid_y][grid_x].parent_state->votes;
				rep_total -= grid[grid_y][grid_x].parent_state->votes;
			}
			grid[grid_y][grid_x].parent_state->result = "Democrat";
		}
	}
}

void MapProgram::checkClickRight()
{
	sf::Vector2i click_pos = sf::Mouse::getPosition(*this->window);
	if (((click_pos.x >= grid[0][0].pixel_shape.getPosition().x) && (click_pos.x <= grid[0][0].pixel_shape.getPosition().x + rows * pixel_size.x)) && ((click_pos.y >= grid[0][0].pixel_shape.getPosition().y) && (click_pos.y <= grid[0][0].pixel_shape.getPosition().y + columns * pixel_size.y))) {
		int grid_x = static_cast<int>(click_pos.x - grid[0][0].pixel_shape.getPosition().x) / static_cast<int>(pixel_size.x);
		int grid_y = static_cast<int>(click_pos.y - grid[0][0].pixel_shape.getPosition().y) / static_cast<int>(pixel_size.y);
		if (grid[grid_y][grid_x].pixel_shape.getFillColor() != sf::Color::White) {
			for (int i = 0; i < grid[grid_y][grid_x].parent_state->pixel_count; i++) {
				grid[grid_y][grid_x].parent_state->state_pixels[i]->pixel_shape.setFillColor(rep);
			}
			if (grid[grid_y][grid_x].parent_state->result == "Tossup") {
				rep_total += grid[grid_y][grid_x].parent_state->votes;
			}
			else if (grid[grid_y][grid_x].parent_state->result == "Democrat") {
				rep_total += grid[grid_y][grid_x].parent_state->votes;
				dem_total -= grid[grid_y][grid_x].parent_state->votes;
			}
			grid[grid_y][grid_x].parent_state->result = "Republican";
		}
	}
}

void MapProgram::hover_over_state(sf::Text &name, sf::Text &votes)
{
	sf::Vector2i mouse_pos = sf::Mouse::getPosition(*this->window);
	if (((mouse_pos.x >= grid[0][0].pixel_shape.getPosition().x) && (mouse_pos.x <= grid[0][0].pixel_shape.getPosition().x + rows * pixel_size.x)) && ((mouse_pos.y >= grid[0][0].pixel_shape.getPosition().y) && (mouse_pos.y <= grid[0][0].pixel_shape.getPosition().y + columns * pixel_size.y))) {
		int grid_x = static_cast<int>(mouse_pos.x - grid[0][0].pixel_shape.getPosition().x) / static_cast<int>(pixel_size.x);
		int grid_y = static_cast<int>(mouse_pos.y - grid[0][0].pixel_shape.getPosition().y) / static_cast<int>(pixel_size.y);
		if (grid[grid_y][grid_x].pixel_shape.getFillColor() != sf::Color::White) {
			name.setString(grid[grid_y][grid_x].parent_state->name);
			name.setFillColor(sf::Color::Black);
			votes.setString(this->int_to_string(grid[grid_y][grid_x].parent_state->votes) + " delegates");
			votes.setFillColor(sf::Color::Black);
		}
		else {
			name.setFillColor(sf::Color::White);
			votes.setFillColor(sf::Color::White);
		}
	}
	else {
		name.setFillColor(sf::Color::White);
		votes.setFillColor(sf::Color::White);
	}
}

void MapProgram::update(sf::Text &name, sf::Text &votes, sf::Text &dem_votes, sf::Text &rep_votes, sf::Text &winner)
{
	pollEvents();
	this->mouse_pos = sf::Mouse::getPosition(*this->window);
	this->hover_over_state(name, votes);
}

void MapProgram::render(sf::Text name, sf::Text votes, sf::Text dem_votes, sf::Text rep_votes, sf::Text winner)
{
	this->window->clear(sf::Color::White);
	this->renderGrid();
	this->window->draw(name);
	this->window->draw(votes);
	this->window->draw(borders.outline_down);
	this->window->draw(borders.outline_down2);
	this->window->draw(borders.outline_up);
	this->window->draw(borders.outline_up2);
	this->window->draw(borders.outline_left);
	this->window->draw(borders.outline_left2);
	this->window->draw(borders.outline_right);
	this->window->draw(borders.outline_right2);

	demBar.setSize(sf::Vector2f(dem_total, 20.f));
	demBar.setPosition(window->getSize().x / 2 - demBar.getSize().x, window->getSize().y / 8 * 7);
	this->window->draw(this->demBar);
	repBar.setSize(sf::Vector2f(rep_total, 20.f));
	repBar.setPosition(window->getSize().x / 2, window->getSize().y / 8 * 7);
	this->window->draw(this->repBar);

	dem_votes.setPosition(window->getSize().x / 2 - demBar.getSize().x, window->getSize().y / 8 * 7 - 20);
	dem_votes.setString(this->int_to_string(dem_total));
	if (dem_total == 0) {
		dem_votes.setFillColor(sf::Color::White);
	}
	else {
		dem_votes.setFillColor(dem);
	}
	if (dem_total < 10) {
		dem_votes.setPosition(this->window->getSize().x / 2 - 10, window->getSize().y / 8 * 7 - 20);
	}
	else if (dem_total < 20) {
		dem_votes.setPosition(this->window->getSize().x /2 - 20, window->getSize().y / 8 * 7 - 20);
	}
	this->window->draw(dem_votes);
	rep_votes.setPosition(window->getSize().x / 2 + repBar.getSize().x - 20, window->getSize().y / 8 * 7 + 18);
	rep_votes.setString(this->int_to_string(rep_total));
	if (rep_total == 0) {
		rep_votes.setFillColor(sf::Color::White);
	}
	else {
		rep_votes.setFillColor(rep);
	}
	if (rep_total < 20) {
		rep_votes.setPosition(this->window->getSize().x / 2, window->getSize().y / 8 * 7 + 18);
	}
	this->window->draw(rep_votes);

	if (this->dem_total > 0) {
		this->winBarLeft.setFillColor(sf::Color::Black);
	}
	else {
		this->winBarLeft.setFillColor(sf::Color::White);
	}
	this->window->draw(winBarLeft);
	if (this->rep_total > 0) {
		this->winBarRight.setFillColor(sf::Color::Black);
	}
	else {
		this->winBarRight.setFillColor(sf::Color::White);
	}
	this->window->draw(winBarRight);

	if (this->dem_total >= 270) {
		winner.setFillColor(dem);
		winner.setString("Democrats win");
	}
	else if (this->rep_total >= 270) {
		winner.setFillColor(rep);
		winner.setString("Republicans win");
	}
	else {
		winner.setFillColor(sf::Color::White);
	}
	this->window->draw(winner);
	this->window->display();
}

void MapProgram::renderGrid()
{
	for (int i = 0; i < this->columns; i++) {
		for (int k = 0; k < this->rows; k++) {
			this->window->draw(this->grid[i][k].pixel_shape);
		}
	}
}

MapProgram::State::State(int votes, std::string name)
{
	this->votes = votes;
	this->name = name;
}

void MapProgram::addToState(Pixel &pixel, int column, int row) {
	if (column == 0) {
		if ((row >= 2) && (row <= 6)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->washington.state_pixels[this->washington.pixel_count] = &pixel;
			this->washington.pixel_count++;
			pixel.parent_state = &washington;
		}
		else if (row == 7) {
			pixel.pixel_shape.setFillColor(tossup);
			this->idaho.state_pixels[this->idaho.pixel_count] = &pixel;
			this->idaho.pixel_count++;
			pixel.parent_state = &idaho;
		}
	}
	else if (column == 1) {
		if ((row >= 2) && (row <= 6)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->washington.state_pixels[this->washington.pixel_count] = &pixel;
			this->washington.pixel_count++;
			pixel.parent_state = &washington;
		}
		else if (row == 7) {
			pixel.pixel_shape.setFillColor(tossup);
			this->idaho.state_pixels[this->idaho.pixel_count] = &pixel;
			this->idaho.pixel_count++;
			pixel.parent_state = &idaho;
		}
		else if ((row >= 8) && (row <= 14)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->montana.state_pixels[this->montana.pixel_count] = &pixel;
			this->montana.pixel_count++;
			pixel.parent_state = &montana;
		}
	}
	else if (column == 2) {
		if ((row >= 1) && (row <= 2)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->oregon.state_pixels[this->oregon.pixel_count] = &pixel;
			this->oregon.pixel_count++;
			pixel.parent_state = &oregon;
		}
		else if ((row >= 3) && (row <= 6)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->washington.state_pixels[this->washington.pixel_count] = &pixel;
			this->washington.pixel_count++;
			pixel.parent_state = &washington;
		}
		else if (row == 7) {
			pixel.pixel_shape.setFillColor(tossup);
			this->idaho.state_pixels[this->idaho.pixel_count] = &pixel;
			this->idaho.pixel_count++;
			pixel.parent_state = &idaho;
		}
		else if ((row >= 8) && (row <= 14)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->montana.state_pixels[this->montana.pixel_count] = &pixel;
			this->montana.pixel_count++;
			pixel.parent_state = &montana;
		}
		else if ((row >= 15) && (row <= 18)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->north_dakota.state_pixels[this->north_dakota.pixel_count] = &pixel;
			this->north_dakota.pixel_count++;
			pixel.parent_state = &north_dakota;
		}
		else if ((row >= 19) && (row <= 21)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->minnesota.state_pixels[this->minnesota.pixel_count] = &pixel;
			this->minnesota.pixel_count++;
			pixel.parent_state = &minnesota;
		}
		else if ((row >= 36) && (row <= 37)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->maine.state_pixels[this->maine.pixel_count] = &pixel;
			this->maine.pixel_count++;
			pixel.parent_state = &maine;
		}
	}
	else if (column == 3) {
		if ((row >= 1) && (row <= 5)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->oregon.state_pixels[this->oregon.pixel_count] = &pixel;
			this->oregon.pixel_count++;
			pixel.parent_state = &oregon;
		}
		else if ((row >= 6) && (row <= 7)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->idaho.state_pixels[this->idaho.pixel_count] = &pixel;
			this->idaho.pixel_count++;
			pixel.parent_state = &idaho;
		}
		else if ((row >= 8) && (row <= 14)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->montana.state_pixels[this->montana.pixel_count] = &pixel;
			this->montana.pixel_count++;
			pixel.parent_state = &montana;
		}
		else if ((row >= 15) && (row <= 18)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->north_dakota.state_pixels[this->north_dakota.pixel_count] = &pixel;
			this->north_dakota.pixel_count++;
			pixel.parent_state = &north_dakota;
		}
		else if ((row >= 19) && (row <= 23)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->minnesota.state_pixels[this->minnesota.pixel_count] = &pixel;
			this->minnesota.pixel_count++;
			pixel.parent_state = &minnesota;
		}
		else if ((row >= 36) && (row <= 38)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->maine.state_pixels[this->maine.pixel_count] = &pixel;
			this->maine.pixel_count++;
			pixel.parent_state = &maine;
		}
	}
	else if (column == 4) {
		if ((row >= 0) && (row <= 5)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->oregon.state_pixels[this->oregon.pixel_count] = &pixel;
			this->oregon.pixel_count++;
			pixel.parent_state = &oregon;
		}
		else if ((row >= 6) && (row <= 8)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->idaho.state_pixels[this->idaho.pixel_count] = &pixel;
			this->idaho.pixel_count++;
			pixel.parent_state = &idaho;
		}
		else if ((row >= 9) && (row <= 14)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->montana.state_pixels[this->montana.pixel_count] = &pixel;
			this->montana.pixel_count++;
			pixel.parent_state = &montana;
		}
		else if ((row >= 15) && (row <= 19)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->north_dakota.state_pixels[this->north_dakota.pixel_count] = &pixel;
			this->north_dakota.pixel_count++;
			pixel.parent_state = &north_dakota;
		}
		else if ((row >= 20) && (row <= 22)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->minnesota.state_pixels[this->minnesota.pixel_count] = &pixel;
			this->minnesota.pixel_count++;
			pixel.parent_state = &minnesota;
		}
		else if (row == 23) {
			pixel.pixel_shape.setFillColor(tossup);
			this->wisconsin.state_pixels[this->wisconsin.pixel_count] = &pixel;
			this->wisconsin.pixel_count++;
			pixel.parent_state = &wisconsin;
		}
		else if ((row >= 24) && (row <= 26)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->michigan.state_pixels[this->michigan.pixel_count] = &pixel;
			this->michigan.pixel_count++;
			pixel.parent_state = &michigan;
		}
		else if ((row >= 24) && (row <= 26)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->michigan.state_pixels[this->michigan.pixel_count] = &pixel;
			this->michigan.pixel_count++;
			pixel.parent_state = &michigan;
		}
		else if (row == 34) {
			pixel.pixel_shape.setFillColor(tossup);
			this->new_york.state_pixels[this->new_york.pixel_count] = &pixel;
			this->new_york.pixel_count++;
			pixel.parent_state = &new_york;
		}
		else if (row == 35) {
			pixel.pixel_shape.setFillColor(tossup);
			this->vermont.state_pixels[this->vermont.pixel_count] = &pixel;
			this->vermont.pixel_count++;
			pixel.parent_state = &vermont;
		}
		else if (row == 36) {
			pixel.pixel_shape.setFillColor(tossup);
			this->new_hampshire.state_pixels[this->new_hampshire.pixel_count] = &pixel;
			this->new_hampshire.pixel_count++;
			pixel.parent_state = &new_hampshire;
		}
		else if ((row >= 37) && (row <= 38)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->maine.state_pixels[this->maine.pixel_count] = &pixel;
			this->maine.pixel_count++;
			pixel.parent_state = &maine;
		}
	}
	else if (column == 5) {
		if ((row >= 0) && (row <= 5)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->oregon.state_pixels[this->oregon.pixel_count] = &pixel;
			this->oregon.pixel_count++;
			pixel.parent_state = &oregon;
		}
		else if ((row >= 6) && (row <= 8)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->idaho.state_pixels[this->idaho.pixel_count] = &pixel;
			this->idaho.pixel_count++;
			pixel.parent_state = &idaho;
		}
		else if (row == 9) {
			pixel.pixel_shape.setFillColor(tossup);
			this->montana.state_pixels[this->montana.pixel_count] = &pixel;
			this->montana.pixel_count++;
			pixel.parent_state = &montana;
		}
		else if ((row >= 10) && (row <= 14)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->wyoming.state_pixels[this->wyoming.pixel_count] = &pixel;
			this->wyoming.pixel_count++;
			pixel.parent_state = &wyoming;
		}
		else if ((row >= 15) && (row <= 19)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->south_dakota.state_pixels[this->south_dakota.pixel_count] = &pixel;
			this->south_dakota.pixel_count++;
			pixel.parent_state = &south_dakota;
		}
		else if ((row >= 20) && (row <= 21)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->minnesota.state_pixels[this->minnesota.pixel_count] = &pixel;
			this->minnesota.pixel_count++;
			pixel.parent_state = &minnesota;
		}
		else if ((row >= 22) && (row <= 24)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->wisconsin.state_pixels[this->wisconsin.pixel_count] = &pixel;
			this->wisconsin.pixel_count++;
			pixel.parent_state = &wisconsin;
		}
		else if ((row == 25) || (row == 27)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->michigan.state_pixels[this->michigan.pixel_count] = &pixel;
			this->michigan.pixel_count++;
			pixel.parent_state = &michigan;
		}
		else if ((row >= 33) && (row <= 34)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->new_york.state_pixels[this->new_york.pixel_count] = &pixel;
			this->new_york.pixel_count++;
			pixel.parent_state = &new_york;
		}
		else if (row == 35) {
			pixel.pixel_shape.setFillColor(tossup);
			this->vermont.state_pixels[this->vermont.pixel_count] = &pixel;
			this->vermont.pixel_count++;
			pixel.parent_state = &vermont;
		}
		else if (row == 36) {
			pixel.pixel_shape.setFillColor(tossup);
			this->new_hampshire.state_pixels[this->new_hampshire.pixel_count] = &pixel;
			this->new_hampshire.pixel_count++;
			pixel.parent_state = &new_hampshire;
		}
		else if (row == 37) {
			pixel.pixel_shape.setFillColor(tossup);
			this->maine.state_pixels[this->maine.pixel_count] = &pixel;
			this->maine.pixel_count++;
			pixel.parent_state = &maine;
		}
	}
	else if (column == 6) {
		if ((row >= 0) && (row <= 5)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->oregon.state_pixels[this->oregon.pixel_count] = &pixel;
			this->oregon.pixel_count++;
			pixel.parent_state = &oregon;
		}
		else if ((row >= 6) && (row <= 9)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->idaho.state_pixels[this->idaho.pixel_count] = &pixel;
			this->idaho.pixel_count++;
			pixel.parent_state = &idaho;
		}
		else if ((row >= 10) && (row <= 14)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->wyoming.state_pixels[this->wyoming.pixel_count] = &pixel;
			this->wyoming.pixel_count++;
			pixel.parent_state = &wyoming;
		}
		else if ((row >= 15) && (row <= 19)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->south_dakota.state_pixels[this->south_dakota.pixel_count] = &pixel;
			this->south_dakota.pixel_count++;
			pixel.parent_state = &south_dakota;
		}
		else if ((row >= 20) && (row <= 21)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->minnesota.state_pixels[this->minnesota.pixel_count] = &pixel;
			this->minnesota.pixel_count++;
			pixel.parent_state = &minnesota;
		}
		else if ((row >= 22) && (row <= 24)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->wisconsin.state_pixels[this->wisconsin.pixel_count] = &pixel;
			this->wisconsin.pixel_count++;
			pixel.parent_state = &wisconsin;
		}
		else if ((row >= 26) && (row <= 28)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->michigan.state_pixels[this->michigan.pixel_count] = &pixel;
			this->michigan.pixel_count++;
			pixel.parent_state = &michigan;
		}
		else if ((row >= 32) && (row <= 35)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->new_york.state_pixels[this->new_york.pixel_count] = &pixel;
			this->new_york.pixel_count++;
			pixel.parent_state = &new_york;
		}
		else if ((row >= 36) && (row <= 37)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->massachussets.state_pixels[this->massachussets.pixel_count] = &pixel;
			this->massachussets.pixel_count++;
			pixel.parent_state = &massachussets;
		}
	}
	else if (column == 7) {
		if ((row >= 0) && (row <= 5)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->oregon.state_pixels[this->oregon.pixel_count] = &pixel;
			this->oregon.pixel_count++;
			pixel.parent_state = &oregon;
		}
		else if ((row >= 6) && (row <= 9)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->idaho.state_pixels[this->idaho.pixel_count] = &pixel;
			this->idaho.pixel_count++;
			pixel.parent_state = &idaho;
		}
		else if ((row >= 10) && (row <= 14)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->wyoming.state_pixels[this->wyoming.pixel_count] = &pixel;
			this->wyoming.pixel_count++;
			pixel.parent_state = &wyoming;
		}
		else if ((row >= 15) && (row <= 19)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->south_dakota.state_pixels[this->south_dakota.pixel_count] = &pixel;
			this->south_dakota.pixel_count++;
			pixel.parent_state = &south_dakota;
		}
		else if ((row >= 20) && (row <= 23)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->iowa.state_pixels[this->iowa.pixel_count] = &pixel;
			this->iowa.pixel_count++;
			pixel.parent_state = &iowa;
		}
		else if (row == 24) {
			pixel.pixel_shape.setFillColor(tossup);
			this->wisconsin.state_pixels[this->wisconsin.pixel_count] = &pixel;
			this->wisconsin.pixel_count++;
			pixel.parent_state = &wisconsin;
		}
		else if ((row >= 26) && (row <= 28)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->michigan.state_pixels[this->michigan.pixel_count] = &pixel;
			this->michigan.pixel_count++;
			pixel.parent_state = &michigan;
		}
		else if ((row >= 31) && (row <= 35)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->new_york.state_pixels[this->new_york.pixel_count] = &pixel;
			this->new_york.pixel_count++;
			pixel.parent_state = &new_york;
		}
		else if (row == 36) {
			pixel.pixel_shape.setFillColor(tossup);
			this->connecticut.state_pixels[this->connecticut.pixel_count] = &pixel;
			this->connecticut.pixel_count++;
			pixel.parent_state = &connecticut;
		}
		else if (row == 37) {
			pixel.pixel_shape.setFillColor(tossup);
			this->rhode_island.state_pixels[this->rhode_island.pixel_count] = &pixel;
			this->rhode_island.pixel_count++;
			pixel.parent_state = &rhode_island;
		}
	}
	else if (column == 8) {
		if ((row >= 0) && (row <= 2)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->california.state_pixels[this->california.pixel_count] = &pixel;
			this->california.pixel_count++;
			pixel.parent_state = &california;
		}
		else if ((row >= 3) && (row <= 6)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->nevada.state_pixels[this->nevada.pixel_count] = &pixel;
			this->nevada.pixel_count++;
			pixel.parent_state = &nevada;
		}
		else if ((row >= 7) && (row <= 9)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->utah.state_pixels[this->utah.pixel_count] = &pixel;
			this->utah.pixel_count++;
			pixel.parent_state = &utah;
		}
		else if ((row >= 10) && (row <= 14)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->wyoming.state_pixels[this->wyoming.pixel_count] = &pixel;
			this->wyoming.pixel_count++;
			pixel.parent_state = &wyoming;
		}
		else if ((row >= 15) && (row <= 18)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->nebraska.state_pixels[this->nebraska.pixel_count] = &pixel;
			this->nebraska.pixel_count++;
			pixel.parent_state = &nebraska;
		}
		else if (row == 19) {
			pixel.pixel_shape.setFillColor(tossup);
			this->south_dakota.state_pixels[this->south_dakota.pixel_count] = &pixel;
			this->south_dakota.pixel_count++;
			pixel.parent_state = &south_dakota;
		}
		else if ((row >= 20) && (row <= 23)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->iowa.state_pixels[this->iowa.pixel_count] = &pixel;
			this->iowa.pixel_count++;
			pixel.parent_state = &iowa;
		}
		else if (row == 24) {
			pixel.pixel_shape.setFillColor(tossup);
			this->illinois.state_pixels[this->illinois.pixel_count] = &pixel;
			this->illinois.pixel_count++;
			pixel.parent_state = &illinois;
		}
		else if ((row >= 26) && (row <= 28)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->michigan.state_pixels[this->michigan.pixel_count] = &pixel;
			this->michigan.pixel_count++;
			pixel.parent_state = &michigan;
		}
		else if (row == 30) {
			pixel.pixel_shape.setFillColor(tossup);
			this->ohio.state_pixels[this->ohio.pixel_count] = &pixel;
			this->ohio.pixel_count++;
			pixel.parent_state = &ohio;
		}
		else if ((row >= 31) && (row <= 34)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->pennsylvania.state_pixels[this->pennsylvania.pixel_count] = &pixel;
			this->pennsylvania.pixel_count++;
			pixel.parent_state = &pennsylvania;
		}
		else if (row == 35) {
			pixel.pixel_shape.setFillColor(tossup);
			this->new_jersey.state_pixels[this->new_jersey.pixel_count] = &pixel;
			this->new_jersey.pixel_count++;
			pixel.parent_state = &new_jersey;
		}
		else if (row == 36) {
			pixel.pixel_shape.setFillColor(tossup);
			this->new_york.state_pixels[this->new_york.pixel_count] = &pixel;
			this->new_york.pixel_count++;
			pixel.parent_state = &new_york;
		}
	}
	else if (column == 9) {
		if ((row >= 0) && (row <= 2)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->california.state_pixels[this->california.pixel_count] = &pixel;
			this->california.pixel_count++;
			pixel.parent_state = &california;
		}
		else if ((row >= 3) && (row <= 6)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->nevada.state_pixels[this->nevada.pixel_count] = &pixel;
			this->nevada.pixel_count++;
			pixel.parent_state = &nevada;
		}
		else if ((row >= 7) && (row <= 10)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->utah.state_pixels[this->utah.pixel_count] = &pixel;
			this->utah.pixel_count++;
			pixel.parent_state = &utah;
		}
		else if ((row >= 11) && (row <= 15)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->colorado.state_pixels[this->colorado.pixel_count] = &pixel;
			this->colorado.pixel_count++;
			pixel.parent_state = &colorado;
		}
		else if ((row >= 16) && (row <= 19)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->nebraska.state_pixels[this->nebraska.pixel_count] = &pixel;
			this->nebraska.pixel_count++;
			pixel.parent_state = &nebraska;
		}
		else if ((row >= 20) && (row <= 23)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->iowa.state_pixels[this->iowa.pixel_count] = &pixel;
			this->iowa.pixel_count++;
			pixel.parent_state = &iowa;
		}
		else if ((row >= 24) && (row <= 25)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->illinois.state_pixels[this->illinois.pixel_count] = &pixel;
			this->illinois.pixel_count++;
			pixel.parent_state = &illinois;
		}
		else if ((row >= 26) && (row <= 27)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->indiana.state_pixels[this->indiana.pixel_count] = &pixel;
			this->indiana.pixel_count++;
			pixel.parent_state = &indiana;
		}
		else if ((row >= 28) && (row <= 30)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->ohio.state_pixels[this->ohio.pixel_count] = &pixel;
			this->ohio.pixel_count++;
			pixel.parent_state = &ohio;
		}
		else if ((row >= 31) && (row <= 34)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->pennsylvania.state_pixels[this->pennsylvania.pixel_count] = &pixel;
			this->pennsylvania.pixel_count++;
			pixel.parent_state = &pennsylvania;
		}
		else if (row == 35) {
			pixel.pixel_shape.setFillColor(tossup);
			this->new_jersey.state_pixels[this->new_jersey.pixel_count] = &pixel;
			this->new_jersey.pixel_count++;
			pixel.parent_state = &new_jersey;
		}
	}
	else if (column == 10) {
		if ((row >= 0) && (row <= 2)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->california.state_pixels[this->california.pixel_count] = &pixel;
			this->california.pixel_count++;
			pixel.parent_state = &california;
		}
		else if ((row >= 3) && (row <= 6)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->nevada.state_pixels[this->nevada.pixel_count] = &pixel;
			this->nevada.pixel_count++;
			pixel.parent_state = &nevada;
		}
		else if ((row >= 7) && (row <= 10)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->utah.state_pixels[this->utah.pixel_count] = &pixel;
			this->utah.pixel_count++;
			pixel.parent_state = &utah;
		}
		else if ((row >= 11) && (row <= 15)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->colorado.state_pixels[this->colorado.pixel_count] = &pixel;
			this->colorado.pixel_count++;
			pixel.parent_state = &colorado;
		}
		else if ((row >= 16) && (row <= 19)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->nebraska.state_pixels[this->nebraska.pixel_count] = &pixel;
			this->nebraska.pixel_count++;
			pixel.parent_state = &nebraska;
		}
		else if ((row >= 20) && (row <= 22)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->missouri.state_pixels[this->missouri.pixel_count] = &pixel;
			this->missouri.pixel_count++;
			pixel.parent_state = &missouri;
		}
		else if ((row >= 23) && (row <= 25)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->illinois.state_pixels[this->illinois.pixel_count] = &pixel;
			this->illinois.pixel_count++;
			pixel.parent_state = &illinois;
		}
		else if ((row >= 26) && (row <= 27)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->indiana.state_pixels[this->indiana.pixel_count] = &pixel;
			this->indiana.pixel_count++;
			pixel.parent_state = &indiana;
		}
		else if ((row >= 28) && (row <= 30)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->ohio.state_pixels[this->ohio.pixel_count] = &pixel;
			this->ohio.pixel_count++;
			pixel.parent_state = &ohio;
		}
		else if (row == 31) {
			pixel.pixel_shape.setFillColor(tossup);
			this->west_virginia.state_pixels[this->west_virginia.pixel_count] = &pixel;
			this->west_virginia.pixel_count++;
			pixel.parent_state = &west_virginia;
		}
		else if ((row >= 32) && (row <= 35)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->maryland.state_pixels[this->maryland.pixel_count] = &pixel;
			this->maryland.pixel_count++;
			pixel.parent_state = &maryland;
		}
		else if (row == 36) {
			pixel.pixel_shape.setFillColor(tossup);
			this->delaware.state_pixels[this->delaware.pixel_count] = &pixel;
			this->delaware.pixel_count++;
			pixel.parent_state = &delaware;
		}
	}
	else if (column == 11) {
		if ((row >= 0) && (row <= 3)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->california.state_pixels[this->california.pixel_count] = &pixel;
			this->california.pixel_count++;
			pixel.parent_state = &california;
		}
		else if ((row >= 4) && (row <= 6)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->nevada.state_pixels[this->nevada.pixel_count] = &pixel;
			this->nevada.pixel_count++;
			pixel.parent_state = &nevada;
		}
		else if ((row >= 7) && (row <= 10)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->utah.state_pixels[this->utah.pixel_count] = &pixel;
			this->utah.pixel_count++;
			pixel.parent_state = &utah;
		}
		else if ((row >= 11) && (row <= 15)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->colorado.state_pixels[this->colorado.pixel_count] = &pixel;
			this->colorado.pixel_count++;
			pixel.parent_state = &colorado;
		}
		else if ((row >= 16) && (row <= 19)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->kansas.state_pixels[this->kansas.pixel_count] = &pixel;
			this->kansas.pixel_count++;
			pixel.parent_state = &kansas;
		}
		else if ((row >= 20) && (row <= 22)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->missouri.state_pixels[this->missouri.pixel_count] = &pixel;
			this->missouri.pixel_count++;
			pixel.parent_state = &missouri;
		}
		else if ((row >= 23) && (row <= 25)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->illinois.state_pixels[this->illinois.pixel_count] = &pixel;
			this->illinois.pixel_count++;
			pixel.parent_state = &illinois;
		}
		else if ((row >= 26) && (row <= 27)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->indiana.state_pixels[this->indiana.pixel_count] = &pixel;
			this->indiana.pixel_count++;
			pixel.parent_state = &indiana;
		}
		else if (row == 28) {
			pixel.pixel_shape.setFillColor(tossup);
			this->kentucky.state_pixels[this->kentucky.pixel_count] = &pixel;
			this->kentucky.pixel_count++;
			pixel.parent_state = &kentucky;
		}
		else if (row <= 29) {
			pixel.pixel_shape.setFillColor(tossup);
			this->ohio.state_pixels[this->ohio.pixel_count] = &pixel;
			this->ohio.pixel_count++;
			pixel.parent_state = &ohio;
		}
		else if ((row >= 30) && (row <= 31)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->west_virginia.state_pixels[this->west_virginia.pixel_count] = &pixel;
			this->west_virginia.pixel_count++;
			pixel.parent_state = &west_virginia;
		}
		else if (row == 32) {
			pixel.pixel_shape.setFillColor(tossup);
			this->virginia.state_pixels[this->virginia.pixel_count] = &pixel;
			this->virginia.pixel_count++;
			pixel.parent_state = &virginia;
		}
		else if ((row >= 33) && (row <= 36)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->maryland.state_pixels[this->maryland.pixel_count] = &pixel;
			this->maryland.pixel_count++;
			pixel.parent_state = &maryland;
		}
	}
	else if (column == 12) {
		if ((row >= 1) && (row <= 4)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->california.state_pixels[this->california.pixel_count] = &pixel;
			this->california.pixel_count++;
			pixel.parent_state = &california;
		}
		else if ((row >= 3) && (row <= 6)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->nevada.state_pixels[this->nevada.pixel_count] = &pixel;
			this->nevada.pixel_count++;
			pixel.parent_state = &nevada;
		}
		else if ((row >= 7) && (row <= 10)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->utah.state_pixels[this->utah.pixel_count] = &pixel;
			this->utah.pixel_count++;
			pixel.parent_state = &utah;
		}
		else if ((row >= 11) && (row <= 15)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->colorado.state_pixels[this->colorado.pixel_count] = &pixel;
			this->colorado.pixel_count++;
			pixel.parent_state = &colorado;
		}
		else if ((row >= 16) && (row <= 19)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->kansas.state_pixels[this->kansas.pixel_count] = &pixel;
			this->kansas.pixel_count++;
			pixel.parent_state = &kansas;
		}
		else if ((row >= 20) && (row <= 23)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->missouri.state_pixels[this->missouri.pixel_count] = &pixel;
			this->missouri.pixel_count++;
			pixel.parent_state = &missouri;
		}
		else if ((row >= 24) && (row <= 25)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->illinois.state_pixels[this->illinois.pixel_count] = &pixel;
			this->illinois.pixel_count++;
			pixel.parent_state = &illinois;
		}
		else if ((row >= 26) && (row <= 29)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->kentucky.state_pixels[this->kentucky.pixel_count] = &pixel;
			this->kentucky.pixel_count++;
			pixel.parent_state = &kentucky;
		}
		else if (row == 30) {
			pixel.pixel_shape.setFillColor(tossup);
			this->west_virginia.state_pixels[this->west_virginia.pixel_count] = &pixel;
			this->west_virginia.pixel_count++;
			pixel.parent_state = &west_virginia;
		}
		else if ((row >= 31) && (row <= 34)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->virginia.state_pixels[this->virginia.pixel_count] = &pixel;
			this->virginia.pixel_count++;
			pixel.parent_state = &virginia;
		}
	}
	else if (column == 13) {
		if ((row >= 1) && (row <= 5)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->california.state_pixels[this->california.pixel_count] = &pixel;
			this->california.pixel_count++;
			pixel.parent_state = &california;
		}
		else if ((row >= 6) && (row <= 10)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->arizona.state_pixels[this->arizona.pixel_count] = &pixel;
			this->arizona.pixel_count++;
			pixel.parent_state = &arizona;
		}
		else if ((row >= 11) && (row <= 14)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->new_mexico.state_pixels[this->new_mexico.pixel_count] = &pixel;
			this->new_mexico.pixel_count++;
			pixel.parent_state = &new_mexico;
		}
		else if ((row >= 15) && (row <= 20)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->oklahoma.state_pixels[this->oklahoma.pixel_count] = &pixel;
			this->oklahoma.pixel_count++;
			pixel.parent_state = &oklahoma;
		}
		else if ((row >= 21) && (row <= 24)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->missouri.state_pixels[this->missouri.pixel_count] = &pixel;
			this->missouri.pixel_count++;
			pixel.parent_state = &missouri;
		}
		else if ((row >= 25) && (row <= 28)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->kentucky.state_pixels[this->kentucky.pixel_count] = &pixel;
			this->kentucky.pixel_count++;
			pixel.parent_state = &kentucky;
		}
		else if ((row >= 29) && (row <= 34)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->virginia.state_pixels[this->virginia.pixel_count] = &pixel;
			this->virginia.pixel_count++;
			pixel.parent_state = &virginia;
		}
	}
	else if (column == 14) {
		if ((row >= 2) && (row <= 5)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->california.state_pixels[this->california.pixel_count] = &pixel;
			this->california.pixel_count++;
			pixel.parent_state = &california;
		}
		else if ((row >= 6) && (row <= 10)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->arizona.state_pixels[this->arizona.pixel_count] = &pixel;
			this->arizona.pixel_count++;
			pixel.parent_state = &arizona;
		}
		else if ((row >= 11) && (row <= 14)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->new_mexico.state_pixels[this->new_mexico.pixel_count] = &pixel;
			this->new_mexico.pixel_count++;
			pixel.parent_state = &new_mexico;
		}
		else if ((row >= 15) && (row <= 16)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->texas.state_pixels[this->texas.pixel_count] = &pixel;
			this->texas.pixel_count++;
			pixel.parent_state = &texas;
		}
		else if ((row >= 17) && (row <= 20)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->oklahoma.state_pixels[this->oklahoma.pixel_count] = &pixel;
			this->oklahoma.pixel_count++;
			pixel.parent_state = &oklahoma;
		}
		else if ((row >= 21) && (row <= 24)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->arkansas.state_pixels[this->arkansas.pixel_count] = &pixel;
			this->arkansas.pixel_count++;
			pixel.parent_state = &arkansas;
		}
		else if ((row >= 25) && (row <= 29)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->tennessee.state_pixels[this->tennessee.pixel_count] = &pixel;
			this->tennessee.pixel_count++;
			pixel.parent_state = &tennessee;
		}
		else if ((row >= 30) && (row <= 33)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->north_carolina.state_pixels[this->north_carolina.pixel_count] = &pixel;
			this->north_carolina.pixel_count++;
			pixel.parent_state = &north_carolina;
		}
	}
	else if (column == 15) {
		if ((row >= 3) && (row <= 5)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->california.state_pixels[this->california.pixel_count] = &pixel;
			this->california.pixel_count++;
			pixel.parent_state = &california;
		}
		else if ((row >= 6) && (row <= 10)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->arizona.state_pixels[this->arizona.pixel_count] = &pixel;
			this->arizona.pixel_count++;
			pixel.parent_state = &arizona;
		}
		else if ((row >= 11) && (row <= 14)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->new_mexico.state_pixels[this->new_mexico.pixel_count] = &pixel;
			this->new_mexico.pixel_count++;
			pixel.parent_state = &new_mexico;
		}
		else if ((row >= 15) && (row <= 16)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->texas.state_pixels[this->texas.pixel_count] = &pixel;
			this->texas.pixel_count++;
			pixel.parent_state = &texas;
		}
		else if ((row >= 17) && (row <= 20)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->oklahoma.state_pixels[this->oklahoma.pixel_count] = &pixel;
			this->oklahoma.pixel_count++;
			pixel.parent_state = &oklahoma;
		}
		else if ((row >= 21) && (row <= 23)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->arkansas.state_pixels[this->arkansas.pixel_count] = &pixel;
			this->arkansas.pixel_count++;
			pixel.parent_state = &arkansas;
		}
		else if ((row >= 24) && (row <= 28)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->tennessee.state_pixels[this->tennessee.pixel_count] = &pixel;
			this->tennessee.pixel_count++;
			pixel.parent_state = &tennessee;
		}
		else if (((row >= 29) && (row <= 30)) || ((row >= 32) && (row <= 33))) {
			pixel.pixel_shape.setFillColor(tossup);
			this->north_carolina.state_pixels[this->north_carolina.pixel_count] = &pixel;
			this->north_carolina.pixel_count++;
			pixel.parent_state = &north_carolina;
		}
		else if (row == 31) {
			pixel.pixel_shape.setFillColor(tossup);
			this->south_carolina.state_pixels[this->south_carolina.pixel_count] = &pixel;
			this->south_carolina.pixel_count++;
			pixel.parent_state = &south_carolina;
		}
	}
	else if (column == 16) {
		if ((row >= 5) && (row <= 10)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->arizona.state_pixels[this->arizona.pixel_count] = &pixel;
			this->arizona.pixel_count++;
			pixel.parent_state = &arizona;
		}
		else if ((row >= 11) && (row <= 14)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->new_mexico.state_pixels[this->new_mexico.pixel_count] = &pixel;
			this->new_mexico.pixel_count++;
			pixel.parent_state = &new_mexico;
		}
		else if ((row >= 15) && (row <= 16)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->texas.state_pixels[this->texas.pixel_count] = &pixel;
			this->texas.pixel_count++;
			pixel.parent_state = &texas;
		}
		else if ((row >= 17) && (row <= 20)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->oklahoma.state_pixels[this->oklahoma.pixel_count] = &pixel;
			this->oklahoma.pixel_count++;
			pixel.parent_state = &oklahoma;
		}
		else if ((row >= 21) && (row <= 23)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->arkansas.state_pixels[this->arkansas.pixel_count] = &pixel;
			this->arkansas.pixel_count++;
			pixel.parent_state = &arkansas;
		}
		else if ((row >= 24) && (row <= 25)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->mississippi.state_pixels[this->mississippi.pixel_count] = &pixel;
			this->mississippi.pixel_count++;
			pixel.parent_state = &mississippi;
		}
		else if ((row >= 26) && (row <= 27)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->alabama.state_pixels[this->alabama.pixel_count] = &pixel;
			this->alabama.pixel_count++;
			pixel.parent_state = &alabama;
		}
		else if ((row >= 28) && (row <= 29)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->georgia.state_pixels[this->georgia.pixel_count] = &pixel;
			this->georgia.pixel_count++;
			pixel.parent_state = &georgia;
		}
		else if ((row >= 30) && (row <= 32)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->south_carolina.state_pixels[this->south_carolina.pixel_count] = &pixel;
			this->south_carolina.pixel_count++;
			pixel.parent_state = &south_carolina;
		}
	}
	else if (column == 17) {
		if ((row >= 7) && (row <= 10)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->arizona.state_pixels[this->arizona.pixel_count] = &pixel;
			this->arizona.pixel_count++;
			pixel.parent_state = &arizona;
		}
		else if ((row >= 11) && (row <= 14)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->new_mexico.state_pixels[this->new_mexico.pixel_count] = &pixel;
			this->new_mexico.pixel_count++;
			pixel.parent_state = &new_mexico;
		}
		else if ((row >= 15) && (row <= 20)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->texas.state_pixels[this->texas.pixel_count] = &pixel;
			this->texas.pixel_count++;
			pixel.parent_state = &texas;
		}
		else if ((row >= 21) && (row <= 23)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->louisiana.state_pixels[this->louisiana.pixel_count] = &pixel;
			this->louisiana.pixel_count++;
			pixel.parent_state = &louisiana;
		}
		else if ((row >= 24) && (row <= 25)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->mississippi.state_pixels[this->mississippi.pixel_count] = &pixel;
			this->mississippi.pixel_count++;
			pixel.parent_state = &mississippi;
		}
		else if ((row >= 26) && (row <= 27)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->alabama.state_pixels[this->alabama.pixel_count] = &pixel;
			this->alabama.pixel_count++;
			pixel.parent_state = &alabama;
		}
		else if ((row >= 28) && (row <= 30)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->georgia.state_pixels[this->georgia.pixel_count] = &pixel;
			this->georgia.pixel_count++;
			pixel.parent_state = &georgia;
		}
		else if (row == 31) {
			pixel.pixel_shape.setFillColor(tossup);
			this->south_carolina.state_pixels[this->south_carolina.pixel_count] = &pixel;
			this->south_carolina.pixel_count++;
			pixel.parent_state = &south_carolina;
		}
	}
	else if (column == 18) {
		if ((row >= 12) && (row <= 20)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->texas.state_pixels[this->texas.pixel_count] = &pixel;
			this->texas.pixel_count++;
			pixel.parent_state = &texas;
		}
		else if ((row >= 21) && (row <= 22)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->louisiana.state_pixels[this->louisiana.pixel_count] = &pixel;
			this->louisiana.pixel_count++;
			pixel.parent_state = &louisiana;
		}
		else if ((row >= 23) && (row <= 25)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->mississippi.state_pixels[this->mississippi.pixel_count] = &pixel;
			this->mississippi.pixel_count++;
			pixel.parent_state = &mississippi;
		}
		else if ((row >= 26) && (row <= 27)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->alabama.state_pixels[this->alabama.pixel_count] = &pixel;
			this->alabama.pixel_count++;
			pixel.parent_state = &alabama;
		}
		else if ((row >= 28) && (row <= 31)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->georgia.state_pixels[this->georgia.pixel_count] = &pixel;
			this->georgia.pixel_count++;
			pixel.parent_state = &georgia;
		}
	}
	else if (column == 19) {
		if ((row >= 13) && (row <= 21)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->texas.state_pixels[this->texas.pixel_count] = &pixel;
			this->texas.pixel_count++;
			pixel.parent_state = &texas;
		}
		else if ((row >= 22) && (row <= 24)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->louisiana.state_pixels[this->louisiana.pixel_count] = &pixel;
			this->louisiana.pixel_count++;
			pixel.parent_state = &louisiana;
		}
		else if (row == 25) {
			pixel.pixel_shape.setFillColor(tossup);
			this->mississippi.state_pixels[this->mississippi.pixel_count] = &pixel;
			this->mississippi.pixel_count++;
			pixel.parent_state = &mississippi;
		}
		else if (row == 26) {
			pixel.pixel_shape.setFillColor(tossup);
			this->alabama.state_pixels[this->alabama.pixel_count] = &pixel;
			this->alabama.pixel_count++;
			pixel.parent_state = &alabama;
		}
		else if ((row >= 27) && (row <= 31)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->florida.state_pixels[this->florida.pixel_count] = &pixel;
			this->florida.pixel_count++;
			pixel.parent_state = &florida;
		}
	}
	else if (column == 20) {
		if ((row == 13) || (row >= 15) && (row <= 20)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->texas.state_pixels[this->texas.pixel_count] = &pixel;
			this->texas.pixel_count++;
			pixel.parent_state = &texas;
		}
		else if ((row >= 21) && (row <= 24)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->louisiana.state_pixels[this->louisiana.pixel_count] = &pixel;
			this->louisiana.pixel_count++;
			pixel.parent_state = &louisiana;
		}
		else if ((row >= 29) && (row <= 31)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->florida.state_pixels[this->florida.pixel_count] = &pixel;
			this->florida.pixel_count++;
			pixel.parent_state = &florida;
		}
	}
	else if (column == 21) {
		if ((row >= 16) && (row <= 19)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->texas.state_pixels[this->texas.pixel_count] = &pixel;
			this->texas.pixel_count++;
			pixel.parent_state = &texas;
		}
		else if (row == 24) {
			pixel.pixel_shape.setFillColor(tossup);
			this->louisiana.state_pixels[this->louisiana.pixel_count] = &pixel;
			this->louisiana.pixel_count++;
			pixel.parent_state = &louisiana;
		}
		else if ((row >= 30) && (row <= 32)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->florida.state_pixels[this->florida.pixel_count] = &pixel;
			this->florida.pixel_count++;
			pixel.parent_state = &florida;
		}
	}
	else if (column == 22) {
		if ((row >= 17) && (row <= 18)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->texas.state_pixels[this->texas.pixel_count] = &pixel;
			this->texas.pixel_count++;
			pixel.parent_state = &texas;
		}
		else if ((row >= 30) && (row <= 32)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->florida.state_pixels[this->florida.pixel_count] = &pixel;
			this->florida.pixel_count++;
			pixel.parent_state = &florida;
		}
	}
	else if (column == 23) {
		if ((row >= 17) && (row <= 18)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->texas.state_pixels[this->texas.pixel_count] = &pixel;
			this->texas.pixel_count++;
			pixel.parent_state = &texas;
		}
		else if ((row >= 31) && (row <= 32)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->florida.state_pixels[this->florida.pixel_count] = &pixel;
			this->florida.pixel_count++;
			pixel.parent_state = &florida;
		}
	}
	else if (column == 24) {
		if ((row >= 31) && (row <= 32)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->florida.state_pixels[this->florida.pixel_count] = &pixel;
			this->florida.pixel_count++;
			pixel.parent_state = &florida;
		}
	}
	else if (column == 26) {
		if ((row >= 4) && (row <= 8)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->alaska.state_pixels[this->alaska.pixel_count] = &pixel;
			this->alaska.pixel_count++;
			pixel.parent_state = &alaska;
		}
	}
	else if (column == 27) {
		if ((row >= 4) && (row <= 8)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->alaska.state_pixels[this->alaska.pixel_count] = &pixel;
			this->alaska.pixel_count++;
			pixel.parent_state = &alaska;
		}
		if (row == 20) {
			pixel.pixel_shape.setFillColor(tossup);
			this->hawaii.state_pixels[this->hawaii.pixel_count] = &pixel;
			this->hawaii.pixel_count++;
			pixel.parent_state = &hawaii;
		}
	}
	else if (column == 28) {
		if ((row >= 3) && (row <= 8)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->alaska.state_pixels[this->alaska.pixel_count] = &pixel;
			this->alaska.pixel_count++;
			pixel.parent_state = &alaska;
		}
		if (row == 22) {
			pixel.pixel_shape.setFillColor(tossup);
			this->hawaii.state_pixels[this->hawaii.pixel_count] = &pixel;
			this->hawaii.pixel_count++;
			pixel.parent_state = &hawaii;
		}
	}
	else if (column == 29) {
		if ((row >= 4) && (row <= 8)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->alaska.state_pixels[this->alaska.pixel_count] = &pixel;
			this->alaska.pixel_count++;
			pixel.parent_state = &alaska;
		}
		if (row == 23) {
			pixel.pixel_shape.setFillColor(tossup);
			this->hawaii.state_pixels[this->hawaii.pixel_count] = &pixel;
			this->hawaii.pixel_count++;
			pixel.parent_state = &hawaii;
		}
	}
	else if (column == 30) {
		if ((row >= 2) && (row <= 8)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->alaska.state_pixels[this->alaska.pixel_count] = &pixel;
			this->alaska.pixel_count++;
			pixel.parent_state = &alaska;
		}
	}
	else if (column == 31) {
		if ((row >= 2) && (row <= 8)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->alaska.state_pixels[this->alaska.pixel_count] = &pixel;
			this->alaska.pixel_count++;
			pixel.parent_state = &alaska;
		}
		if (row == 24) {
			pixel.pixel_shape.setFillColor(tossup);
			this->hawaii.state_pixels[this->hawaii.pixel_count] = &pixel;
			this->hawaii.pixel_count++;
			pixel.parent_state = &hawaii;
		}
	}
	else if (column == 32) {
		if ((row == 3) || (row == 9)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->alaska.state_pixels[this->alaska.pixel_count] = &pixel;
			this->alaska.pixel_count++;
			pixel.parent_state = &alaska;
		}
	}
	else if (column == 33) {
		if ((row >= 2) && (row <= 3) || (row >= 10) && (row <= 11)) {
			pixel.pixel_shape.setFillColor(tossup);
			this->alaska.state_pixels[this->alaska.pixel_count] = &pixel;
			this->alaska.pixel_count++;
			pixel.parent_state = &alaska;
		}
	}
	else if (column == 34) {
		if (row == 11) {
			pixel.pixel_shape.setFillColor(tossup);
			this->alaska.state_pixels[this->alaska.pixel_count] = &pixel;
			this->alaska.pixel_count++;
			pixel.parent_state = &alaska;
		}
	}
}