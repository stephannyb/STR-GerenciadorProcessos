#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void geraprocessos();
    void geraprocessos2();
    void populandoTabela();
    void populandoTabela2();
    void pegarPID();
    void executarComando(int idComando);
    void timerEvent(QTimerEvent *event);
    void porcentagem();
    void porcentagem1();

public slots:
     void suspender();
     void continuar();
     void matar();
     int alterarCPU();
     void mudarPrioridade();
     void pegarPIDTabela(QModelIndex *index);
     void filtro();
     void copiaTexto();



private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
