#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form_usuarios.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Boton_Usuarios_clicked()
{
    form_usuarios *usuarios = new form_usuarios;
    usuarios->show();

}
