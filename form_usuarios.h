#ifndef FORM_USUARIOS_H
#define FORM_USUARIOS_H

#include <QWidget>


namespace Ui {
class form_usuarios;
}

class form_usuarios : public QWidget
{
    Q_OBJECT

public:
    explicit form_usuarios(QWidget *parent = 0);
    ~form_usuarios();

private slots:
    void on_Boton_buscar_clicked();

    void on_text_usuario_textChanged(const QString &arg1);

    void on_list_usuarios_clicked(const QModelIndex &index);

    void on_text_usuario_returnPressed();

    void prueba();

private:
    Ui::form_usuarios *ui;

};

#endif // FORM_USUARIOS_H
