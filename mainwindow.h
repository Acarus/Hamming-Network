#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIcon>
#include <QToolBar>
#include <QMenu>
#include <QAction>
#include <QMenuBar>

#include "project.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

    QAction* new_project_a_;
    QAction* open_project_a_;
    QAction* save_project_a_;
    QAction* save_project_as_a_;
    QAction* exit_a_;

    QToolBar* tool_bar_;

    QMenuBar* menu_bar_;
    QMenu* menu_file_;

    Project project_;

    void ShowSaveButtons();
    void HideSaveButtons();

public:

    const int DEAFAULT_WIDTH = 800;
    const int DEFAULT_HEIGHT = 600;

    const QString PROJECT_EXT = ".proj";

    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void OpenProject();
    void NewProject();
    void SaveProject();
    void SaveProjectAs();
};

#endif // MAINWINDOW_H
