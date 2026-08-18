#ifndef GUI_H
#define GUI_H

#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QApplication>
#include <QAbstractItemView>
#include <QFile>
#include <QFrame>
#include <QFutureWatcher>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QtConcurrent>
#include <algorithm>

#include "../class.h"

class MainWindow : public QMainWindow
{
public:
    MainWindow();

private:

    QLabel* titleLabel;
    QLabel* directoryLabel;

    QPushButton* scanButton;
    QPushButton* backButton;
    QPushButton* deleteButton;

    QListWidget* fileList;

    fs::path currentDirectory;

    vector<Entry> currentEntries;

    void scanDirectory();
    void displayEntries();
    void deleteSelected();
};

#endif