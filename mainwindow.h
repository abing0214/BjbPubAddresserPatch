#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDir>
#include <QFileDialog>
#include <QTextCodec>
#include <QFile>
#include <QTextStream>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantList>
#include <QDebug>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QString m_str_name;
    QString m_str_addr;
    QString m_str_zipcode;

    bool Process(QString str_htm_file);


};

#endif // MAINWINDOW_H
