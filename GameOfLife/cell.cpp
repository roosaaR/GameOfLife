#include "cell.h"



void Cell::setCellState(int state)
{
    if (state == 0) {
        this->cellState = false;
    } else {
        this->cellState = true;
    }
}

int Cell::getCellState()
{
    return this->cellState;
}

Cell::Cell(QWidget *parent) : QPushButton(parent)
{}
