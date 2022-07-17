#include <iostream>
#include <random>
#include <array>
#include <cassert>

std::random_device rd;
std::seed_seq ss{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
std::mt19937 mt{ ss };
std::uniform_int_distribution boardSideRange{ 0, 2 };

using grid3 = std::array<std::array<char, 3>, 3>;

struct Coordinates {
    int x;
    int y;
};

void ignoreLine() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool checkBlocked(grid3& gameBoard, Coordinates& coords) {
    return gameBoard[coords.y][coords.x] != ' ';
}

bool checkRows(grid3& gameBoard, char symbol) {
    for (int iii = 0; iii < 3; ++iii) {
        if ((gameBoard[iii][0] == symbol) && (gameBoard[iii][1] == symbol) && (gameBoard[iii][2] == symbol)) {
            return true;
        }
    }
    return false;
}

bool checkColumns(grid3& gameBoard, char symbol) {
    for (int iii = 0; iii < 3; ++iii) {
        if ((gameBoard[0][iii] == symbol) && (gameBoard[1][iii] == symbol) && (gameBoard[2][iii] == symbol)) {
            return true;
        }
    }
    return false;
}

bool checkDiagonals(grid3& gameBoard, char symbol) {
    // Diagonal checks are hard coded, since it is not useful to check diagonals using a for-loop
    if ((gameBoard[0][0] == symbol) && (gameBoard[1][1] == symbol) && (gameBoard[2][2] == symbol) ||
        (gameBoard[0][2] == symbol) && (gameBoard[1][1] == symbol) && (gameBoard[2][0] == symbol)) {
        return true;
    }
    else {
        return false;
    }
}

bool checkWin(grid3& gameBoard, char symbol) {
    return (checkRows(gameBoard, symbol) || checkColumns(gameBoard, symbol) || checkDiagonals(gameBoard, symbol));
}

bool checkBoardFilled(grid3& gameBoard) {
    for (int iii = 0; iii < 3; ++iii) {
        for (int jjj = 0; jjj < 3; ++jjj) {
            if (gameBoard[iii][jjj] == ' ') {
                return false;
            }
        }
    }
    return true;
}

Coordinates getPlayerInput(grid3& gameBoard) {
    while (true) {
        std::cout << "Enter Coordinates from 0 to 2 (x, y): ";
        Coordinates playerCoords{};
        char ignore{};
        std::cin >> playerCoords.x >> ignore >> playerCoords.y;

        if (!std::cin) {
            std::cin.clear();
            ignoreLine();
            std::cerr << "Input is invalid. Please try again and write in the format of (x, y)." << '\n';
        }
        else if (playerCoords.x < 0 || playerCoords.x > 2 || playerCoords.y < 0 || playerCoords.y > 2) {
            std::cerr << "Input is out of board range. Please try again and input coordinates in the range of 0-2." << '\n';
        }
        else {
            if (!checkBlocked(gameBoard, playerCoords)) {
                return playerCoords;
            }
            else {
                std::cerr << "Space occupied. Please pick a different space." << '\n';
            }
        }
    }
}

Coordinates getOpponentInput(grid3& gameBoard) {
    while (true) {
        int x{ boardSideRange(mt) };
        int y{ boardSideRange(mt) };

        Coordinates opponentCoords{ x, y };

        if (!checkBlocked(gameBoard, opponentCoords)) {
            return opponentCoords;
        }
    }
}

void printGameboard(grid3& gameBoard) {
    for (int iii{ 0 }; iii < 3; ++iii) {
        for (int jjj{ 0 }; jjj < 3; ++jjj){
            std::cout << ' ' << gameBoard[iii][jjj] << ' ';
            
            if (jjj < 2) {
                std::cout << "||";
            }
        }
        
        if (iii < 2) {
            std::cout << "\n=============\n";
        }
    }
    std::cout << "\n\n";
}

void flipCell(grid3& gameBoard, Coordinates& position, char symbol) {
    gameBoard[position.y][position.x] = symbol;
}

void clearBoard(grid3& gameBoard) {
    for (int iii = 0; iii < 3; ++iii) {
        for (int jjj = 0; jjj < 3; ++jjj) {
            gameBoard[iii][jjj] = ' ';
        }
    }
}

void printWinMessage(char winChar) {
    switch (winChar) {
    case 'p':
        std::cout << "You got three in a row, you win!" << '\n';
        break;
    case 'o':
        std::cout << "Your opponent got three in a row, you lose!" << '\n';
        break;
    case 't':
        std::cout << "It's a tie! Nobody wins!" << '\n';
        break;
    }
}

bool runSingleGame(grid3& gameBoard) {
    clearBoard(gameBoard);
    char winChar{};

    while (true) {
        printGameboard(gameBoard);

        Coordinates playerCoords = getPlayerInput(gameBoard);
        flipCell(gameBoard, playerCoords, 'X');
        if (checkWin(gameBoard, 'X')) {
            winChar = 'p';
            break;
        }
        else if (checkBoardFilled(gameBoard)) {
            winChar = 't';
            break;
        }

        Coordinates opponentCoords = getOpponentInput(gameBoard);
        flipCell(gameBoard, opponentCoords, 'O');
        if (checkWin(gameBoard, 'O')) {
            winChar = 'o';
            break;
        }
        else if (checkBoardFilled(gameBoard)) {
            winChar = 't';
            break;
        }
    }

    assert(winChar && "Winner failed to be determined.");

    printGameboard(gameBoard);
    printWinMessage(winChar);
    
    std::cout << "\n======================\n\n";
        
    while (true) {
        std::cout << "Play again? (y/n): ";
        char isPlayingAgain{};
        std::cin >> isPlayingAgain;
        if (!std::cin) {
            std::cin.clear();
            ignoreLine();
            std::cerr << "Input is invalid. Please try again." << '\n';
        }
        else {
            switch (isPlayingAgain) {
            case 'y':
                return true;
            case 'n':
                return false;
            }
        }
    }
}

int main()
{
    grid3 gameBoard{};
    
    bool gameRunning{ true };
    while (gameRunning) {
        gameRunning = runSingleGame(gameBoard);
    }

    std::cout << "Thank you for playing!";
}
