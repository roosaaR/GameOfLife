#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <vector>
#include <unordered_set>
#include <cell.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Node {
    Node* top;
    Node* bottom;
    Node* right;
    Node* left;
    Cell* cell;
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

    void on_NextButton_clicked();

private:
    Ui::MainWindow *ui;

    //private functions
    void createBoard();
    void setInitialState(Cell*);
    void calculateCells();
    void game();
    int checkNeighbours(Node*);
    void createNodes(std::vector<std::vector<Node*>> &nodes);
    void linkNodes(std::vector<std::vector<Node*>> &nodes);
    void populateNodes(std::vector<std::vector<Node*>> &nodes);
    void disablePushButtons();
    void setNewState();


    //private attributes
    std::unordered_set<Node*> allCells;
    std::vector<Node*> dyingCells;
    std::vector<Node*> awakeningCells;
    QWidget *widget;
    QGridLayout *layout;
    QPushButton *clickedCell;
    int cellAmount = 0;
    int gridSize = 0;
    bool gameState  = false;

    //Constant for pushbutton's (=cell) size
    static const int CELL_SIZE = 13;

};
#endif // MAINWINDOW_H
