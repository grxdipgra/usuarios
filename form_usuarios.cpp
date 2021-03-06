﻿#include "form_usuarios.h"
#include "ui_form_usuarios.h"
//para los msgbox:
#include <QMessageBox>
//para operar con comando de consola
#include <QProcess>
#include <QDebug>
#include <QTextStream>
#include <QFile>
#include <QDateTime>

form_usuarios::form_usuarios(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form_usuarios)
{
    ui->setupUi(this);

    //ocultamos el listwidget al abrir el formulario
    ui->list_usuarios->setVisible(false);
    ui->list_usuarios_2->setVisible(false);
    //prueba();
}

form_usuarios::~form_usuarios()
{
    delete ui;
}


void form_usuarios::on_Boton_buscar_clicked()
{

    QString salida, usuario;
    QStringList argumentos;
    QMessageBox msgBox;
    QProcess proceso;
    int pos1,pos2;

    //proceso.start("ls -la");

    //proceso.start(proceso.readAllStandardOutput() + " | grep si_serafin");

    proceso.start("ldapsearch -QLLL -o ldif-wrap=no -b \"dc = grx\" \"(&(!(objectclass=computer))(objectclass=user)(samAccountName=*" + ui->text_usuario->text() + "*))\" sAMAccountName ");
    //proceso.start(proceso.readAllStandardOutput() + " |grep sAMAccountName");

    //argumentos.insert(0,"|grep sAMAccountName");
    //argumentos.insert(1,"/home/si_serafin/Imágenes/103APPLE");
    //msgBox.setText(argumentos.value(0));
    //msgBox.exec();

    //metemos el contenido del text_usuario en la lista
    //ui->list_usuarios->addItem(ui->text_usuario->text());

    //proceso.start("/home/si_serafin/asistencia/ldap/ldap_usuario.sh");

    ui->list_usuarios->clear();

    proceso.waitForFinished();
    salida=proceso.readAllStandardOutput();

    //qDebug() << ui->text_usuario->text();

    if (salida.length()>0) {

        ui->list_usuarios->setVisible(true);

        pos1=0;
        int num_usu=salida.count("sAMAccountName");

        for(int i =1; i<=num_usu; i++)
        {
            pos1=salida.indexOf("sAMAccountName", pos1)+15;
            pos2=salida.indexOf("\n",pos1);
            usuario=salida.mid(pos1,pos2-pos1);
            ui->list_usuarios->addItem(usuario);
        };
    }
    else
        ui->list_usuarios->setVisible(false);



    //Otra manera de hacerlo cortando la cadena salida en cada paso
    //    for(int i =1; i<=num_usu; i++)
    //    {
    //        pos2=salida.indexOf("sAMAccountName")+15;
    //        salida=salida.remove(pos1,pos2);
    //        usuario=salida.mid(1,salida.indexOf("\n"));

    //        msgBox.setText(usuario.insert(0,"Usuarios de Active Directory:\n\n"));
    //       //msgBox.setText(usuario + "  " + QString::number(salida.length()) + "  " + QString::number(pos1)+ "  " + QString::number(pos2));
    //        msgBox.exec();
    //    };

    //salida=proceso.readAllStandardError();

    //msgBox.setText(salida.insert(0,"Usuarios de Active Directory:\n\n"));
    //msgBox.setText(QString::number(salida.indexOf("sAMAccountName")));
   // msgBox.setText(salida.mid(salida.indexOf("sAMAccountName")+16,salida.indexOf("sAMAccountName")+30));

    //numero de usuarios devueltos
    //msgBox.setText(QString::number(salida.count("sAMAccountName")));

    //msgBox.exec();
}

void form_usuarios::on_text_usuario_textChanged(const QString &arg1)
{
    //QMessageBox msgBox;
    //msgBox.setText(arg1);
    //msgBox.exec();
    on_Boton_buscar_clicked();
    //qDebug() << ui->list_usuarios->visibleRegion();

}

void form_usuarios::on_list_usuarios_clicked(const QModelIndex &index)
{
    ui->text_usuario->setText(ui->list_usuarios->currentItem()->text().trimmed());
    ui->list_usuarios->setVisible(false);
    on_text_usuario_returnPressed();
}

//cuando pulsemos intro si hay un usuario en la lista lo escriba en el text y si hay mas se vaya a la lista
void form_usuarios::on_text_usuario_returnPressed()
{
    QString salida, usuario, salida1, temp;
    QProcess proceso;
    QMessageBox msgBox;
    int pos1,pos2;
    QDateTime fecha;


    //Si la lista está visible
    if (ui->list_usuarios->visibleRegion().isNull()==false){

        //si hay solo un elemento en la lista
        if (ui->list_usuarios->count()==1){
            ui->text_usuario->setText(ui->list_usuarios->item(0)->text().trimmed());
            ui->list_usuarios->setVisible(false);
        }

        else
            ui->list_usuarios->setFocus();
    }
    //Si la lista ya no está visible enseñamos los campos
    else{
//        msgBox.setText("OK, vamos a rellenar los datos del usuario");
//        msgBox.exec();

        //ejecutamos la consulta Ldap
        proceso.start("ldapsearch -QLLL -o ldif-wrap=no -b \"dc = grx\" \"(&(!(objectclass=computer))(objectclass=user)(samAccountName=" + ui->text_usuario->text() + "))\" ");
        proceso.waitForFinished();
        salida=proceso.readAllStandardOutput();

//        //consulta Ldap cargada en archivo
//        QFile archivo("/home/si_serafin/git/usuarios/ldap.txt");
//        archivo.open(QIODevice::ReadOnly);
//        QTextStream text_archivo(&archivo);
//        salida=text_archivo.readAll();

        //nombre y apellidos
        /*pos1=salida.indexOf("cn:")+3;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1);
        ui->text_nombre->setText(usuario);*/

        //telefono
        pos1=salida.indexOf("telephoneNumber:")+16;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1);
        ui->text_telefono->setText(usuario);

        //correo
        pos1=salida.indexOf("mail:")+5;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1);
        ui->text_correo->setText(usuario);

        //ultimo logon
        pos1=salida.indexOf("lastLogon:")+10;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1);
        fecha.setSecsSinceEpoch((usuario.toLongLong()/10000000)-11644473600);
        //ui->text_ulti_login->setText(fecha.date().toString(Qt::SystemLocaleDate));
        ui->text_ulti_login->setText(fecha.toString("dd-MM-yyyy"));

        //la cuenta caduca
        pos1=salida.indexOf("accountExpires:")+15;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();;
        qDebug()<<usuario;
        //usuario="9223372036854775807";
        if (usuario.toLongLong()==9223372036854775807 || usuario=="0")
             ui->text_cuenta_caduca->setText("No Caduca");
        else {
            fecha.setSecsSinceEpoch((usuario.toLongLong()/10000000)-11644473600);
            //ui->text_ulti_login->setText(fecha.date().toString(Qt::SystemLocaleDate));
            ui->text_cuenta_caduca->setText(fecha.toString("dd-MM-yyyy hh:mm"));
        }

        //Estado
        pos1=salida.indexOf("lockoutTime:")+12;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();
        if (usuario=="0")
            ui->text_estado->setText("Activa");
        else
            ui->text_estado->setText("Bloqueada");

        //Numero de logon
        pos1=salida.indexOf("logonCount:")+11;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();
        ui->text_logon->setText(usuario);

        //Creada
        pos1=salida.indexOf("whenCreated:")+12;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();
        usuario=usuario.mid(6,2)+"-"+usuario.mid(4,2)+"-"+usuario.mid(0,4);
        ui->text_creada->setText(usuario);

        //cuando se creo la cuenta
        pos1=salida.indexOf("whenCreated:")+12;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();
        usuario=usuario.mid(6,2)+"-"+usuario.mid(4,2)+"-"+usuario.mid(0,4);
        ui->text_creada->setText(usuario);

        //cuando se modifico por ultima vez la cuenta
        pos1=salida.indexOf("whenChanged:")+12;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();
        usuario=usuario.mid(6,2)+"-"+usuario.mid(4,2)+"-"+usuario.mid(0,4);
        ui->text_modif_cuenta->setText(usuario);

        //cuando se creo la cuenta de correo
        pos1=salida.indexOf("msExchWhenMailboxCreated:")+25;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();
        usuario=usuario.mid(6,2)+"-"+usuario.mid(4,2)+"-"+usuario.mid(0,4);
        ui->text_fecha_correo->setText(usuario);

        //Ultimo cambio de contraseña
        pos1=salida.indexOf("pwdLastSet:")+11;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();
        temp=salida.mid(pos1,pos2-pos1).trimmed();
        fecha.setSecsSinceEpoch((usuario.toLongLong()/10000000)-11644473600);
        ui->text_cambio_clave->setText(fecha.toString("dd-MM-yyyy  hh:mm"));

        //Intentos fallidos de contraseña
        pos1=salida.indexOf("badPwdCount:")+12;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();
        ui->text_intentos->setText(usuario);

        pos1=salida.indexOf("userAccountControl:")+20;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();

        //useraccountcontrol=66048 	Enabled, Password Doesn't Expire
        //useraccountcontrol=66050 	Disabled, Password Doesn't Expire
        if (usuario=="66048") {
            ui->text_clave_caduca->setText("No caduca");
        }
        else{
            //ejecutamos la consulta Ldap para obtener el campo maxPwdAge
            proceso.start("ldapsearch -QLLL -o ldif-wrap=no -b \"dc = grx\" \"(&(objectClass=domain))\" maxPwdAge");
            proceso.waitForFinished();
            salida1=proceso.readAllStandardOutput();

    //        //consulta Ldap cargada en archivo
    //        QFile archivo1("/home/si_serafin/git/usuarios/ldap_dominio.txt");
    //        archivo1.open(QIODevice::ReadOnly);
    //        QTextStream text_archivo1(&archivo1);
    //        salida1=text_archivo1.readAll();

            pos1=salida1.indexOf("maxPwdAge:")+11;
            pos2=salida1.indexOf("\n",pos1);
            usuario=salida1.mid(pos1,pos2-pos1).trimmed();
            //qDebug() << usuario;
            usuario=QString::number(temp.toLongLong()-usuario.toLongLong());
            fecha.setSecsSinceEpoch((usuario.toLongLong()/10000000)-11644473600);
            ui->text_clave_caduca->setText(fecha.toString("dd-MM-yyyy  hh:mm"));
        }

    }
}

//prueba de implementacion de procedimiento y llamada
void form_usuarios::prueba()
{
    QMessageBox msgBox;
    msgBox.setText("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
    msgBox.exec();

}



void form_usuarios::on_text_nombre_textChanged(const QString &arg1)
{
    QString salida, usuario;
    QStringList argumentos;
    QMessageBox msgBox;
    QProcess proceso;
    int pos1,pos2;

    proceso.start("ldapsearch -QLLL -o ldif-wrap=no -b \"dc = grx\" \"(&(!(objectclass=computer))(objectclass=user)(cn=*" + ui->text_nombre->text() + "*))\" CN ");

    ui->list_usuarios_2->clear();

    proceso.waitForFinished();
    salida=proceso.readAllStandardOutput();

    if (salida.length()>0) {

        ui->list_usuarios_2->setVisible(true);

        pos1=0;
        int num_usu=salida.count("cn");

        for(int i =1; i<=num_usu; i++)
        {
            pos1=salida.indexOf("cn:", pos1)+3;
            pos2=salida.indexOf("\n",pos1);
            usuario=salida.mid(pos1,pos2-pos1);
            ui->list_usuarios_2->addItem(usuario);
        };
    }
    else
        ui->list_usuarios_2->setVisible(false);


    //msgBox.setText(salida.insert(0,"Usuarios de Active Directory:\n\n"));
    //msgBox.setText(QString::number(salida.indexOf("sAMAccountName")));
   // msgBox.setText(salida.mid(salida.indexOf("sAMAccountName")+16,salida.indexOf("sAMAccountName")+30));

    //numero de usuarios devueltos
    //msgBox.setText(QString::number(salida.count("sAMAccountName")));

    //msgBox.exec();
}


void form_usuarios::on_list_usuarios_2_clicked(const QModelIndex &index)
{
    ui->text_nombre->setText(ui->list_usuarios_2->currentItem()->text().trimmed());
    ui->list_usuarios_2->setVisible(false);
    on_text_nombre_returnPressed();

}

void form_usuarios::on_text_nombre_returnPressed()
{

    QString salida, usuario, salida1, temp;
    QProcess proceso;
    QMessageBox msgBox;
    int pos1,pos2;
    QDateTime fecha;


    //Si la lista está visible
    if (ui->list_usuarios_2->visibleRegion().isNull()==false){

        //si hay solo un elemento en la lista
        if (ui->list_usuarios_2->count()==1){
            ui->text_nombre->setText(ui->list_usuarios_2->item(0)->text().trimmed());
            ui->list_usuarios_2->setVisible(false);
        }

        else
            ui->list_usuarios_2->setFocus();
    }
    //Si la lista ya no está visible enseñamos los campos
    else{
//        msgBox.setText("OK, vamos a rellenar los datos del usuario");
//        msgBox.exec();

        //ejecutamos la consulta Ldap
        proceso.start("ldapsearch -QLLL -o ldif-wrap=no -b \"dc = grx\" \"(&(!(objectclass=computer))(objectclass=user)(cn=" + ui->text_nombre->text() + "))\" ");
        proceso.waitForFinished();
        salida=proceso.readAllStandardOutput();

//        //consulta Ldap cargada en archivo
//        QFile archivo("/home/si_serafin/git/usuarios/ldap.txt");
//        archivo.open(QIODevice::ReadOnly);
//        QTextStream text_archivo(&archivo);
//        salida=text_archivo.readAll();

        //nombre y apellidos
        /*pos1=salida.indexOf("cn:")+3;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1);
        ui->text_nombre->setText(usuario);*/

        //telefono
        pos1=salida.indexOf("telephoneNumber:")+16;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1);
        ui->text_telefono->setText(usuario);

        //correo
        pos1=salida.indexOf("mail:")+5;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1);
        ui->text_correo->setText(usuario);

        //ultimo logon
        pos1=salida.indexOf("lastLogon:")+10;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1);
        fecha.setSecsSinceEpoch((usuario.toLongLong()/10000000)-11644473600);
        //ui->text_ulti_login->setText(fecha.date().toString(Qt::SystemLocaleDate));
        ui->text_ulti_login->setText(fecha.toString("dd-MM-yyyy"));

        //la cuenta caduca
        pos1=salida.indexOf("accountExpires:")+15;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();;
        qDebug()<<usuario;
        //usuario="9223372036854775807";
        if (usuario.toLongLong()==9223372036854775807 || usuario=="0")
             ui->text_cuenta_caduca->setText("No Caduca");
        else {
            fecha.setSecsSinceEpoch((usuario.toLongLong()/10000000)-11644473600);
            //ui->text_ulti_login->setText(fecha.date().toString(Qt::SystemLocaleDate));
            ui->text_cuenta_caduca->setText(fecha.toString("dd-MM-yyyy hh:mm"));
        }

        //Estado
        pos1=salida.indexOf("lockoutTime:")+12;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();
        if (usuario=="0")
            ui->text_estado->setText("Activa");
        else
            ui->text_estado->setText("Bloqueada");

        //Numero de logon
        pos1=salida.indexOf("logonCount:")+11;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();
        ui->text_logon->setText(usuario);

        //Creada
        pos1=salida.indexOf("whenCreated:")+12;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();
        usuario=usuario.mid(6,2)+"-"+usuario.mid(4,2)+"-"+usuario.mid(0,4);
        ui->text_creada->setText(usuario);

        //cuando se creo la cuenta
        pos1=salida.indexOf("whenCreated:")+12;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();
        usuario=usuario.mid(6,2)+"-"+usuario.mid(4,2)+"-"+usuario.mid(0,4);
        ui->text_creada->setText(usuario);

        //cuando se modifico por ultima vez la cuenta
        pos1=salida.indexOf("whenChanged:")+12;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();
        usuario=usuario.mid(6,2)+"-"+usuario.mid(4,2)+"-"+usuario.mid(0,4);
        ui->text_modif_cuenta->setText(usuario);

        //cuando se creo la cuenta de correo
        pos1=salida.indexOf("msExchWhenMailboxCreated:")+25;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();
        usuario=usuario.mid(6,2)+"-"+usuario.mid(4,2)+"-"+usuario.mid(0,4);
        ui->text_fecha_correo->setText(usuario);

        //Ultimo cambio de contraseña
        pos1=salida.indexOf("pwdLastSet:")+11;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();
        temp=salida.mid(pos1,pos2-pos1).trimmed();
        fecha.setSecsSinceEpoch((usuario.toLongLong()/10000000)-11644473600);
        ui->text_cambio_clave->setText(fecha.toString("dd-MM-yyyy  hh:mm"));

        //Intentos fallidos de contraseña
        pos1=salida.indexOf("badPwdCount:")+12;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();
        ui->text_intentos->setText(usuario);

        pos1=salida.indexOf("userAccountControl:")+20;
        pos2=salida.indexOf("\n",pos1);
        usuario=salida.mid(pos1,pos2-pos1).trimmed();

        //useraccountcontrol=66048 	Enabled, Password Doesn't Expire
        //useraccountcontrol=66050 	Disabled, Password Doesn't Expire
        if (usuario=="66048") {
            ui->text_clave_caduca->setText("No caduca");
        }
        else{
            //ejecutamos la consulta Ldap para obtener el campo maxPwdAge
            proceso.start("ldapsearch -QLLL -o ldif-wrap=no -b \"dc = grx\" \"(&(objectClass=domain))\" maxPwdAge");
            proceso.waitForFinished();
            salida1=proceso.readAllStandardOutput();

    //        //consulta Ldap cargada en archivo
    //        QFile archivo1("/home/si_serafin/git/usuarios/ldap_dominio.txt");
    //        archivo1.open(QIODevice::ReadOnly);
    //        QTextStream text_archivo1(&archivo1);
    //        salida1=text_archivo1.readAll();

            pos1=salida1.indexOf("maxPwdAge:")+11;
            pos2=salida1.indexOf("\n",pos1);
            usuario=salida1.mid(pos1,pos2-pos1).trimmed();
            //qDebug() << usuario;
            usuario=QString::number(temp.toLongLong()-usuario.toLongLong());
            fecha.setSecsSinceEpoch((usuario.toLongLong()/10000000)-11644473600);
            ui->text_clave_caduca->setText(fecha.toString("dd-MM-yyyy  hh:mm"));
        }

        //ui->list_usuarios_2->setVisible(false);
    }

}
