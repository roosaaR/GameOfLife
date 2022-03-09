#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cell.h"
#include <iostream>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->boardDimension->setMinimum(10);
    ui->boardDimension->setMaximum(40);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createBoard()
{   //Creates N x N board with pushButtons. PushButtons represent cells

    ui->gridLayout->setSpacing(0);

    //Creates two-dimensional vector with Nodes(struct)
    std::vector<std::vector<Node*>> nodes;

    createNodes(nodes);
    linkNodes(nodes);
    populateNodes(nodes);

    //Copying Nodes from two-dimensional vector to unordered set.
    for (int i=0; i < gridSize; ++i) {
        for (int j=0; i < gridSize; ++i) {
            allCells.insert(nodes[i][j]);
         }
     }
}

void MainWindow::linkNodes(std::vector<std::vector<Node*>> &nodes) {

    int row = 0;
    int column = 0;

    qDebug() << nodes.size();

    for (int i=0; i < cellAmount; ++i) {
        if (column == gridSize) {
            row += 1;
            column = 0;
        }

        Node* node = nodes[row][column];
        int topIndex = row - 1;
        int bottomIndex = row + 1;
        int leftIndex = column - 1;
        int rightIndex = column + 1;

        if(topIndex < 0) {
            node->top = nullptr;
        } else {
            node->top = nodes[topIndex][column];
        } if (bottomIndex > gridSize) {
            node->bottom = nullptr;
        } else {
            node->bottom = nodes[bottomIndex][column];
        } if (leftIndex < 0) {
            node->left = nullptr;
        } else {
            node->left = nodes[row][leftIndex];
        } if (rightIndex > gridSize) {
            node->right = nullptr;
        } else {
            node->right = nodes[row][rightIndex];
        }
        column += 1;
    }
}

void MainWindow::populateNodes(std::vector<std::vector<Node*>> &nodes) {

    int row = 0;
    int column = 0;

    for (int i=0; i < cellAmount; ++i) {
        Cell *cell = new Cell(this);
        cell->setCellState(0);
        nodes[row][column]->button = cell;
        cell->setStyleSheet("background-color: grey");
        connect(cell, &QPushButton::clicked, this,
                           [=]() { setInitialState(cell); });
        cell->setFixedWidth(CELL_SIZE);
        cell->setFixedHeight(CELL_SIZE);

        if (column == gridSize) {
            row += 1;
            column = 0;
        }

        ui->gridLayout->addWidget(cell, row, column);
        column += 1;
    }
}

void MainWindow::createNodes(std::vector<std::vector<Node*>> &nodes) {

    int row = 0;
    int column = 0;

    std::vector<Node*> rowOfNodes;

    for (int i=0; i < cellAmount; ++i) {
        if (column == gridSize) {
            nodes.push_back(rowOfNodes);
            rowOfNodes.clear();
            row += 1;
            column = 0;
        }
        rowOfNodes.push_back(new Node);
        //nodes[row][column] = new Node;
        column += 1;
    }
}

void MainWindow::game() {
    //Actual game implementation

    //int neighbourCells = 0;

    /*for (Node* cell : allCells) {
        //If the cell is dead
        if (getCellState(cell) == 0) {
           neighbourCells = checkNeighbours(cell);
           if (neighbourCells == 3) {
               cell->button->setStyleSheet("background-color: purple");
           }

        } //If the cell is alive
        else if (getCellState(cell) == 1) {
            neighbourCells = checkNeighbours(cell);
            if (neighbourCells == 0 || neighbourCells == 1) {
                cell->button->setStyleSheet("background-color: grey");
            } else if (neighbourCells >= 4) {
                cell->button->setStyleSheet("background-color: grey");
            }
        }
    }*/
}

int checkNeighbours(Node*) {
    //Calculates how many neighbourcells is alive and returns the amount
    int neighbourAmount = 0;
    return neighbourAmount;
}

void MainWindow::calculateCells() {
    //Calculates total amount of cells in the gameboard
    cellAmount = gridSize * gridSize;
}

void MainWindow::setInitialState(Cell* c) {
    //Creates initial state for the game. Cells turn purple when user
    //clicks them

    clickedCell = static_cast<Cell*>(sender());
    clickedCell->setStyleSheet("background-color: purple");
    initialCells.push_back(clickedCell);
    c->setCellState(1);
}

int MainWindow::getCellState(Node*) {
    //returns 1 if cell is alive and returns 0 if cell is dead
    return 1;
}

void MainWindow::on_StartButton_clicked()
{   //Starts game and disables some buttons while game is on
    if (gameState ==  false) {
        return;
    }  else {
        ui->createBoard->setDisabled(true);
        ui->boardDimension->setDisabled(true);
        ui->StartButton->setDisabled(true);
        game();
    }
}

void MainWindow::on_ClearButton_clicked()
{   //Clears the gameboard and stops the game
    createBoard();
    gameState = false;
    ui->createBoard->setDisabled(false);
    ui->boardDimension->setDisabled(false);
    ui->StartButton->setDisabled(false);
}

void MainWindow::on_boardDimension_valueChanged(int arg1) {
    //Saves the SpinBox value
    gridSize = arg1;
}

void MainWindow::on_createBoard_clicked()
{   // Creates gameboard when createBoard button is clicked.
    calculateCells();
    gameState = true;
    createBoard();
}
