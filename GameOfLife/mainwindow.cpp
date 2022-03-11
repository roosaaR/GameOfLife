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
    ui->boardDimension->setMinimum(10);
    ui->boardDimension->setMaximum(40);
    ui->ClearButton->setDisabled(true);
    ui->StartButton->setDisabled(true);
    ui->NextButton->setDisabled(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(game()));
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

void MainWindow::createBoard()
{   //Creates N x N board with pushButtons. PushButtons represent cells

    ui->gridLayout->setSpacing(0);


    //Initializes a two-dimensional vector of Nodes(struct)
    std::vector<std::vector<Node*>> nodes;

    createNodes(nodes);
    linkNodes(nodes);
    populateNodes(nodes);

    //Copying Nodes from two-dimensional vector to unordered set.
    for (int i=0; i < gridSize; ++i) {
        for (int j=0; j < gridSize; ++j) {
            allCells.insert(nodes[i][j]);
         }
     }

    nodes.clear(); //Clears the vector
}

void MainWindow::linkNodes(std::vector<std::vector<Node*>> &nodes) {
    // Iterates through gamegrid and links every cell to its neighbourcells.
    // Every cell has four neighbours.

    int row = 0;
    int column = 0;

    for (int i=0; i < cellAmount; ++i) {

        Node* node = nodes[row][column];
        int topIndex = row - 1;
        int bottomIndex = row + 1;
        int leftIndex = column - 1;
        int rightIndex = column + 1;

        if(topIndex >= 0) {  // top neighbourcell
            node->neighbourCells.insert(nodes[topIndex][column]);
        } if (bottomIndex < gridSize) { // bottom neighbour
            node->neighbourCells.insert(nodes[bottomIndex][column]);
        } if (leftIndex >= 0) { // left neighbour
            node->neighbourCells.insert(nodes[row][leftIndex]);
        } if (rightIndex < gridSize) { // right neighbour
            node->neighbourCells.insert(nodes[row][rightIndex]);
        } if (topIndex >= 0 && leftIndex >= 0) { // top left corner neighbour
            node->neighbourCells.insert(nodes[topIndex][leftIndex]);
        } if (bottomIndex < gridSize && leftIndex >= 0) { // bottom left corner neighbour
            node->neighbourCells.insert(nodes[bottomIndex][leftIndex]);
        } if (bottomIndex < gridSize && rightIndex < gridSize) { // bottom right corner neighbour
            node->neighbourCells.insert(nodes[bottomIndex][rightIndex]);
        } if (topIndex >= 0 && rightIndex < gridSize) { // top right corner neighbour
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
// Creates a new Cell object for every Node. Cells are pushbuttons
// and cells are initially dead (grey).

    int row = 0;
    int column = 0;

    for (int i=0; i < cellAmount; ++i) {
        Cell *cell = new Cell(this);
        nodes[row][column]->cell = cell;
        cell->setCellState(0); // Setting a cell as "dead"
        cell->setStyleSheet("background-color: silver");
        connect(cell, &QPushButton::clicked, this,
                      [=]() { setInitialState(cell); });

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

void MainWindow::createNodes(std::vector<std::vector<Node*>> &nodes) {
    // Iterates through gamegrid and creates a new Node struct for every cell
    // Nodes are added to vector

    int column = 0;

    std::vector<Node*> rowofNodes;

    for (int i=0; i < cellAmount; ++i) {

        rowofNodes.push_back(new Node); // Adds all cells of one row to a vector
        column += 1;
        if (column == gridSize) {
            nodes.push_back(rowofNodes);
            rowofNodes.clear();
            column = 0;
        }
    }
}

void MainWindow::game() {
//Actual game implementation

    int neighbourCells = 0;

    // Iterates through a set of all cells
    for (Node* node : allCells) {

        //If the cell is dead
        if (node->cell->getCellState() == false) {
           neighbourCells = checkNeighbours(node);
           if (neighbourCells == 3) {
               // Cell turns alive as if by regrowth
               awakeningCells.push_back(node);
           }
        }
        //If the cell is alive
        else if (node->cell->getCellState() == true) {
            neighbourCells = checkNeighbours(node);
            if ((neighbourCells == 0) || (neighbourCells == 1)) {
                // Cell dies as if by solitude
                dyingCells.push_back(node);
            } else if (neighbourCells >= 4) {
                // Cells dies as if by overpopulation
                dyingCells.push_back(node);
            } else if ((neighbourCells == 2) || (neighbourCells == 3)) {
                // Cell remains alive
                awakeningCells.push_back(node);
            }
        }
    }

    setNewState();
}

void MainWindow::setNewState() {    
    // Cells turn dead
     for(Node* node : dyingCells) {
        node->cell->setStyleSheet("background-color: silver");
        node->cell->setCellState(0);
     }
     dyingCells.clear();

     // Cells turn alive
     for (Node* node : awakeningCells) {
         node->cell->setStyleSheet("background-color: purple");
         node->cell->setCellState(1);
     }
     awakeningCells.clear();
}

int MainWindow::checkNeighbours(Node* node) {
    //Calculates how many neighbourcells is alive and returns the amount

    int neighbourAmount = 0;

    for (Node* n : node->neighbourCells){
        if (n->cell->getCellState() == true )
        neighbourAmount += 1;
    }

    return neighbourAmount;
}

void MainWindow::calculateCells() {

    //Calculates total amount of cells in the gameboard
    cellAmount = gridSize * gridSize;
}

void MainWindow::setInitialState(Cell* c) {
    //Creates initial state for the game. Cells turn purple when user
    //clicks them

    c->setStyleSheet("background-color: purple");
    c->setCellState(1);
    ui->boardDimension->setDisabled(true);
    ui->createBoard->setDisabled(true);
}

void MainWindow::disableCells() {
// Sets all pushButtons (cells) disabled
    for (Node* node : allCells) {
        node->cell->setDisabled(true);
    }
}

void MainWindow::enableCells() {
    for (Node* node : allCells) {
        node->cell->setEnabled(true);
    }
}

void MainWindow::on_StartButton_clicked()
{   //Starts game and disables some buttons when game is on
    if (gameState == false) {
        return;
    }  else {
        ui->createBoard->setDisabled(true);
        ui->boardDimension->setDisabled(true);
        ui->StartButton->setDisabled(true);
        ui->NextButton->setDisabled(true);
        disableCells(); // Disables all cells when game is on

        timer->start(500);
    }
}

void MainWindow::on_ClearButton_clicked()
{   // Clears the gameboard and stops the game, if game is on.

    if (gameState == false) {
        return;
    } else {
        timer->stop();
        for (Node* node : allCells) {
            if (node->cell->getCellState() == true){
                node->cell->setStyleSheet("background-color: silver");
            }
        }
        ui->createBoard->setDisabled(false);
        ui->boardDimension->setDisabled(false);
        changePushButtonsState(true);
        gameState = true;
    }
}

void MainWindow::on_boardDimension_valueChanged(int arg1) {
    //Saves the SpinBox value
    gridSize = arg1;
}

void MainWindow::changePushButtonsState(bool state) {
    ui->ClearButton->setDisabled(state);
    ui->StartButton->setDisabled(state);
    ui->NextButton->setDisabled(state);
}

void MainWindow::on_createBoard_clicked()
{   // Creates gameboard when createBoard button is clicked.
    for (Node* node : allCells) {
        delete node->cell;
    }
    allCells.clear(); // Clears all cells from the vector
    changePushButtonsState(false);
    calculateCells();
    gameState = true;
    createBoard();
}

void MainWindow::on_NextButton_clicked()
// Disables some buttons and starts the game when NextButton is clicked.
{   ui->createBoard->setDisabled(true);
    ui->boardDimension->setDisabled(true);
    disableCells(); // Disables all cells when game is on
    game();
}
