#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Node {
    Node* top;
    Node* bottom;
    Node* right;
    Node* left;

    QPushButton button;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_StartButton_clicked();

    void on_ClearButton_clicked();

    void on_boardDimension_valueChanged(int arg1);

    void on_createBoard_clicked();

private:
    Ui::MainWindow *ui;

    void createBoard();
    void buttonClicked(QPushButton*);
    int getCellState(QPushButton*);
    void setInitialState(QPushButton*);
    void calculateCells();

    //private attributes

    std::vector<QPushButton*> initialCells;
    std::vector<QPushButton*> allCells;
    QWidget *widget;
    QGridLayout *layout;
    QPushButton *cell;
    QPushButton *clickedCell;
    int cellAmount = 0;
    int cellState = 0;
    int gridSize = 0;
    int cellSize = 0;
    bool gameState  = false;

    //Constant for pushbutton's (=cell) size
    static const int CELL_SIZE = 13;

};
#endif // MAINWINDOW_H
