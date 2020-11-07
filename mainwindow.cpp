#define COLUMNS 7
#define SUSPENDER 1
#define MATAR 2
#define CONTINUAR 3

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTextStream>
#include <QFile>
#include <QDebug>

#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include  <time.h>
#include <sched.h>
#include <stdlib.h>
#include <sstream>
#include <sys/time.h>
#include <qresource.h>
#include <sys/resource.h>
#include <typeinfo>
#define _GNU_SOURCE
#include <sched.h>


using namespace std;

QString pid;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    populandoTabela();
    startTimer(1000);

    //ui->asd,SLOT(setValue(int));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::geraprocessos(){
    //cout << "passo1"<< endl;
    system("ps -e -o pid,uname,psr,pcpu,pmem,state,pri,comm --sort=-pcpu | sed 1d > /home/mateus/Documentos/STR/processos.txt");
    //cout << "passo2"<< endl;
    system("column -t /home/mateus/Documentos/STR/processos.txt  > /home/mateus/Documentos/STR/processos1.txt");
    //cout << "passo3"<< endl;
    system("tr -s \" \" < /home/mateus/Documentos/STR/processos1.txt > /home/mateus/Documentos/STR/processos2.txt" );


}

void MainWindow::populandoTabela()
{
    ui->tabela->clearContents();
    geraprocessos();
    //cout << "top -b n1 | grep \"^ \" | awk '{ printf(\"%-8s  %-8s  %-8s  %-8s  %-8s  %-8s  %-8s\n\", $1, $2, $8, $3, $9, $10, $12); }' >> /home/mateus/Documentos/STR/teste1.txt";
    //cout<<"estou aqui";
    //mudar caminho do arquivo
    //QString path = "/home/mateus/Documentos/STR/processos.txt";

    QString path = "/home/mateus/Documentos/STR/processos2.txt";
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }

    QTextStream in(&file);

    int rows = 0;
    float cpu1 = 0;
    float cpu2 = 0;
    while(!in.atEnd()) {
        QString line = in.readLine();

        ui->tabela->setRowCount(line.size());

        QStringList fields = line.split(" ");
        //qDebug() << fields;
        for (auto c=0; c<8; c++){
             ui->tabela->setItem( rows, c, new QTableWidgetItem(fields.at(c)));
             ui->tabela->update();
        }
        rows++;

        if (fields[2] == "0"){
            cpu1 += fields[3].toFloat();
        }else if(fields[2] == "1"){
            cpu2 += fields[3].toFloat();
        }




    }
    if(cpu1>=100){
        cpu1=100;
    }
    if(cpu2>=100){
        cpu2=100;
    }
    ui->asd2->setValue(cpu1);
    ui->asd->setValue(cpu2);
    ui->tabela->update();

    file.close();
}

void MainWindow::pegarPID()
{
    pid = ui->textEditPID->toPlainText();
    cout << typeid(pid).name() << '\n';

}

void MainWindow::executarComando(int idComando)
{
    pegarPID();
    QString comando;

    switch (idComando) {
    case MATAR:
        kill(pid.toInt(),SIGKILL);
        break;
    case SUSPENDER:
        kill(pid.toInt(),SIGSTOP);
        break;
    case CONTINUAR:
        kill(pid.toInt(),SIGCONT);
        break;
    default:
        break;
    }

    //QByteArray comandoConvertido = comando.toLocal8Bit();
    //const char *comandoFinal = comandoConvertido.data();

    //system(comandoFinal);
    //populandoTabela();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(ui->textEditFiltro->toPlainText().isEmpty()){
        system("pstree > processos.txt");
        cout<<"ESTOU AQUI"<<endl;
        populandoTabela();
    }else{
        system("pstree > processos.txt");
        populandoTabela2();
    }
}

void MainWindow::suspender()
{
    executarComando(SUSPENDER);
}

void MainWindow::continuar()
{
    executarComando(CONTINUAR);
}

void MainWindow::matar()
{
    executarComando(MATAR);
}

int MainWindow::alterarCPU()
{
    pegarPID();
    int novaCPU;
    novaCPU = ui->comboBoxCPU->currentText().toInt();
    novaCPU -= 1;
    cpu_set_t  mask;
        CPU_ZERO(&mask);


        CPU_SET(novaCPU, &mask);    // alocar na CPU 0

        //CPU_SET(1, &mask);
        //CPU_SET(2, &mask);
        //CPU_SET(3, &mask);

        int result = sched_setaffinity(pid.toInt(), sizeof(mask), &mask);  // 0 --> Aqui é no próprio processo. No Geral deve ser o PID


}

void MainWindow::mudarPrioridade()
{
    QString novaPrioridade;
    QString comando;

    novaPrioridade = ui->comboBoxPrioridade->currentText();
    pegarPID();

    //comando = " renice " + novaPrioridade + " -p " + pid;

    //QByteArray comandoConvertido = comando.toLocal8Bit();
    //const char *comandoFinal = comandoConvertido.data();

    //system(comandoFinal);

    setpriority(PRIO_PROCESS, pid.toInt() , novaPrioridade.toInt());

    //qDebug() << novaPrioridade;
}

void MainWindow::pegarPIDTabela(QModelIndex *i)
{

    qDebug() << i->row();
    qDebug() << "clicou";
}

void MainWindow::filtro()
{
     QString textoFiltro = ui->textEditFiltro->toPlainText();

     QString comando;
     comando = " ps -auf | grep " + textoFiltro;

     QByteArray comandoConvertido = comando.toLocal8Bit();
     const char *comandoFinal = comandoConvertido.data();

     system(comandoFinal);

     populandoTabela();
}

void MainWindow::copiaTexto()
{
    //QString a = ui->tabela->item(ui);

}


void MainWindow::geraprocessos2(){

    QString comando;
    QString a = ui->textEditFiltro->toPlainText();
    comando = " ps -e -o pid,uname,psr,pcpu,pmem,state,pri,comm --sort=-pcpu | sed 1d | grep "+ a + " > /home/mateus/Documentos/STR/processosa.txt";
    qDebug() << a;

    QByteArray comandoConvertido = comando.toLocal8Bit();
    const char *comandoFinal = comandoConvertido.data();

    system(comandoFinal);
    //cout << "passo1"<< endl;
    //system("ps -e -o pid,uname,psr,pcpu,pmem,state,pri,comm --sort=-pcpu | sed 1d > /home/mateus/Documentos/STR/processos.txt");
    //cout << "passo2"<< endl;
    system("column -t /home/mateus/Documentos/STR/processosa.txt  > /home/mateus/Documentos/STR/processosa1.txt");
    //cout << "passo3"<< endl;
    system("tr -s \" \" < /home/mateus/Documentos/STR/processosa1.txt > /home/mateus/Documentos/STR/processosa2.txt" );

}

void MainWindow::populandoTabela2()
{
    ui->tabela->clearContents();
    geraprocessos2();
    //cout << "top -b n1 | grep \"^ \" | awk '{ printf(\"%-8s  %-8s  %-8s  %-8s  %-8s  %-8s  %-8s\n\", $1, $2, $8, $3, $9, $10, $12); }' >> /home/mateus/Documentos/STR/teste1.txt";
    //cout<<"estou aqui";
    //mudar caminho do arquivo
    //QString path = "/home/mateus/Documentos/STR/processos.txt";

    QString path = "/home/mateus/Documentos/STR/processosa2.txt";
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }

    QTextStream in(&file);

    int rows = 0;
    float cpu1 = 0;
    float cpu2 = 0;
    while(!in.atEnd()) {
        QString line = in.readLine();

        ui->tabela->setRowCount(line.size());

        QStringList fields = line.split(" ");

        for (auto c=0; c<8; c++){
             ui->tabela->setItem( rows, c, new QTableWidgetItem(fields.at(c)));
             ui->tabela->update();
        }


        if (fields[2] == "0"){
            cpu1 += fields[3].toFloat();
        }else if(fields[2] == "1"){
            cpu2 += fields[3].toFloat();
        }

        rows++;
    }
    if(cpu1>=100){
        cpu1=100;
    }
    if(cpu2>=100){
        cpu2=100;
    }
    ui->asd2->setValue(cpu1);
    ui->asd->setValue(cpu2);
    ui->tabela->update();

    file.close();
}



