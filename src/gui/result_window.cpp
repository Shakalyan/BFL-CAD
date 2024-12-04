#include "result_window.h"

#include "logger.h"

#include <QTableWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTableView>
#include <QVBoxLayout>
#include <QSplitter>
#include <QLabel>

BFCAD::UI::BFInfoFrame::BFInfoFrame(std::unique_ptr<BooleanFunction> bf, QWidget *parent) : QFrame(parent)
{
    QLabel *bf_repr = new QLabel(this);
    bf_repr->setText(" Boolean Function: " + QString::fromStdString(bf->to_string()));
    bf_repr->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    bf_repr->setWordWrap(true);

    std::unique_ptr<TruthTable> truth_table = bf->get_truth_table();
    QStandardItemModel *truth_table_model = new QStandardItemModel(truth_table->rows_count, truth_table->columns_count, this);
    for (int c = 0; c < truth_table->columns_count; ++c) {
        truth_table_model->setHeaderData(c, Qt::Horizontal, QString::fromStdString(truth_table->headers[c]));
        for (int r = 0; r < truth_table->rows_count; ++r) {
            QString value = QString::fromStdString(std::to_string(truth_table->table[r][c]));
            truth_table_model->setItem(r, c, new QStandardItem(value));
        }
    }
    // truth_table_model->setItem(0, 0, new QStandardItem("0"));
    // truth_table_model->setItem(0, 1, new QStandardItem("0"));
    // truth_table_model->setItem(0, 2, new QStandardItem("1"));
    // truth_table_model->setItem(1, 0, new QStandardItem("0"));
    // truth_table_model->setItem(1, 1, new QStandardItem("1"));
    // truth_table_model->setItem(1, 2, new QStandardItem("0"));
    // truth_table_model->setItem(2, 0, new QStandardItem("0"));
    // truth_table_model->setItem(2, 1, new QStandardItem("1"));
    // truth_table_model->setItem(2, 2, new QStandardItem("0"));
    // truth_table_model->setItem(3, 0, new QStandardItem("0"));
    // truth_table_model->setItem(3, 1, new QStandardItem("1"));
    // truth_table_model->setItem(3, 2, new QStandardItem("0"));

    // truth_table_model->setHeaderData(0, Qt::Horizontal, "x1");
    // truth_table_model->setHeaderData(1, Qt::Horizontal, "x2");
    // truth_table_model->setHeaderData(2, Qt::Horizontal, "Result asdfasdfj;alsdkfj;alksjdf;lakjsd;flkjas;lkdjf;alksjdf;laksjdf;lasjdf;lkajsdfl;kjasd;lfkjasl;kdfjasl;kdjf");

    QTableView *view = new QTableView(this);
    view->setModel(truth_table_model);
    view->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    view->resizeColumnsToContents();
    view->resizeRowsToContents();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(bf_repr);
    layout->addWidget(view);
    layout->setStretchFactor(bf_repr, 0);
    layout->setStretchFactor(view, 1);
    //layout->setContentsMargins(5, 5, 5, 5);

    setLayout(layout);
    Logger::log("end creating bf info frame");
}

BFCAD::UI::ResultWindow::ResultWindow(std::unique_ptr<BooleanFunction> initial_bf, 
                                      std::unique_ptr<BooleanFunction> optimized_bf, 
                                      QWidget *parent) : QMainWindow(parent)
{
    BFInfoFrame *bf_info = new BFInfoFrame(std::move(initial_bf), this);
    BFInfoFrame *bf_info2 = new BFInfoFrame(std::move(optimized_bf), this);

    QSplitter *horizontal_splitter = new QSplitter(Qt::Horizontal, this);
    horizontal_splitter->addWidget(bf_info);
    horizontal_splitter->addWidget(bf_info2);
    
    setCentralWidget(horizontal_splitter);    
}