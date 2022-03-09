#include "cell.h"



void Cell::setCellState(int state)
{
    if (state == 0) {
        this->cellState = 0;
    } else {
        this->cellState = 1;
    }
}

int Cell::getCellState()
{
    return this->cellState;
}

Cell::Cell(QWidget *parent) : QPushButton(parent)
{}
