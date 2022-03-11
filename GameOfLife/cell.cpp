#include "cell.h"


void Cell::setCellState(int state)
// Sets cell alive, if parameter state is 1 and
// sets cell dead if parameter is 0.
{
    if (state == 0) {
        this->cellState = false;
    } else if (state == 1) {
        this->cellState = true;
    }
}

bool Cell::getCellState()
// Returns true if cell is alive and false if cell is dead.
{
    return this->cellState;
}

Cell::Cell(QWidget *parent) : QPushButton(parent)
{}
