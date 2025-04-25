#include <iostream>
#include <vector>
#include <limits>
#include <random>
using namespace std;

const char HUMAN = 'X'; //represents HUMAN move
const char COMPUTER = 'O'; //represents COMPUTER move
const char EMPTY = ' '; //represents avaiable move/empty cell

class GameState {
private:
    vector<char> board; // 9 cells for Tic-Tac-Toe

public:
    GameState() : board(9, EMPTY) {} //default board

    GameState(const vector<char>& b) : board(b) {} //sets board with provided state

    void printBoard() const { //outputs board
        for (int i = 0; i < 9; ++i) { //iterates through board
            cout << (board[i] == EMPTY ? '-' : board[i]) << " "; //prints 'X' or 'O' if move is present, otherwise prints '-'
            if ((i + 1) % 3 == 0) cout << endl; //starts new line after every third character
        }
    }

    bool isFull() const { //checks if there are no more avaiable moves to make
        for (char c : board) //iterates through every character in board vector
            if (c == EMPTY) return false; //if there is an available move, returns false
        return true; //if there are no available moves, returns true
    }

    bool isGameOver() const { //checks if game is done
        return checkWinner() != EMPTY || isFull(); //returns true if there is a winner or board is full
    }

    char checkWinner() const { //checks if a player has a winning pattern
        const int winPatterns[8][3] = { //makes a 2D array for all possible win combinations
            {0,1,2}, {3,4,5}, {6,7,8}, //rows
            {0,3,6}, {1,4,7}, {2,5,8}, //columns
            {0,4,8}, {2,4,6} //diagonals
        };

        for (auto& pattern : winPatterns) { //iterates through each possible win combination
            if (board[pattern[0]] != EMPTY && //if first character is not empty
                board[pattern[0]] == board[pattern[1]] && //if first character matches second character
                board[pattern[1]] == board[pattern[2]]) { //if second character matches third character
                return board[pattern[0]]; //return 'X' or 'O' depending on winner
            }
        }
        return EMPTY; //return ' ' if no winner
    }

    vector<int> getAvailableMoves() const { //returns available cells
        vector<int> moves; //makes vector to hold empty cell indexes
        for (int i = 0; i < 9; ++i) //iterates through all cells
            if (board[i] == EMPTY) moves.push_back(i); //if cells is empty, adds it to moves vector
        return moves; //returns all empty cells
    }

    GameState makeMove(int index, char player) const { //updates game board with player move
        vector<char> newBoard = board; //sets newBoard to current game board
        newBoard[index] = player; //sets character at index value to 'X' or 'O' depending on player
        return GameState(newBoard); //returns updated game board
    }

    const vector<char>& getBoard() const { //accessor for game board
        return board;
    }
};

class TicTacToeTree {
public:
    int minimax(const GameState& state, bool isMaximizing) { //Boolean is used to check if computer or human makes move
        char winner = state.checkWinner(); //First, checks if game is over using char winner
        if (winner == COMPUTER) return 1;
        if (winner == HUMAN) return -1;
        if (state.isFull()) return 0;

        if (isMaximizing) { //if bool = true, COMPUTER's turn (maximizing score)
            int bestScore = numeric_limits<int>::min(); //setting bestScore to lowest possible int value
            for (int move : state.getAvailableMoves()) { //loops through moves vector (contains all available moves)
                GameState newState = state.makeMove(move, COMPUTER); //creates a seperate game state for each COMPUTER move
                int score = minimax(newState, false); //recursively simulates through each game state (assumes HUMAN picks best options)
                bestScore = max(bestScore, score); //if a certain move results in a higher score (better for COMPUTER), updates bestScore
            }
            return bestScore;  //return the highest possible score found
        } else { //if bool = false, HUMAN's turn (minimizing score)
            int bestScore = numeric_limits<int>::max(); //sets bestScore to highest possible int value
            for (int move : state.getAvailableMoves()) { //loops through moves vector (contains all available moves)
                GameState newState = state.makeMove(move, HUMAN); //creates a seperate game state for each HUMAN move
                int score = minimax(newState, true); //rescursively simulates through each game state (assums COMPUTER picks best options)
                bestScore = min(bestScore, score); //if a certain move results in a lower score (better for HUMAN), updates bestScore
            }
            return bestScore; //return the lowest possible score found
        }
    }

    int findBestMove(const GameState& state) { //function predicts best next move based current board state (only for COMPUTER)
        int bestScore = numeric_limits<int>::min();
        int bestMove = -1;

        for (int move : state.getAvailableMoves()) { //loops through every empty index (every available move)
            GameState newState = state.makeMove(move, COMPUTER); //makes a new game state for current COMPUTER move
            int score = minimax(newState, false); //calculates final game score if current move is made
            if (score > bestScore) { //if move results in higher overall score (for COMPUTER) than current best
                bestScore = score; //sets current best to current score (basically updates best move's value)
                bestMove = move; //sets current best move to current index value (updates computer's next move)
            }
            else if (score == bestScore){ //ADDED: RANDOMNESS (randomly choose between two equal moves)
                if (rand() % 2 == 0){ //50 50 chance
                    bestScore = score;
                    bestMove = move;
                }
            }
        }
        return bestMove; //returns the move with the highest overall score for COMPUTER
    }
};

void playGame() {
    GameState state; //creates an intial board
    TicTacToeTree ai; //creates COMPUTER player with intergrated logic
    char currentPlayer = HUMAN; //creates HUMAN player

    while (!state.isGameOver()) { //continues looping while game isn't finished
        state.printBoard(); //outputs board to screen
        if (currentPlayer == HUMAN) { //if it's HUMAN's turn
            int move; //variable to store move choice
            cout << "Enter your move (0-8): ";
            cin >> move; //updates move to HUMAN choice
            if (move >= 0 && move < 9 && state.getBoard()[move] == EMPTY) { //checks move is valid to make
                state = state.makeMove(move, HUMAN); //updates board with move
                currentPlayer = COMPUTER; //switches to COMPUTER's turn
            } else {
                cout << "Invalid move. Try again.\n"; //if move is invalid, prompts user to enter another one
            }
        } else {
            int move = ai.findBestMove(state); //selects move that leads to highest possible score
            state = state.makeMove(move, COMPUTER); //updates board with move
            cout << "Computer plays at position " << move << endl; //lets user know where the move was made
            currentPlayer = HUMAN; //switches to HUMAN's turn
        }
    }

    state.printBoard(); //prints final board
    char winner = state.checkWinner(); //checks for winner == 'X' or winner == 'O'
    if (winner == COMPUTER) cout << "Computer wins!\n"; //lets user know who the winner is
    else if (winner == HUMAN) cout << "You win!\n";
    else cout << "It's a draw!\n"; //if no winner is present, game is a draw
}

int main() {
    playGame();
    return 0;
}
