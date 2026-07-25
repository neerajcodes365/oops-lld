#include <bits/stdc++.h>
using namespace std;

class Player {
private:
    int id;
    string name;
    char symbol;

public:
    Player(int id, string name, char symbol) {
        this->id = id;
        this->name = name;
        this->symbol = symbol;
    }
    int getPlayerId() { return this->id; }
    string getPlayerName() { return this->name; }
    char getSymbol() { return this->symbol; }
};

class Board {
private:
    int n;
    char emptySymbol;
    vector<vector<char>> grid;

public:
    Board(int n, char emptySymbol) {
        this->n = n;
        this->emptySymbol = emptySymbol;
        grid.assign(n, vector<char>(n, emptySymbol));
    }

    int getSize() { return n; }
    char getEmptySymbol() { return emptySymbol; }
    vector<vector<char>> getGrid() { return grid; }

    bool isCellEmpty(int row, int col) {
        return grid[row][col] == emptySymbol;
    }

    bool placeSymbol(int row, int col, char symbol) {
        if (!isCellEmpty(row, col)) return false;
        grid[row][col] = symbol;
        return true;
    }

    void display() {
        for (auto &row : grid) {
            for (auto &cell : row) cout << cell << " ";
            cout << endl;
        }
    }
};

class Rules {
public:
    // checks bounds + whether the target cell is empty
    bool isValidMove(Board *board, int row, int col) {
        int n = board->getSize();
        if (row < 0 || row >= n || col < 0 || col >= n) return false;
        return board->isCellEmpty(row, col);
    }

    bool isWinner(Board *board, char symbol) {
        vector<vector<char>> grid = board->getGrid();
        int n = board->getSize();

        // rows and columns
        for (int i = 0; i < n; i++) {
            bool rowWin = true, colWin = true;
            for (int j = 0; j < n; j++) {
                if (grid[i][j] != symbol) rowWin = false;
                if (grid[j][i] != symbol) colWin = false;
            }
            if (rowWin || colWin) return true;
        }

        // diagonals
        bool diag1 = true, diag2 = true;
        for (int i = 0; i < n; i++) {
            if (grid[i][i] != symbol) diag1 = false;
            if (grid[i][n - 1 - i] != symbol) diag2 = false;
        }
        return diag1 || diag2;
    }

    bool isDraw(Board *board) {
        vector<vector<char>> grid = board->getGrid();
        char empty = board->getEmptySymbol();
        for (auto &row : grid)
            for (auto &cell : row)
                if (cell == empty) return false;
        return true; // board full (caller should already have checked no winner)
    }
};

class Game {
private:
    Board *board;
    queue<Player *> players;
    Rules *rules;

public:
    Game(Board *board, queue<Player *> players, Rules *rules) {
        this->board = board;
        this->players = players;
        this->rules = rules;
    }

    void play() {
        while (!players.empty()) {
            Player *current = players.front();
            players.pop();

            board->display();
            cout << current->getPlayerName() << " (" << current->getSymbol()
                 << ") - enter row and col: ";

            int row, col;
            cin >> row >> col;

            if (!rules->isValidMove(board, row, col)) {
                cout << "Invalid move, try again.\n";
                players.push(current);
                continue;
            }

            board->placeSymbol(row, col, current->getSymbol());

            if (rules->isWinner(board, current->getSymbol())) {
                board->display();
                cout << current->getPlayerName() << " wins!\n";
                return;
            }

            if (rules->isDraw(board)) {
                board->display();
                cout << "It's a draw!\n";
                return;
            }

            players.push(current); // back of the queue for next turn
        }
    }
};

int main() {
    int size;
    cout << "input size" << endl;
    cin >> size;

    char emptyStateSymbol;
    cout << "input emptystatesymbol " << endl;
    cin >> emptyStateSymbol;

    Board *board = new Board(size, emptyStateSymbol);
    Rules *rules = new Rules();

    queue<Player *> players;
    players.push(new Player(1, "Player1", 'X'));
    players.push(new Player(2, "Player2", 'O'));

    Game *g = new Game(board, players, rules);
    g->play();

    return 0;
}