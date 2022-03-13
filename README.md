# Game Of Life
This project is my C++ implementation of Conway's Game of Life, also known as Life. Game of Life was created by a British mathematician John Horton Conway in 1970. 
Game of Life is a zero-player game, and the progress of the game depends on its initial state that user can decide by clicking cells alive. The game is a two-dimensional grid
that consists of the same sized square cells. Every cell has eight neighbours and the game progresses in states, which are determined by the amount of alive neighbour cells.

I implemented this game as a gui -project by using Qt Creator. The project includes two classes: MainWindow and Cell. Most of the code is implemented in the MainWindow class and 
part of the gui -elements are created in a Qt desing mode. Cells are PushButtons and data about cell object and its neighbour cells is stored in a Node struct.

## Rules
- If the cell is dead, it will turn alive if it has exactly three neighbours. As if by reproduction.
- If the cell is alive, it will turn dead if it has 1 or no neighbours. As if by solitude.
- If the cell is alive, it will turn dead if it has 4 or more neighbours. As if by overpopulation.
- If the cell is alive and it has 2 or 3 neighbours, it will remain alive. 
## How to play
When the program starts, it creates a 15 x 15 board of initially dead grey cells. If the user wants to change the grid size, it can be done by choosing a new value between 15 and 40
from the spinbox. Game starts when user clicks a New Game button. After that the user can set the initial state of the game by clicking cells, when the clicked cells turn purple.
When the user is happy with the initial state, the user can choose to run the game either sequentially by pressing the Next button multiple times or just pressing Start button 
and the game starts to progress automatically. The user can also first use the Next button and switch to automatic mode, if desired. The game ends if all the cells turn back grey. 
The game can also stay running continuously until the user presses the Clear Button and ends the game. User can start a new game by pressing the New Game button. 
