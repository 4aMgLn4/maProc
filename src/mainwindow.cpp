#include "include/mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QMessageBox>

#define ICON_PASS_PID "../src/assets/pid_proc.png"
#define ICON_CLEAN "../src/assets/clean.png"
#define ICON_SEARCH "../src/assets/search.png"
#define TITLE_WINDOW "maProc v1.0"
#define CLEAN_ROW " "

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(TITLE_WINDOW);

    // status bar conf default
    ui->statusBar->showMessage("Not passed process for map");

    // buttons
    ui->statusBar->addPermanentWidget(ui->setPid);
    ui->statusBar->addPermanentWidget(ui->mapButton);
    conf_button_pass_pid();
    conf_button_clean();
    conf_button_search();
    
    column_config_all();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::conf_button_pass_pid()
{
    ui->pidButton->setIcon(QIcon(ICON_PASS_PID));
}

void MainWindow::conf_button_clean()
{
    ui->statusBar->addPermanentWidget(ui->cleanButton);

    ui->cleanButton->setIcon(QIcon(ICON_CLEAN));
}

void MainWindow::conf_button_search()
{
    ui->searchButton->setIcon(QIcon(ICON_SEARCH));
}

void MainWindow::column_config_all()
{
    QStringList column;
    column << "Address"
           << "Value"
           << "Memory";

    QStringList column_infos;
    column_infos << "Address_on"
                 << "Address_off"
                 << "Size_map";

    // view address
    ui->viewAddress->setColumnCount(3);
    ui->viewAddress->setHorizontalHeaderLabels(column);
    ui->viewAddress->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->infos_addr->setColumnCount(3);
    ui->infos_addr->setHorizontalHeaderLabels(column_infos);
}

void MainWindow::set_values_column_heap()
{
    // heaView config
    // int rowCount_heap = ui->viewAddress->rowCount();

    ui->viewAddress->setShowGrid(false);
    ui->viewAddress->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // ui->viewAddress->insertRow(rowCount_heap);

    // infos_addr
    QString on = QString::fromStdString(mapper.get_addr_on());
    QString off = QString::fromStdString(mapper.get_addr_off());

    ui->infos_addr->setShowGrid(false);
    ui->infos_addr->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // config itens
    QTableWidgetItem *on_item = new QTableWidgetItem(on);
    on_item->setTextAlignment(100);

    QTableWidgetItem *off_item = new QTableWidgetItem(off);
    off_item->setTextAlignment(100);

    QTableWidgetItem *sizeMap_item = new QTableWidgetItem(QString::number(mapper.get_size_address()));
    sizeMap_item->setTextAlignment(100);

    // QTableWidgetItem *Address_mapped = new QTableWidgetItem("done");
    // Address_mapped->setTextAlignment(100);

    // set itens
    ui->infos_addr->setItem(0, Address_on, on_item);
    ui->infos_addr->setItem(0, Address_off, off_item);
    ui->infos_addr->setItem(0, Size_map, sizeMap_item);

    // ui->viewAddress->setItem(rowCount_heap - 1, Address, Address_mapped);
}

void MainWindow::set_values_column_stack()
{
    // viewAddress config
    // int rowCount_stack = ui->viewAddress->rowCount() - 1;

    ui->viewAddress->setShowGrid(false);
    ui->viewAddress->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // ui->viewAddress->insertRow(rowCount_heap);

    // infos_addr
    QString on = QString::fromStdString(mapper.get_addr_on());
    QString off = QString::fromStdString(mapper.get_addr_off());

    // config itens
    QTableWidgetItem *on_item = new QTableWidgetItem(on);
    on_item->setTextAlignment(100);

    QTableWidgetItem *off_item = new QTableWidgetItem(off);
    off_item->setTextAlignment(100);

    QTableWidgetItem *sizeMap_item = new QTableWidgetItem(QString::number(mapper.get_size_address()));
    sizeMap_item->setTextAlignment(100);

    // set itens
    ui->infos_addr->setItem(1, Address_on, on_item);
    ui->infos_addr->setItem(1, Address_off, off_item);
    ui->infos_addr->setItem(1, Size_map, sizeMap_item);
}

void MainWindow::column_clean(QTableWidget *__column, bool __delete = false)
{
    if (__delete)
    {
        while (__column->rowCount() > 0)
            __column->removeRow(0);
    }
    else
    {
        for (int i = 1; i >= 0; i--)
        {
            __column->setItem(i, Address_on, new QTableWidgetItem(CLEAN_ROW));
            __column->setItem(i, Address_off, new QTableWidgetItem(CLEAN_ROW));
            __column->setItem(i, Size_map, new QTableWidgetItem(CLEAN_ROW));
        }
    }
}

bool MainWindow::verify_pid()
{
    pid = ui->setPid->text().toStdString();

    if (pid.size() == 0)
    {
        QMessageBox::about(nullptr,
                           "Error",
                           "Empty pid passed");
        return false;
    }
    if (mapper.map_pid(pid) == -1)
    {
        QMessageBox::about(nullptr,
                           "Warning",
                           "Error not found pid");
        return false;
    }
    else
        ui->statusBar->showMessage("Mapping process PID " + ui->setPid->text()); // tell the status bar which pid is being mapped

    return true;
}

bool MainWindow::mapper_heap()
{
    bool status_exit = true;
    if (mapper.map_mem("[heap]") == false)
    {
        QMessageBox::about(nullptr,
                           "Warning",
                           "Error heap not mapped");
        status_exit = false;
    }

    return status_exit;
}

bool MainWindow::mapper_stack()
{
    bool status_exit = true;
    if (mapper.map_mem("[stack]") == false)
    {
        QMessageBox::about(nullptr,
                           "Warning",
                           "Error stack not mapped");
        status_exit = false;
    }

    return status_exit;
}

void MainWindow::on_mapButton_clicked()
{
    column_clean(ui->viewAddress, true);
    column_clean(ui->viewAddress, true);
    column_clean(ui->infos_addr);

    if (verify_pid() == false)
        return;

    // heap class mapping
    if (mapper_heap())
    {
        if (mapper.get_addr_on().size() == 0)
        {
            QMessageBox::about(nullptr,
                               "Info",
                               "Process not usage heap");
        }
        else
            set_values_column_heap();
    }

    // stack class mapping
    if (mapper_stack())
    {
        if (mapper.get_addr_on().size() == 0)
        {
            QMessageBox::about(nullptr,
                               "Info",
                               "Process not usage stack");
            return;
        }
        else
            set_values_column_stack();
    }
}

void MainWindow::on_cleanButton_clicked()
{
    column_clean(ui->viewAddress, true);
    column_clean(ui->viewAddress, true);
    column_clean(ui->infos_addr);
    column_clean(ui->infos_addr);
}

void MainWindow::on_pidButton_clicked()
{
    dir.exec();
}