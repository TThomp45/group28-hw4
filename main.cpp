#include <iostream>
#include <cstdlib>
#include "bowling_frame.h"

using namespace std;

// Given helper: adds up frame_score() for every frame in one player's list
int current_game_total(Bowling_Frame * first_frame)
{
    Bowling_Frame * temp = first_frame;
    int game_total = 0;
    while(temp)
    {
        game_total += temp->frame_score();
        temp = temp->next_frame();
    }
    return game_total;
}

// TODO: print every frame's Score 1 / Score 2 for one player,
//       then "Total Game Score for player N: X"
void print_player(Bowling_Frame * first_frame, int player_num)
{
}

int main(int argc, char * argv[])
{
    // TODO 1: if no argument was given, print a usage message and return 1

    // TODO 2: turn argv[1] into an int and print "Player count is: N"

    // TODO 3: make an array of Bowling_Frame pointers, one per player

    // TODO 4: loop over players; for each one use "new" to build 10 frames,
    //         link next/prev both ways, store the first frame in the array

    // TODO 5: call update_scores() enough times for every player's game

    // TODO 6: print every player (print_player)

    return 0;
}
