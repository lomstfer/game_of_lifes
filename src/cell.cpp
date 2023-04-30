#include <cassert>
#include <stdint.h>

#include <cell.hpp>
#include <utils.hpp>
#include <state_machine.hpp>

int get_cell(std::vector<std::vector<Cell>> &world, Vector2I pos) {
    return world[pos.y][pos.x];
}

void set_cell(std::vector<std::vector<Cell>> &world, Vector2I pos, int type) {
    Cell prev_type = get_cell(world, pos);

    if (prev_type == TEAM_NONE && cells_placed < CELLS_PER_TURN) {
        world[pos.y][pos.x] = type;
        cells_placed++;
        return;
    }

    if (prev_type == type) {
        world[pos.y][pos.x] = TEAM_NONE;
        cells_placed--;
        return;
    }

    return;
}

void force_set_cell(std::vector<std::vector<Cell>> &world, Vector2I pos, int type) {
    world[pos.y][pos.x] = type;
    return;
}

int get_neighbours(std::vector<std::vector<Cell>> &world, Vector2I pos) {
    int blue_count = 0;
    int red_count = 0;
    //Cell type = get_cell(world, pos);
    
    if (pos.x < 0 || pos.y < 0 || pos.x > COLUMNS || pos.y > ROWS) {
        printf("\x1b[31mBUG: get_neighbors: out of range: %d, %d\x1b[m\n", pos.x, pos.y);
        return 0;
    }

    for (int x = pos.x - 1; x <= pos.x + 1; x++) {
		if (x < 0 || x >= COLUMNS)
			continue;

        for (int y = pos.y - 1; y <= pos.y + 1; y++) {
            if (y < 0 || y >= ROWS)
                continue;
            
            if (x == pos.x && y == pos.y)
                continue;
            
            int t = get_cell(world, {x, y});
            if (t == TEAM_BLUE)
                blue_count++;
            else if (t == TEAM_RED)
                red_count++;

        }
    }

    return count;
}

void tick_cell(std::vector<std::vector<Cell>> &world, std::vector<std::vector<Cell>> &world_copy, Vector2I pos) {
    int count = get_neighbours(world_copy, pos);

    switch (get_cell(world_copy, pos)) {
    case TEAM_NONE:
        if (count == 3)
            set_cell(world, pos, TEAM_BLUE);
        break;

    case TEAM_BLUE:
        if (count != 2 && count != 3) {
            set_cell(world, pos, TEAM_NONE);
        }
        break;
    }
    return;
}

int who_won(std::vector<std::vector<Cell>> &world) 
{
    int blue_count = 0;
    int red_count = 0;
    for (int x = 0; x < COLUMNS; x++) {
        for (int y = 0; y < ROWS; y++) {

            switch (world[x][y])
            {
            case TEAM_RED:
                red_count++;
                break;
            
            case TEAM_BLUE:
                blue_count++;
                break;
            }

        }   
    }
    if (blue_count > red_count) {
        return TEAM_BLUE;
    }
    else {
        return TEAM_RED;
    }
}