/*
* @file botTemplate.cpp
* @author Arun Tejasvi Chaganty <arunchaganty@gmail.com>
* @date 2010-02-04
* Template for users to create their own bots
*/

#include "Othello.h"
#include "OthelloBoard.h"
#include "OthelloPlayer.h"
#include "OthelloGame.h"
#include <cstdlib>
#include <climits>
using namespace std;
using namespace Desdemona;

OthelloBoard globalBoard;
Turn myturn;
class MyBot: public OthelloPlayer
{
    public:
        /**
         * Initialisation routines here
         * This could do anything from open up a cache of "best moves" to
         * spawning a background processing thread. 
         */
        MyBot( Turn turn );

        /**
         * Play something 
         */
        virtual Move play( const OthelloBoard& board );
		virtual double minimax(OthelloBoard auxBoard, int depth, bool maximizing, Turn turn, double alpha, double beta);
		virtual double potentialMobility(OthelloBoard &auxBoard, Turn curr, Turn checkTurn);
    private:
};

MyBot::MyBot( Turn turn )
    : OthelloPlayer( turn )
{
}

double MyBot::potentialMobility(OthelloBoard &auxBoard, Turn curr, Turn checkTurn)
{
		int numberOfOp = 0;
		int numberOfedges = 0;
	if((this->turn == curr))
	{
		for(int x = 0; x < 8; x++)
		{
			for(int y = 0; y < 8; y++)
			{
				if( auxBoard.get(x, y) == other( turn ) )
				{
					if( x == 0 || x == 7 )
					{
						numberOfedges++;
					}
					if( y == 0 || y == 7 )
					{
						numberOfedges++;
					}
					if( (x > 0 && x < 8) && (y > 0 && y < 8))
					{
						int y1 = y-1;
						for( int x1 = x-1; x1 <= x + 1; x1 = x1 + 2, y1 = y1+2)
							{
								if(auxBoard.get(x1, y1) == EMPTY)
									numberOfOp++;
							} 
						int x1 = x;
						for( int y1 = y-1; y1 <= y + 1; y1 = y1 + 2, x1 = x1+2)
							{
								if(auxBoard.get(x1, y1) == EMPTY)
									numberOfOp++;
							} 
					}
				}
			}
		}
		
	}
	else
	{
		for(int x = 0; x < 8; x++)
		{
			for(int y = 0; y < 8; y++)
			{
				if( auxBoard.get(x, y) ==  this->turn  )
				{
					if( x == 0 || x == 7 )
					{
						numberOfedges++;
					}
					else if( y == 0 || y == 7 )
					{
						numberOfedges++;
					}
					if( (x > 0 && x < 8) && (y > 0 && y < 8))
					{
						int y1 = y-1;
						for( int x1 = x-1; x1 <= x + 1; x1 = x1 + 2, y1 = y1+2)
							{
								if(auxBoard.get(x1, y1) == EMPTY)
									numberOfOp++;
							} 
						int x1 = x;
						for( int y1 = y-1; y1 <= y + 1; y1 = y1 + 2, x1 = x1+2)
							{
								if(auxBoard.get(x1, y1) == EMPTY)
									numberOfOp++;
							} 
					}
				}
			}
		}
	}
	return numberOfOp - 1.2 *numberOfedges;
}

double comparePotential( OthelloBoard auxBoard)
{
	
		int numberOfOp = 0;
		int numberOfedges = 0;
	for(int x = 0; x < 8; x++)
	{
		for(int y = 0; y < 8; y++)
		{
			if( auxBoard.get(x, y) == other( myturn ) )
			{
				if( x == 0 || x == 7 )
				{
					numberOfedges++;
				}
				if( y == 0 || y == 7 )
				{
					numberOfedges++;
				}
				if( (x > 0 && x < 8) && (y > 0 && y < 8))
				{
					int y1 = y-1;
					for( int x1 = x-1; x1 <= x + 1; x1 = x1 + 2, y1 = y1+2)
					{
						if(auxBoard.get(x1, y1) == EMPTY)
							numberOfOp++;
					} 
					int x1 = x;
					for( int y1 = y-1; y1 <= y + 1; y1 = y1 + 2, x1 = x1+2)
					{
						if(auxBoard.get(x1, y1) == EMPTY)
							numberOfOp++;
					} 
			}
		}
	}
}
	return numberOfOp - 1.2 * numberOfedges;
}

bool compare(Move m1, Move m2)
{
	OthelloBoard auxBoard1 = OthelloBoard( globalBoard );
	OthelloBoard auxBoard2 = OthelloBoard( globalBoard );
	auxBoard1.makeMove(myturn, m1);
	auxBoard2.makeMove(myturn, m2);
	double p1 = comparePotential( auxBoard1);
	double p2 = comparePotential( auxBoard2);

	return (p1 > p2);
	
}

double MyBot::minimax(OthelloBoard auxBoard, int depth, bool maximizing, Turn turn, double alpha, double beta)
{
	
	if( maximizing )
	{	
		double bestMoveScore = -64;
		if( depth == 5 )
		{
			double hf = potentialMobility(auxBoard, turn, this->turn);
			return hf;
		}
		OthelloBoard localBoard = OthelloBoard( auxBoard );
		list<Move> opMoves = localBoard.getValidMoves( turn );
		for( list<Move>::iterator itr = opMoves.begin(); itr != opMoves.end(); itr++)
		{
			localBoard = OthelloBoard( auxBoard );
			localBoard.makeMove( turn,*itr );
			double moveScore = minimax( localBoard, depth+1, false, other( turn ), alpha, beta);
			bestMoveScore = max(moveScore, bestMoveScore);
			alpha = max(bestMoveScore, alpha);
			
			if( beta <= alpha )
			{
				return bestMoveScore;
			}
		}
		return bestMoveScore;
	}
	else
	{
		double bestMoveScore = 64;
		if( depth == 5 )
		{
			double hf = potentialMobility(auxBoard, turn, this->turn);
			return hf;
		}
		OthelloBoard localBoard = OthelloBoard( auxBoard );
		list<Move> opMoves = localBoard.getValidMoves( turn );
		for( list<Move>::iterator itr = opMoves.begin(); itr != opMoves.end(); itr++)
		{
			localBoard = OthelloBoard( auxBoard );
			localBoard.makeMove( turn, *itr );
			double moveScore = minimax( localBoard, depth+1, true, other( turn ), alpha, beta);
			bestMoveScore = min(moveScore, bestMoveScore);
			beta = min(beta, bestMoveScore);
			if( beta <= alpha )
			{
				return bestMoveScore;
			}
		}
		return bestMoveScore;
	}
}

Move MyBot::play( const OthelloBoard& board )
{
	globalBoard = board;
	myturn = this->turn;
    list<Move> moves = board.getValidMoves( turn );
	moves.sort(compare);
    list<Move>::iterator it = moves.begin();
	int countOfBlack = 0;
	double bestScore = (double)INT_MIN;
	list<Move>::iterator bestMove = moves.begin();
	for(list<Move>::iterator itr = moves.begin() ; itr != moves.end(); itr++)
	{
		if(( (*itr).x == 0 && (*itr).y == 0) || ((*itr).x == 0 && (*itr).y == 7) || ((*itr).x == 7 && (*itr).y == 0) || ((*itr).x == 7 && (*itr).y == 7))
			{
				return *itr;
			}
		OthelloBoard auxBoard = OthelloBoard( board );
		auxBoard.makeMove( this->turn, *itr );
		double score;
		if( (auxBoard.getValidMoves( other( this->turn ))).size() > 0)
			score = minimax(auxBoard, 0, false, other(turn), -64, 64 );
		else
			continue;
		if( bestScore < score )
		{
			bestScore = score;
			bestMove = itr;
		}
		
	}
    return *bestMove;
}

// The following lines are _very_ important to create a bot module for Desdemona

extern "C" {
    OthelloPlayer* createBot( Turn turn )
    {
        return new MyBot( turn );
    }

    void destroyBot( OthelloPlayer* bot )
    {
        delete bot;
    }
}


