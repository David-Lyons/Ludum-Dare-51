#include "PPU466.hpp"
#include "Mode.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	//functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const& drawable_size) override;

	enum Turn {
		PLAYER1,
		PLAYER2
	} turn;

	enum Color {
		NONE,
		RED,
		BLACK
	} color;

	void swap_color();
	void switch_turn();
	void fill_slot(int x, int y);

	Color board[7][6];
	float time;
	bool colors_swapped;
	bool game_over;
	Turn winner;

	//----- drawing handled by PPU466 -----
	PPU466 ppu;
};
