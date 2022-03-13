#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cell.h"
#include <iostream>
#include <QTimer>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer(this))
{
    ui->setupUi(this);
    ui->boardDimension->setMinimum(15); // Minimum size of the N x N gameboard
    ui->boardDimension->setMaximum(40);  // Maximum size of the N x N gameboard
    MainWindow::changePushButtonsState(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(game()));
    MainWindow::on_createBoard_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;

    for (Node* node : allCells ) {
        delete node;
    }
    for (Node* node : allCells) {
        delete node->cell;
    }
}

void MainWindow::createBoard() {
// Creates a N x N board with pushButtons that represent cells.

    ui->gridLayout->setSpacing(0); // Setting no space between cells

    // Initializes a two-dimensional vector of Nodes(struct)
    std::vector<std::vector<Node*>> nodes;

    createNodes(nodes);
    linkNodes(nodes);
    populateNodes(nodes);

    // Copying Nodes from the two-dimensional vector to an unordered set.
    for (int i=0; i < gridSize; ++i) {
        for (int j=0; j < gridSize; ++j) {
            allCells.insert(nodes[i][j]);
         }
     }

    nodes.clear(); // Clearing the vector
}

void MainWindow::createNodes(std::vector<std::vector<Node*>> &nodes) {
// Iterates through the gamegrid and creates a new Node struct for every cell.

    int column = 0;

    std::vector<Node*> rowofNodes;

    for (int i=0; i < cellAmount; ++i) {
        rowofNodes.push_back(new Node); // Adding each cell of one row to a vector
        column += 1;
        if (column == gridSize) {
            // Adding one cell row at a time to the function's parameter vector
            nodes.push_back(rowofNodes);
            rowofNodes.clear();
            column = 0;
        }
    }
}

void MainWindow::linkNodes(std::vector<std::vector<Node*>> &nodes) {
// Iterates through the gamegrid and links every cell to its neighbour cells.
// Every cell has eight neighbours.

    int row = 0;
    int column = 0;

    for (int i=0; i < cellAmount; ++i) {

        Node* node = nodes[row][column];
        int topIndex = row - 1;
        int bottomIndex = row + 1;
        int leftIndex = column - 1;
        int rightIndex = column + 1;

        if(topIndex >= 0) {  // top neighbour cell
            node->neighbourCells.insert(nodes[topIndex][column]);
        } if (bottomIndex < gridSize) { // bottom neighbour
            node->neighbourCells.insert(nodes[bottomIndex][column]);
        } if (leftIndex >= 0) { // left neighbour
            node->neighbourCells.insert(nodes[row][leftIndex]);
        } if (rightIndex < gridSize) { // right neighbour
            node->neighbourCells.insert(nodes[row][rightIndex]);
        } if (topIndex >= 0 && leftIndex >= 0) { // top left corner neighbour
            node->neighbourCells.insert(nodes[topIndex][leftIndex]);
        } if (bottomIndex < gridSize && leftIndex >= 0) { // bottom left corner
            node->neighbourCells.insert(nodes[bottomIndex][leftIndex]);
        } if (bottomIndex < gridSize && rightIndex < gridSize) { // bottom right corner
            node->neighbourCells.insert(nodes[bottomIndex][rightIndex]);
        } if (topIndex >= 0 && rightIndex < gridSize) { // top right corner
            node->neighbourCells.insert(nodes[topIndex][rightIndex]);
        }
        column += 1;

        if (column == gridSize) {
            row += 1;
            column = 0;
        }
    }
}

void MainWindow::populateNodes(std::vector<std::vector<Node*>> &nodes) {
// Creates a new Cell for every Node. Cells are pushbuttons
// which are initially dead (grey).

    int row = 0;
    int column = 0;

    for (int i=0; i < cellAmount; ++i) {
        Cell *cell = new Cell(this); // Creating a new cell object
        nodes[row][column]->cell = cell;
        cell->setCellState(false); // Setting the cell as "dead"
        cell->setStyleSheet("background-color: silver");
        // Creating a new connection when cell is clicked
        connect(cell, &QPushButton::clicked, this,
                      [=]() { setInitialState(cell); });

        // Setting the right cell size and adding it to the gamegrid
        cell->setFixedWidth(CELL_SIZE);
        cell->setFixedHeight(CELL_SIZE);
        ui->gridLayout->addWidget(cell, row, column);
        column += 1;

        if (column == gridSize) {
            row += 1;
            column = 0;
        }
    }
}

void MainWindow::game() {
// Actual game implementation

    int neighbourCells = 0;

    // Iterates through the set that contains data of all the cells.
    for (Node* node : allCells) {

        // If the cell is dead
        if (node->cell->getCellState() == false) {
           neighbourCells = checkNeighbours(node);
            if (neighbourCells == 3) {
                // Cell turns alive as if by regrowth
                awakeningCells.push_back(node); // Adding cell to a vector
           }
        }
         // If the cell is alive
        else if (node->cell->getCellState() == true) {
            neighbourCells = checkNeighbours(node);
            if ((neighbourCells == 0) || (neighbourCells == 1)) {
                // Cell dies as if by solitude
                dyingCells.push_back(node); // Adding cell to a vector
            } else if (neighbourCells >= 4) {
                // Cells dies as if by overpopulation
                dyingCells.push_back(node);
            } else if ((neighbourCells == 2) || (neighbourCells == 3)) {
                // Cell remains alive
                awakeningCells.push_back(node);
            }
        }
    }

    if (awakeningCells.size() == 0 && dyingCells.size() == 0) {
    // If none of the cells is going to turn alive or dead (= game is over)
        round += 1;
        on_ClearButton_clicked();
    } else {
         setNewState(); // Setting cells to a new state after each "round"
    }
}

void MainWindow::setNewState() {
// Iterates through two vectors: first vector contains data of the cells
// that will turn dead and other vector has data of the cells that will turn alive.

    for(Node* node : dyingCells) {  // Cells turn dead
        node->cell->setStyleSheet("background-color: silver");
        node->cell->setCellState(false);
    }
    dyingCells.clear();

    for (Node* node : awakeningCells) {  // Cells turn alive
        node->cell->setStyleSheet("background-color: purple");
        node->cell->setCellState(true);
    }
    awakeningCells.clear();
}

int MainWindow::checkNeighbours(Node* node) {
// Takes one Node as a parameter and calculates how many of its
// neighbours is alive and returns the value.

    int neighbourAmount = 0;

    for (Node* n : node->neighbourCells){
        if (n->cell->getCellState() == true )
        neighbourAmount += 1;
    }

    return neighbourAmount;
}

void MainWindow::calculateCells() {
// Calculates the total amount of the cells in the gameboard

    cellAmount = gridSize * gridSize;
}

void MainWindow::setInitialState(Cell* c) {
// Creates the initial state of the game. Cells turn purple (=alive)
// when user clicks them.

    c->setStyleSheet("background-color: purple");
    c->setCellState(true);
    ui->boardDimension->setDisabled(true);
    ui->createBoard->setDisabled(true);
    changePushButtonsState(false);
}

void MainWindow::disableCells() {
// Sets all pushButtons (cells) disabled

    for (Node* node : allCells) {
        node->cell->setDisabled(true);
    }
}

void MainWindow::on_StartButton_clicked() {
// When Start button is clicked, game function is called in every
// 0.5 seconds and the game goes on until the Clear button is clicked.

    if (gameState == false) {
        return;
    }  else {
        ui->createBoard->setDisabled(true);
        ui->boardDimension->setDisabled(true);
        ui->StartButton->setDisabled(true);
        ui->NextButton->setDisabled(true);
        disableCells(); // Disables all cell pushbuttons when game is on

        timer->start(500); // Starts calling the game function with 0.5 sec delay
    }
}

void MainWindow::on_ClearButton_clicked() {
// Clears the gameboard and stops the game (only if the game is on).

    if (gameState == false) {
        return;
    } else {
        timer->stop(); // Stops the QTimer and the game loop stops running

        for (Node* node : allCells) { // Changes all the cells back to grey.
            if (node->cell->getCellState() == true){
                node->cell->setStyleSheet("background-color: silver");
            }
        }

        // Changes the color of the New Game button, so that it's
        // clear what button should click next.
        ui->createBoard->setStyleSheet("background-color: thistle");
        ui->createBoard->setDisabled(false);
        ui->boardDimension->setDisabled(false);
        changePushButtonsState(true);
        gameState = true;
    }
}

void MainWindow::on_boardDimension_valueChanged(int arg1) {
// Saves the SpinBox value to a variable, when the value is changed.

    gridSize = arg1;
}

void MainWindow::changePushButtonsState(bool state) {
// Disables or enables pushbuttons depending on the function parameter

    ui->ClearButton->setDisabled(state);
    ui->StartButton->setDisabled(state);
    ui->NextButton->setDisabled(state);
}

void MainWindow::on_createBoard_clicked() {
// Creates a new gameboard when the New Game -button is clicked.

    gameState = true;
    ui->createBoard->setStyleSheet("background-color: lightgrey");

    for (Node* node : allCells) { // Deletes all cells from a previous game
        delete node->cell;
    }
    allCells.clear(); // Clears all cells from the vector
    if (round != 0) {
        changePushButtonsState(false);
    }
    calculateCells();
    createBoard();
}

void MainWindow::on_NextButton_clicked() {
// Disables some pushbuttons and goes through the game function once, when
// the Next button is clicked.

    ui->createBoard->setDisabled(true);
    ui->boardDimension->setDisabled(true);
    disableCells(); // Disables all cells when the game is on
    game();
}
