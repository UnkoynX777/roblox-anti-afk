#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsDropShadowEffect>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    configurarJanela();
    configurarNotificacao();
    configurarAnimacoes();
    configurarTimers();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::configurarJanela()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::Window, true);
}

void MainWindow::configurarNotificacao()
{
    notificacao = new QLabel(this);
    notificacao->setFixedSize(150, 30);
    notificacao->setAlignment(Qt::AlignCenter);
    notificacao->setStyleSheet("color: white; border-radius: 6px;");
    notificacao->hide();

    auto *sombra = new QGraphicsDropShadowEffect(this);
    sombra->setBlurRadius(8);
    sombra->setOffset(0, 2);
    notificacao->setGraphicsEffect(sombra);

    efeitoOpacidade = new QGraphicsOpacityEffect(this);
    efeitoOpacidade->setOpacity(0.0);
    notificacao->setGraphicsEffect(efeitoOpacidade);
}

void MainWindow::configurarAnimacoes()
{
    animSlide = new QPropertyAnimation(notificacao, "pos", this);
    animSlide->setDuration(300);
    animSlide->setEasingCurve(QEasingCurve::OutCubic);

    animFade = new QPropertyAnimation(efeitoOpacidade, "opacity", this);
    animFade->setDuration(300);
}

void MainWindow::configurarTimers()
{
    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, &QTimer::timeout, [this]()
            {
        if (tempoNotificacao.hasExpired(NOTIFICATION_DISPLAY_TIME_MS)) {
            timer->stop();
            animFade->stop();
            animFade->setStartValue(1.0);
            animFade->setEndValue(0.0);
            animFade->start();
        } });
    connect(animFade, &QPropertyAnimation::finished, this, [this]()
            {
        if (efeitoOpacidade->opacity() == 0.0) {
            notificacao->hide();
        } });
    antAfkTimer = new QTimer(this);
    connect(antAfkTimer, &QTimer::timeout, this, &MainWindow::executarAntAfk);
    rodando = false;
}

void MainWindow::on_btnFechar_clicked()
{
    close();
}

void MainWindow::on_btnMinimizar_clicked()
{
    showMinimized();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        posicaoInicial = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPosition().toPoint() - posicaoInicial);
        event->accept();
    }
}

void MainWindow::mostrarNotificacao(const QString &mensagem, const QColor &corFundo)
{
    timer->stop();
    animFade->stop();
    animSlide->stop();
    efeitoOpacidade->setOpacity(0.0);
    notificacao->setVisible(true);
    notificacao->setText(mensagem);
    notificacao->setStyleSheet(QString("background-color: %1; color: white; border-radius: 6px;").arg(corFundo.name()));

    int x = (width() - notificacao->width()) / 2;
    int yFinal = height() - notificacao->height() - 10;
    int yInicial = height();
    notificacao->move(x, yInicial);

    animSlide->setStartValue(QPoint(x, yInicial));
    animSlide->setEndValue(QPoint(x, yFinal));
    animSlide->start();

    animFade->setStartValue(0.0);
    animFade->setEndValue(1.0);
    animFade->start();

    tempoNotificacao.restart();
    timer->start();
}

void MainWindow::executarAntAfk()
{
    if (!rodando)
    {
        antAfkTimer->stop();
        return;
    }
    if (!WindowsUtils::isWindowFocused(QStringLiteral("RobloxPlayerBeta.exe")))
    {
        int delayMs = (QRandomGenerator::global()->bounded(ANT_AFK_MAX_DELAY_SECONDS - ANT_AFK_MIN_DELAY_SECONDS + 1) + ANT_AFK_MIN_DELAY_SECONDS) * 1000;
        antAfkTimer->start(delayMs);
        return;
    }
    const WORD scancodes[] = {0x1E, 0x1F, 0x11, 0x20};
    int indice = QRandomGenerator::global()->bounded(4);

    INPUT inputs[4] = {};
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wScan = 0x39;
    inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wScan = scancodes[indice];
    inputs[1].ki.dwFlags = KEYEVENTF_SCANCODE;

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wScan = scancodes[indice];
    inputs[2].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wScan = 0x39;
    inputs[3].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

    SendInput(4, inputs, sizeof(INPUT));

    int delayMs = (QRandomGenerator::global()->bounded(60) + 1) * 1000;
    antAfkTimer->start(delayMs);
}

void MainWindow::on_btnStart_clicked()
{
    if (!rodando)
    {
        if (WindowsUtils::isProcessRunning(QStringLiteral("RobloxPlayerBeta.exe")))
        {
            mostrarNotificacao("Running!", QColor(40, 167, 69));
            ui->btnStart->setText("Stop");
            rodando = true;
            int delayMs = (QRandomGenerator::global()->bounded(ANT_AFK_MAX_DELAY_SECONDS - ANT_AFK_MIN_DELAY_SECONDS + 1) + ANT_AFK_MIN_DELAY_SECONDS) * 1000;
            antAfkTimer->start(delayMs);
        }
        else
        {
            mostrarNotificacao("OPEN ROBLOX!", QColor(220, 53, 69));
        }
    }
    else
    {
        mostrarNotificacao("Stopped!", QColor(255, 193, 7));
        ui->btnStart->setText("Start");
        rodando = false;
        antAfkTimer->stop();
    }
}
