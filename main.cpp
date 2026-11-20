#include <iostream>
#include <cstdlib>
#include "bowling_frame.h"

using namespace std;

// Adds up the scores from all the frames
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

// Prints every frame's scores for one player, then the player's total
void print_player(Bowling_Frame * first_frame, int player_num)
{
    Bowling_Frame * temp = first_frame;
    while(temp)
    {
        cout << "Frame " << temp->frame_num() << ":" << endl;
        cout << " Score 1: " << temp->score1() << endl;
        cout << " Score 2: " << temp->score2() << endl;

        // The last frame earns a third roll after a strike or a spare.
        // Its score is the three rolls added together, so the third roll is what is left over.
        if (temp->frame_num() == 10 && (temp->strike() || temp->spare()))
        {
            cout << " Score 3: " << temp->frame_score() - temp->score1() - temp->score2() << endl;
        }
        temp = temp->next_frame();
    }

    cout << endl;
    cout << "Total Game Score for player " << player_num << ": "
         << current_game_total(first_frame) << endl << endl;
}

int main(int argc, char * argv[])
{
    // The program needs the number of players
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <num_players>" << endl;
        return 1;
    }

    // Turn the argument into a number and make sure it is a sensible player count
    char * end;
    long player_count = strtol(argv[1], &end, 10);
    if (*argv[1] == '\0' || *end != '\0' || player_count < 1 || player_count > 100)
    {
        cout << "Please enter a valid number of players (1-100)." << endl;
        return 1;
    }
    cout << "Player count is: " << player_count << endl;

    // One pointer per player, each pointing at that player's first frame
    Bowling_Frame ** players = new Bowling_Frame * [player_count];

    for (int i = 0; i < player_count; i++)
    {
        // Build the 10 frames for this player and link them both ways
        players[i] = new Bowling_Frame(1);
        Bowling_Frame * previous = players[i];

        for (int j = 2; j <= 10; j++)
        {
            Bowling_Frame * current = new Bowling_Frame(j);
            previous->next_frame(current);
            current->prev_frame(previous);
            previous = current;
        }

        // Each call to update_scores() on the first frame makes one roll somewhere in the
        // game, and a game has at most 21 rolls, so calling it 21 times plays the whole game.
        for (int roll = 0; roll < 21; roll++)
        {
            players[i]->update_scores();
        }
    }

    for (int i = 0; i < player_count; i++)
    {
        print_player(players[i], i + 1);
    }

    delete [] players;
    return 0;
}
