#include "mainwindow.h"
#include "ui_mainwindow.h"


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
{
    //Creates N x N board with pushButtons. PushButtons represent cells

    int row = 0;
    int column = 0;

    ui->gridLayout->setSpacing(0);

    Node* nodes[gridSize][gridSize];

    for (int i=0; i < cellAmount; ++i) {
        if (column == gridSize) {
            row += 1;
            column = 0;
        }
        nodes[row][column] = new Node;
        column += 1;
    }

    row = 0;
    column = 0;

    for (int i=0; i < cellAmount; ++i) {
        QPushButton *cell = new QPushButton(this);
        cell->setStyleSheet("background-color: grey");
        connect(cell, &QPushButton::clicked, this,
                           [=]() { setInitialState(cell); });
        allCells.push_back(cell);
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


void MainWindow::calculateCells() {

    cellAmount = gridSize * gridSize;
}

void MainWindow::setInitialState(QPushButton*) {

    clickedCell = static_cast<QPushButton*>(sender());
    clickedCell->setStyleSheet("background-color: purple");
    initialCells.push_back(clickedCell);

}

void MainWindow::buttonClicked(QPushButton*) {

    clickedCell = static_cast<QPushButton*>(sender());
    clickedCell->setStyleSheet("background-color: purple");

}

int MainWindow::getCellState(QPushButton*) {
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
{   // Creates gameboard when start button is clicked.
    calculateCells();
    createBoard();
    gameState = true;
}
