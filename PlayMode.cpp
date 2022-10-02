#include "PlayMode.hpp"

//for the GL_ERRORS() macro:
#include "gl_errors.hpp"

//for glm::value_ptr() :
#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <time.h>
#include <fstream>

#include "data_path.hpp"
#include "load_save_png.hpp"
#include "read_write_chunk.hpp"
#include "asset_script.hpp"

PlayMode::PlayMode() {
	create_assets();

	// Background sky blue
	ppu.background_color = glm::u8vec3(150, 200, 250);

	// Blank out all palettes, tiles, background tiles, and sprites
	for (uint32_t i = 0; i < 8; i++) {
		ppu.palette_table[i] = { glm::u8vec4(0,0,0,0), glm::u8vec4(0,0,0,0),
			glm::u8vec4(0,0,0,0), glm::u8vec4(0,0,0,0) };
	}
	for (uint32_t i = 0; i < 256; i++) {
		ppu.tile_table[i] = { { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 } };
	}
	for (uint32_t i = 0; i < PPU466::BackgroundWidth * PPU466::BackgroundHeight; i++) {
		ppu.background[i] = 0;
	}
	for (uint32_t i = 0; i < 64; i++) {
		ppu.sprites[i] = { 0, 250, 0, 0 };
	}

	// Load runtime assets into PPU
	std::ifstream datafile(data_path("RuntimeAssets.bin"), std::ios::in);
	std::vector< PPU466::Palette > palettes;
	std::vector< PPU466::Tile > tiles;
	std::vector< PPU466::Sprite > sprites;
	read_chunk(datafile, "PALT", &palettes);
	read_chunk(datafile, "TILE", &tiles);
	for (uint32_t i = 0; i < palettes.size(); i++) {
		ppu.palette_table[i] = palettes[i];
	}
	for (uint32_t i = 0; i < tiles.size(); i++) {
		ppu.tile_table[i] = tiles[i];
	}

	// Initialize board
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 6; j++) {
			board[i][j] = Color::NONE;
		}
	}
	turn = Turn::PLAYER1;
	game_over = false;
	colors_swapped = false;
	color = Color::RED;
	time = 0.0f;

	// Draw an empty board
	for (uint16_t i = 0; i < 30; i++) {
		for (uint16_t j = 0; j < 32; j++) {
			if (i < 12 && j > 9 && j < 24) {
				if (i % 2 == 0 && j % 2 == 0) {
					ppu.background[i * PPU466::BackgroundWidth + j] = 17;
				} else if (i % 2 == 0) {
					ppu.background[i * PPU466::BackgroundWidth + j] = 18;
				} else if (j % 2 == 0) {
					ppu.background[i * PPU466::BackgroundWidth + j] = 15;
				} else {
					ppu.background[i * PPU466::BackgroundWidth + j] = 16;
				}
			} else if (i == 12 && j > 9 && j < 24 && j % 2 != 0) {
				ppu.background[i * PPU466::BackgroundWidth + j] = (3 << 8) | (j - 10);
			} else if (i == 13 && j > 9 && j < 24 && j % 2 != 0) {
				ppu.background[i * PPU466::BackgroundWidth + j] = (3 << 8) | (j - 9);
			} else if (j == 9 && i < 12) {
				ppu.background[i * PPU466::BackgroundWidth + j] = (3 << 8) | (i + 1);
			}
		}
	}

	// Draw the player/color indicators
	ppu.background[23 * PPU466::BackgroundWidth + 2] = (3 << 8) | 1;
	ppu.background[24 * PPU466::BackgroundWidth + 2] = (3 << 8) | 2;
	ppu.background[23 * PPU466::BackgroundWidth + 3] = (1 << 8) | 19;
	ppu.background[23 * PPU466::BackgroundWidth + 4] = (1 << 8) | 20;
	ppu.background[24 * PPU466::BackgroundWidth + 3] = (1 << 8) | 21;
	ppu.background[24 * PPU466::BackgroundWidth + 4] = (1 << 8) | 22;

	ppu.background[23 * PPU466::BackgroundWidth + 28] = (3 << 8) | 3;
	ppu.background[24 * PPU466::BackgroundWidth + 28] = (3 << 8) | 4;
	ppu.background[23 * PPU466::BackgroundWidth + 29] = (2 << 8) | 19;
	ppu.background[23 * PPU466::BackgroundWidth + 30] = (2 << 8) | 20;
	ppu.background[24 * PPU466::BackgroundWidth + 29] = (2 << 8) | 21;
	ppu.background[24 * PPU466::BackgroundWidth + 30] = (2 << 8) | 22;

	ppu.background[25 * PPU466::BackgroundWidth + 2] = (4 << 8) | 23;
}

PlayMode::~PlayMode() {
}

void PlayMode::swap_color() {
	if (colors_swapped) {
		ppu.background[23 * PPU466::BackgroundWidth + 3] = (1 << 8) | 19;
		ppu.background[23 * PPU466::BackgroundWidth + 4] = (1 << 8) | 20;
		ppu.background[24 * PPU466::BackgroundWidth + 3] = (1 << 8) | 21;
		ppu.background[24 * PPU466::BackgroundWidth + 4] = (1 << 8) | 22;
		ppu.background[23 * PPU466::BackgroundWidth + 29] = (2 << 8) | 19;
		ppu.background[23 * PPU466::BackgroundWidth + 30] = (2 << 8) | 20;
		ppu.background[24 * PPU466::BackgroundWidth + 29] = (2 << 8) | 21;
		ppu.background[24 * PPU466::BackgroundWidth + 30] = (2 << 8) | 22;
		colors_swapped = false;
	} else {
		ppu.background[23 * PPU466::BackgroundWidth + 3] = (2 << 8) | 19;
		ppu.background[23 * PPU466::BackgroundWidth + 4] = (2 << 8) | 20;
		ppu.background[24 * PPU466::BackgroundWidth + 3] = (2 << 8) | 21;
		ppu.background[24 * PPU466::BackgroundWidth + 4] = (2 << 8) | 22;
		ppu.background[23 * PPU466::BackgroundWidth + 29] = (1 << 8) | 19;
		ppu.background[23 * PPU466::BackgroundWidth + 30] = (1 << 8) | 20;
		ppu.background[24 * PPU466::BackgroundWidth + 29] = (1 << 8) | 21;
		ppu.background[24 * PPU466::BackgroundWidth + 30] = (1 << 8) | 22;
		colors_swapped = true;
	}

	if (color == Color::RED) {
		color = Color::BLACK;
	} else if (color == Color::BLACK) {
		color = Color::RED;
	}
}

void PlayMode::switch_turn() {
	if (turn == Turn::PLAYER1) {
		ppu.background[25 * PPU466::BackgroundWidth + 2] = 0;
		ppu.background[25 * PPU466::BackgroundWidth + 28] = (4 << 8) | 23;
		turn = Turn::PLAYER2;
	} else if (turn == Turn::PLAYER2) {
		ppu.background[25 * PPU466::BackgroundWidth + 2] = (4 << 8) | 23;
		ppu.background[25 * PPU466::BackgroundWidth + 28] = 0;
		turn = Turn::PLAYER1;
	}
}

void PlayMode::fill_slot(int x, int y) {
	board[x][y] = color;
	uint16_t idx = 0;
	if (color == Color::RED) {
		idx = 1 << 8;
	} else if (color == Color::BLACK) {
		idx = 2 << 8;
	}
	ppu.background[2 * y * PPU466::BackgroundWidth + 10 + 2 * x] = idx | 17;
	ppu.background[2 * y * PPU466::BackgroundWidth + 10 + 2 * x + 1] = idx | 18;
	ppu.background[(2 * y + 1) * PPU466::BackgroundWidth + 10 + 2 * x] = idx | 15;
	ppu.background[(2 * y + 1) * PPU466::BackgroundWidth + 10 + 2 * x + 1] = idx | 16;

	if (// Diagonal 1
		(x <= 3 && y <= 2 && board[x+1][y+1] == color && board[x+2][y+2] == color && board[x+3][y+3] == color) 
		|| (x <= 4 && x >= 1 && y <= 3 && y >= 1 && board[x - 1][y - 1] == color && board[x + 1][y + 1] == color && board[x + 2][y + 2] == color)
		|| (x <= 5 && x >= 2 && y <= 4 && y >= 2 && board[x-2][y-2] == color && board[x-1][y-1] == color && board[x+1][y+1] == color)
		|| (x >= 3 && y >= 3 && board[x-3][y-3] == color && board[x - 2][y - 2] == color && board[x - 1][y - 1] == color)
		// Diagonal 2
		|| (x <= 3 && y >= 3 && board[x+1][y-1] == color && board[x+2][y-2] == color && board[x+3][y-3] == color)
		|| (x <= 4 && x >= 1 && y >= 2 && y <= 4 && board[x-1][y+1] == color && board[x+1][y-1] == color && board[x+2][y-2] == color)
		|| (x <= 5 && x >= 2 && y >= 1 && y <= 3 && board[x-2][y+2] == color && board[x-1][y+1] == color && board[x+1][y-1] == color)
		|| (x >= 3 && y <= 2 && board[x-3][y+3] == color && board[x-2][y+2] == color && board[x-1][y+1] == color)
		// Horizontal
		|| (x <= 3 && board[x+1][y] == color && board[x+2][y] == color && board[x+3][y] == color)
		|| (x <= 4 && x >= 1 && board[x-1][y] == color && board[x+1][y] == color && board[x+2][y] == color)
		|| (x <= 5 && x >= 2 && board[x-2][y] == color && board[x-1][y] == color && board[x+1][y] == color)
		|| (x >= 3 && board[x-3][y] == color && board[x-2][y] == color && board[x-1][y] == color)
		// Vertical
		|| (y >= 3 && board[x][y-3] == color && board[x][y-2] == color && board[x][y-1] == color)){
		game_over = true;
		if (turn == Turn::PLAYER1) {
			ppu.background[25 * PPU466::BackgroundWidth + 2] = (4 << 8) | 24;
		} else if (turn == Turn::PLAYER2) {
			ppu.background[25 * PPU466::BackgroundWidth + 28] = (4 << 8) | 24;
		}
	} else {
		switch_turn();
	}

	if (color == Color::RED) {
		color = Color::BLACK;
	} else if (color == Color::BLACK) {
		color = Color::RED;
	}
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	if (game_over) {
		return false;
	}
	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == SDLK_1) {
			if (board[0][5] != Color::NONE) {
				return false;
			}
			for (int i = 0; i < 6; i++) {
				if (board[0][i] == Color::NONE) {
					fill_slot(0, i);
					break;
				}
			}
			return true;
		} else if (evt.key.keysym.sym == SDLK_2) {
			if (board[1][5] != Color::NONE) {
				return false;
			}
			for (int i = 0; i < 6; i++) {
				if (board[1][i] == Color::NONE) {
					fill_slot(1, i);
					break;
				}
			}
			return true;
		} else if (evt.key.keysym.sym == SDLK_3) {
			if (board[2][5] != Color::NONE) {
				return false;
			}
			for (int i = 0; i < 6; i++) {
				if (board[2][i] == Color::NONE) {
					fill_slot(2, i);
					break;
				}
			}
			return true;
		} else if (evt.key.keysym.sym == SDLK_4) {
			if (board[3][5] != Color::NONE) {
				return false;
			}
			for (int i = 0; i < 6; i++) {
				if (board[3][i] == Color::NONE) {
					fill_slot(3, i);
					break;
				}
			}
			return true;
		} else if (evt.key.keysym.sym == SDLK_5) {
			if (board[4][5] != Color::NONE) {
				return false;
			}
			for (int i = 0; i < 6; i++) {
				if (board[4][i] == Color::NONE) {
					fill_slot(4, i);
					break;
				}
			}
			return true;
		} else if (evt.key.keysym.sym == SDLK_6) {
			if (board[5][5] != Color::NONE) {
				return false;
			}
			for (int i = 0; i < 6; i++) {
				if (board[5][i] == Color::NONE) {
					fill_slot(5, i);
					break;
				}
			}
			return true;
		} else if (evt.key.keysym.sym == SDLK_7) {
			if (board[6][5] != Color::NONE) {
				return false;
			}
			for (int i = 0; i < 6; i++) {
				if (board[6][i] == Color::NONE) {
					fill_slot(6, i);
					break;
				}
			}
			return true;
		}
	}

	return false;
}

void PlayMode::update(float elapsed) {
	if (game_over) {
		return;
	}
	time += elapsed;
	if (time >= 10.0f) {
		time -= 10.0f;
		swap_color();
	}
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	//--- set ppu state based on game state ---

	//--- actually draw ---
	ppu.draw(drawable_size);
}
