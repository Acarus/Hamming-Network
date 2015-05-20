#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QIcon>
#include <QToolBar>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QFormLayout>

#include "project.h"

namespace Ui {
class Window;
}

class Window : public QMainWindow {
    Q_OBJECT

    QAction* new_project_a_;
    QAction* open_project_a_;
    QAction* save_project_a_;
    QAction* save_project_as_a_;
    QAction* exit_a_;
    QAction* multi_test_a_;

    QToolBar* tool_bar_;

    QMenuBar* menu_bar_;
    QMenu* menu_file_;

    Project project_;

    Ui::Window *ui;

    void ShowAll();
    void HideAll();

    QString prev_image_;

public:

    const int DEAFAULT_WIDTH = 600;
    const int DEFAULT_HEIGHT = 400;

    const QString PROJECT_EXT = ".proj";

    Window(QWidget *parent = 0);
    ~Window();


private slots:
    void OpenProject();
    void NewProject();
    void SaveProject();
    void SaveProjectAs();
    void Test();

    void on_load_btn_clicked();
    void on_pushButton_clicked();
};

#endif WINDOW_H
