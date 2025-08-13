#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QElapsedTimer>
#include "windowsutils.h"
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>
#include <QGraphicsOpacityEffect>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void on_btnFechar_clicked();
    void on_btnMinimizar_clicked();
    void on_btnStart_clicked();

private:
    Ui::MainWindow *ui;
    QPoint posicaoInicial;
    QLabel *notificacao;
    QPropertyAnimation *animSlide;
    QPropertyAnimation *animFade;
    QTimer *timer;
    QGraphicsOpacityEffect *efeitoOpacidade;
    QElapsedTimer tempoNotificacao;
    bool rodando = false;
    QTimer *antAfkTimer = nullptr;

    void configurarJanela();
    void configurarNotificacao();
    void configurarAnimacoes();
    void configurarTimers();
    void executarAntAfk();
    void mostrarNotificacao(const QString &mensagem, const QColor &corFundo);
};

#endif

#define NOTIFICATION_DISPLAY_TIME_MS 4000
#define ANT_AFK_MIN_DELAY_SECONDS 1
#define ANT_AFK_MAX_DELAY_SECONDS 60