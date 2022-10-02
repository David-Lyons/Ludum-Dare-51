#include "asset_script.hpp"

void create_assets() {
	// Set up vectors to hold load_png results
	glm::uvec2 file_size = glm::uvec2(0, 0);
	std::vector< glm::u8vec4 > palette_data;

	// Set up vectors for storage
	std::vector< PPU466::Palette > palettes;
	std::vector< PPU466::Tile > tiles;

	/*
	* -------------------------------------
	* -------------Palettes----------------
	* -------------------------------------
	*/

	// BoardEmpty
	try {
		load_png(data_path("../assets/BoardEmpty.png"), &file_size, &palette_data, OriginLocation::UpperLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(file_size.x == 4 && file_size.y == 1);

	palettes.push_back({
		glm::u8vec4(0,0,0,0),
		glm::u8vec4(palette_data[1][0],palette_data[1][1],palette_data[1][2],255),
		glm::u8vec4(0,0,0,0),
		glm::u8vec4(palette_data[3][0],palette_data[3][1],palette_data[3][2],255),
	});

	// BoardRed
	try {
		load_png(data_path("../assets/BoardRed.png"), &file_size, &palette_data, OriginLocation::UpperLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(file_size.x == 4 && file_size.y == 1);

	palettes.push_back({
		glm::u8vec4(0,0,0,0),
		glm::u8vec4(palette_data[1][0],palette_data[1][1],palette_data[1][2],255),
		glm::u8vec4(palette_data[2][0],palette_data[2][1],palette_data[2][2],255),
		glm::u8vec4(palette_data[3][0],palette_data[3][1],palette_data[3][2],255),
	});

	// BoardBlack
	try {
		load_png(data_path("../assets/BoardBlack.png"), &file_size, &palette_data, OriginLocation::UpperLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(file_size.x == 4 && file_size.y == 1);

	palettes.push_back({
		glm::u8vec4(0,0,0,0),
		glm::u8vec4(palette_data[1][0],palette_data[1][1],palette_data[1][2],255),
		glm::u8vec4(palette_data[2][0],palette_data[2][1],palette_data[2][2],255),
		glm::u8vec4(palette_data[3][0],palette_data[3][1],palette_data[3][2],255),
	});

	// Numbers
	try {
		load_png(data_path("../assets/Numbers.png"), &file_size, &palette_data, OriginLocation::UpperLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(file_size.x == 4 && file_size.y == 1);

	palettes.push_back({
		glm::u8vec4(0,0,0,0),
		glm::u8vec4(palette_data[1][0],palette_data[1][1],palette_data[1][2],255),
		glm::u8vec4(palette_data[2][0],palette_data[2][1],palette_data[2][2],255),
		glm::u8vec4(palette_data[3][0],palette_data[3][1],palette_data[3][2],255),
		});

	// ArrowCrown
	try {
		load_png(data_path("../assets/ArrowCrown.png"), &file_size, &palette_data, OriginLocation::UpperLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(file_size.x == 4 && file_size.y == 1);

	palettes.push_back({
		glm::u8vec4(0,0,0,0),
		glm::u8vec4(palette_data[1][0],palette_data[1][1],palette_data[1][2],255),
		glm::u8vec4(palette_data[2][0],palette_data[2][1],palette_data[2][2],255),
		glm::u8vec4(palette_data[3][0],palette_data[3][1],palette_data[3][2],255),
		});

	/*
	* -------------------------------------
	* ---------------Tiles-----------------
	* -------------------------------------
	*/
	std::array<uint8_t, 8> bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	std::array<uint8_t, 8> bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };

	// Empty Tile
	tiles.push_back({ bit0, bit1 });

	// Numbers
	for (int i = 1; i < 8; i++) {
		bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
		bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
		try {
			load_png(data_path("../assets/" + std::to_string(i) + "Bottom.png"), &file_size, &palette_data, OriginLocation::LowerLeftOrigin);
		}
		catch (std::runtime_error& e) {
			std::cout << e.what();
		}
		assert(file_size.x == 8 && file_size.y == 8);

		for (uint32_t row = 0; row < file_size.y; row++) {
			for (uint32_t col = 0; col < file_size.x; col++) {
				glm::u8vec4 color = palette_data[row * file_size.x + col];
				if (color[0]) {
					bit0[row] |= (1 << col);
				}
				else if (color[1]) {
					bit1[row] |= (1 << col);
				}
				else if (color[2]) {
					bit0[row] |= (1 << col);
					bit1[row] |= (1 << col);
				}
			}
		}
		tiles.push_back({ bit0, bit1 });


		bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
		bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
		try {
			load_png(data_path("../assets/" + std::to_string(i) + "Top.png"), &file_size, &palette_data, OriginLocation::LowerLeftOrigin);
		}
		catch (std::runtime_error& e) {
			std::cout << e.what();
		}
		assert(file_size.x == 8 && file_size.y == 8);

		for (uint32_t row = 0; row < file_size.y; row++) {
			for (uint32_t col = 0; col < file_size.x; col++) {
				glm::u8vec4 color = palette_data[row * file_size.x + col];
				if (color[0]) {
					bit0[row] |= (1 << col);
				}
				else if (color[1]) {
					bit1[row] |= (1 << col);
				}
				else if (color[2]) {
					bit0[row] |= (1 << col);
					bit1[row] |= (1 << col);
				}
			}
		}
		tiles.push_back({ bit0, bit1 });
	}

	// Upper Left
	bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	try {
		load_png(data_path("../assets/UpperLeft.png"), &file_size, &palette_data, OriginLocation::LowerLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(file_size.x == 8 && file_size.y == 8);
	for (uint32_t row = 0; row < file_size.y; row++) {
		for (uint32_t col = 0; col < file_size.x; col++) {
			glm::u8vec4 color = palette_data[row * file_size.x + col];
			if (color[0]) {
				bit0[row] |= (1 << col);
			}
			else if (color[1]) {
				bit1[row] |= (1 << col);
			}
			else if (color[2]) {
				bit0[row] |= (1 << col);
				bit1[row] |= (1 << col);
			}
		}
	}
	tiles.push_back({ bit0, bit1 });

	// Upper Right
	bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	try {
		load_png(data_path("../assets/UpperRight.png"), &file_size, &palette_data, OriginLocation::LowerLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(file_size.x == 8 && file_size.y == 8);
	for (uint32_t row = 0; row < file_size.y; row++) {
		for (uint32_t col = 0; col < file_size.x; col++) {
			glm::u8vec4 color = palette_data[row * file_size.x + col];
			if (color[0]) {
				bit0[row] |= (1 << col);
			}
			else if (color[1]) {
				bit1[row] |= (1 << col);
			}
			else if (color[2]) {
				bit0[row] |= (1 << col);
				bit1[row] |= (1 << col);
			}
		}
	}
	tiles.push_back({ bit0, bit1 });

	// Lower Left
	bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	try {
		load_png(data_path("../assets/LowerLeft.png"), &file_size, &palette_data, OriginLocation::LowerLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(file_size.x == 8 && file_size.y == 8);
	for (uint32_t row = 0; row < file_size.y; row++) {
		for (uint32_t col = 0; col < file_size.x; col++) {
			glm::u8vec4 color = palette_data[row * file_size.x + col];
			if (color[0]) {
				bit0[row] |= (1 << col);
			}
			else if (color[1]) {
				bit1[row] |= (1 << col);
			}
			else if (color[2]) {
				bit0[row] |= (1 << col);
				bit1[row] |= (1 << col);
			}
		}
	}
	tiles.push_back({ bit0, bit1 });

	// Lower Right
	bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	try {
		load_png(data_path("../assets/LowerRight.png"), &file_size, &palette_data, OriginLocation::LowerLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(file_size.x == 8 && file_size.y == 8);
	for (uint32_t row = 0; row < file_size.y; row++) {
		for (uint32_t col = 0; col < file_size.x; col++) {
			glm::u8vec4 color = palette_data[row * file_size.x + col];
			if (color[0]) {
				bit0[row] |= (1 << col);
			}
			else if (color[1]) {
				bit1[row] |= (1 << col);
			}
			else if (color[2]) {
				bit0[row] |= (1 << col);
				bit1[row] |= (1 << col);
			}
		}
	}
	tiles.push_back({ bit0, bit1 });

	// Color indicators
	bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	try {
		load_png(data_path("../assets/TopLL.png"), &file_size, &palette_data, OriginLocation::LowerLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(file_size.x == 8 && file_size.y == 8);
	for (uint32_t row = 0; row < file_size.y; row++) {
		for (uint32_t col = 0; col < file_size.x; col++) {
			glm::u8vec4 color = palette_data[row * file_size.x + col];
			if (color[0]) {
				bit0[row] |= (1 << col);
			}
			else if (color[1]) {
				bit1[row] |= (1 << col);
			}
			else if (color[2]) {
				bit0[row] |= (1 << col);
				bit1[row] |= (1 << col);
			}
		}
	}
	tiles.push_back({ bit0, bit1 });

	bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	try {
		load_png(data_path("../assets/TopLR.png"), &file_size, &palette_data, OriginLocation::LowerLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(file_size.x == 8 && file_size.y == 8);
	for (uint32_t row = 0; row < file_size.y; row++) {
		for (uint32_t col = 0; col < file_size.x; col++) {
			glm::u8vec4 color = palette_data[row * file_size.x + col];
			if (color[0]) {
				bit0[row] |= (1 << col);
			}
			else if (color[1]) {
				bit1[row] |= (1 << col);
			}
			else if (color[2]) {
				bit0[row] |= (1 << col);
				bit1[row] |= (1 << col);
			}
		}
	}
	tiles.push_back({ bit0, bit1 });

	bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	try {
		load_png(data_path("../assets/TopUL.png"), &file_size, &palette_data, OriginLocation::LowerLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(file_size.x == 8 && file_size.y == 8);
	for (uint32_t row = 0; row < file_size.y; row++) {
		for (uint32_t col = 0; col < file_size.x; col++) {
			glm::u8vec4 color = palette_data[row * file_size.x + col];
			if (color[0]) {
				bit0[row] |= (1 << col);
			}
			else if (color[1]) {
				bit1[row] |= (1 << col);
			}
			else if (color[2]) {
				bit0[row] |= (1 << col);
				bit1[row] |= (1 << col);
			}
		}
	}
	tiles.push_back({ bit0, bit1 });

	bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	try {
		load_png(data_path("../assets/TopUR.png"), &file_size, &palette_data, OriginLocation::LowerLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(file_size.x == 8 && file_size.y == 8);
	for (uint32_t row = 0; row < file_size.y; row++) {
		for (uint32_t col = 0; col < file_size.x; col++) {
			glm::u8vec4 color = palette_data[row * file_size.x + col];
			if (color[0]) {
				bit0[row] |= (1 << col);
			}
			else if (color[1]) {
				bit1[row] |= (1 << col);
			}
			else if (color[2]) {
				bit0[row] |= (1 << col);
				bit1[row] |= (1 << col);
			}
		}
	}
	tiles.push_back({ bit0, bit1 });

	bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	try {
		load_png(data_path("../assets/Arrow.png"), &file_size, &palette_data, OriginLocation::LowerLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(file_size.x == 8 && file_size.y == 8);
	for (uint32_t row = 0; row < file_size.y; row++) {
		for (uint32_t col = 0; col < file_size.x; col++) {
			glm::u8vec4 color = palette_data[row * file_size.x + col];
			if (color[0]) {
				bit0[row] |= (1 << col);
			}
			else if (color[1]) {
				bit1[row] |= (1 << col);
			}
			else if (color[2]) {
				bit0[row] |= (1 << col);
				bit1[row] |= (1 << col);
			}
		}
	}
	tiles.push_back({ bit0, bit1 });

	bit0 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	bit1 = { 0, 0, 0, 0, 0, 0, 0, 0 };
	try {
		load_png(data_path("../assets/Crown.png"), &file_size, &palette_data, OriginLocation::LowerLeftOrigin);
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	assert(file_size.x == 8 && file_size.y == 8);
	for (uint32_t row = 0; row < file_size.y; row++) {
		for (uint32_t col = 0; col < file_size.x; col++) {
			glm::u8vec4 color = palette_data[row * file_size.x + col];
			if (color[0]) {
				bit0[row] |= (1 << col);
			}
			else if (color[1]) {
				bit1[row] |= (1 << col);
			}
			else if (color[2]) {
				bit0[row] |= (1 << col);
				bit1[row] |= (1 << col);
			}
		}
	}
	tiles.push_back({ bit0, bit1 });

	/*
	* -------------------------------------
	* ---------------Saving----------------
	* -------------------------------------
	*/

	assert(palettes.size() == 5);
	assert(tiles.size() == 25);

	// Learned how to create an ofstream from https://stackoverflow.com/questions/52803734/c-ostream-not-creating-a-new-file
	std::ofstream datafile(data_path("RuntimeAssets.bin"), std::ios::out);

	write_chunk("PALT", palettes, &datafile);
	write_chunk("TILE", tiles, &datafile);
}