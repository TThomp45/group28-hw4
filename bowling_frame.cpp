#include "bowling_frame.h"
#include <random>

std::random_device dev;
std::mt19937 rng(dev());

Bowling_Frame::Bowling_Frame(int fnum)
{
	score1_ = 0;
	score2_ = 0;
	strike_ = false;
	spare_ = false;
	frame_num_ = fnum;
	frame_score_ = 0;
	roll_one_ = false;
	roll_two_ = false;

	// The last frame keeps its third roll in total_score_ (-1 means not thrown yet).
	total_score_ = -1;

	next_frame_ = NULL;
	prev_frame_ = NULL;

}

Bowling_Frame::~Bowling_Frame()
{
	free(next_frame_);
	free(prev_frame_);
}

Bowling_Frame* Bowling_Frame::next_frame() { return next_frame_; }
void Bowling_Frame::next_frame( Bowling_Frame * new_frame ) { next_frame_ = new_frame; }

Bowling_Frame* Bowling_Frame::prev_frame() { return prev_frame_; }
void Bowling_Frame::prev_frame( Bowling_Frame * new_frame ) { prev_frame_ = new_frame; }

int Bowling_Frame::score1()				{ return score1_; }
void Bowling_Frame::score1( int score )	{ score1_ = score; }

int Bowling_Frame::score2()				{ return score2_; }
void Bowling_Frame::score2( int score )	{ score2_ = score; }

bool Bowling_Frame::strike() 			{ return strike_; }
void Bowling_Frame::strike(bool value )	{ strike_ = value; }

bool Bowling_Frame::spare() 			{ return spare_; }
void Bowling_Frame::spare(bool value )	{ spare_ = value; }

int Bowling_Frame::frame_num()
{
	return frame_num_;
}

int Bowling_Frame::frame_score()
{
	return frame_score_; 
}

void Bowling_Frame::update_scores()
{
	std::uniform_int_distribution<std::mt19937::result_type> dist10(0, 10);

	// Rolls: take the next roll for this frame, or pass the turn on if it is finished.
	if (!roll_one_)
	{
		score1_ = dist10(rng);
		roll_one_ = true;

		if(score1_ == 10) strike_ = true; 
	}
	else if ( (!strike_ || !next_frame_) && !roll_two_ ) 	
	{
		int pins_standing = 10 - score1_;
		if (strike_) pins_standing = 10;

		std::uniform_int_distribution<std::mt19937::result_type> distX(0, pins_standing);
		score2_ = distX(rng); 
		roll_two_ = true;

		if (!strike_ && (score1_ + score2_) == 10) spare_ = true;
	}
	else if (!next_frame_ && (strike_ || spare_) && total_score_ == -1)
	{
		int pins_standing = 10;
		if (strike_ && score2_ != 10) pins_standing = 10 - score2_;

		std::uniform_int_distribution<std::mt19937::result_type> distY(0, pins_standing);
		total_score_ = distY(rng);
	}
	else if (next_frame_) 	next_frame_->update_scores();

	// Score: this frame's pins plus its bonus. The last frame adds its third roll,
	// a spare adds the next roll, and a strike adds the next two rolls.
	frame_score_ = score1_ + score2_;

	if (!next_frame_)
	{
		if (total_score_ != -1) frame_score_ = frame_score_ + total_score_;
	}
	else if (spare_)
	{
		frame_score_ = frame_score_ + next_frame_->score1();
	}
	else if (strike_)
	{
		int next_roll = next_frame_->score1();
		int roll_after;

		if (next_frame_->strike() && next_frame_->next_frame())
		{
			roll_after = next_frame_->next_frame()->score1();
		}
		else
		{
			roll_after = next_frame_->score2();
		}

		frame_score_ = frame_score_ + next_roll + roll_after;
	}
}
