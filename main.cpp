#include <iostream>
#include <vector>
#include <limits>
using namespace std;

const char HUMAN = 'X';
const char COMPUTER = 'O';
const char EMPTY = ' ';

class GameState {
private:
    vector<char> board; // 9 cells for Tic-Tac-Toe

public:
    GameState() : board(9, EMPTY) {}

    GameState(const vector<char>& b) : board(b) {}

    void printBoard() const {
        for (int i = 0; i < 9; ++i) {
            cout << (board[i] == EMPTY ? '-' : board[i]) << " ";
            if ((i + 1) % 3 == 0) cout << endl;
        }
    }

    bool isFull() const {
        for (char c : board)
            if (c == EMPTY) return false;
        return true;
    }

    bool isGameOver() const {
        return checkWinner() != EMPTY || isFull();
    }

    char checkWinner() const {
        const int winPatterns[8][3] = {
            {0,1,2}, {3,4,5}, {6,7,8},
            {0,3,6}, {1,4,7}, {2,5,8},
            {0,4,8}, {2,4,6}
        };

        for (auto& pattern : winPatterns) {
            if (board[pattern[0]] != EMPTY &&
                board[pattern[0]] == board[pattern[1]] &&
                board[pattern[1]] == board[pattern[2]]) {
                return board[pattern[0]];
            }
        }
        return EMPTY;
    }

    //iterates through board vector, adds empty indexes to moves vector, returns moves
    vector<int> getAvailableMoves() const {
        vector<int> moves;
        for (int i = 0; i < 9; ++i)
            if (board[i] == EMPTY) moves.push_back(i);
        return moves;
    }

    GameState makeMove(int index, char player) const {
        vector<char> newBoard = board; //sets newBoard to current game board
        newBoard[index] = player; //sets character at index value to 'X' or 'O' depending on player
        return GameState(newBoard); //returns updated game board
    }

    const vector<char>& getBoard() const {
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
            for (int move : state.getAvailableMoves()) { //loops through moves vector (contains empty indexes)
                GameState newState = state.makeMove(move, COMPUTER); //creates a new game state with COMPUTER move added
                int score = minimax(newState, false); //calls minimax function again (but switches to HUMAN's turn)
                bestScore = max(bestScore, score); //update bestScore if a higher score is found
            }
            return bestScore;  //return the best score found for COMPUTER
        } else { //if bool = false, HUMAN's turn (minimizing score)
            int bestScore = numeric_limits<int>::max(); //sets bestScore to highest possible int value
            for (int move : state.getAvailableMoves()) {
                GameState newState = state.makeMove(move, HUMAN); //creates a new game state with HUMAN move added
                int score = minimax(newState, true); //calls minimax function again (but switches to COMPUTER's turn)
                bestScore = min(bestScore, score); //update bestScore if a lower score is found
            }
            return bestScore; //return the best score found for HUMAN
        }
    }

    int findBestMove(const GameState& state) { //function predicts best next move based on score
        int bestScore = numeric_limits<int>::min();
        int bestMove = -1;

        for (int move : state.getAvailableMoves()) { //loops through every empty index (every available move)
            GameState newState = state.makeMove(move, COMPUTER); //makes a new game state for current COMPUTER move
            int score = minimax(newState, false); //calculates overall game score if current move is made
            if (score > bestScore) { //if move results in higher overall score (for COMPUTER) than current best
                bestScore = score; //sets current best to current score (basically updates best move's value)
                bestMove = move; //sets current best move to current index value (updates computer's next move)
            }
        }
        return bestMove; //returns the move with the highest overall score for COMPUTER
    }
};

void playGame() {
    GameState state;
    TicTacToeTree ai;
    char currentPlayer = HUMAN;

    while (!state.isGameOver()) {
        state.printBoard();
        if (currentPlayer == HUMAN) {
            int move;
            cout << "Enter your move (0-8): ";
            cin >> move;
            if (move >= 0 && move < 9 && state.getBoard()[move] == EMPTY) {
                state = state.makeMove(move, HUMAN);
                currentPlayer = COMPUTER;
            } else {
                cout << "Invalid move. Try again.\n";
            }
        } else {
            int move = ai.findBestMove(state);
            state = state.makeMove(move, COMPUTER);
            cout << "Computer plays at position " << move << endl;
            currentPlayer = HUMAN;
        }
    }

    state.printBoard();
    char winner = state.checkWinner();
    if (winner == COMPUTER) cout << "Computer wins!\n";
    else if (winner == HUMAN) cout << "You win!\n";
    else cout << "It's a draw!\n";
}

int main() {
    playGame();
    return 0;
}
