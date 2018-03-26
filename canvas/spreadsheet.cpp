
#include <QtWidgets>

#include <canvas.h>

Spreadsheet::Spreadsheet(QWidget *parent)
    : QTableWidget(parent)
{
    autoRecalc = true;

//    setItemPrototype(new Cell);
    setSelectionMode(ContiguousSelection);

    clear();
}
