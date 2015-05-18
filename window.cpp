#include "window.h"
#include "ui_window.h"
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QApplication>
#include "window.h"

#include <QGridLayout>


void Window::ShowSaveButtons() {
    save_project_a_->setEnabled(true);
    save_project_as_a_->setEnabled(true);
}

void Window::HideSaveButtons() {
    save_project_a_->setEnabled(false);
    save_project_as_a_->setEnabled(false);
}

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);

    new_project_a_ = new QAction(this);
    new_project_a_->setIcon(QIcon(":/img/menu/resources/New-Project.ico"));
    new_project_a_->setToolTip(tr("Create new project"));
    new_project_a_->setText(tr("New"));
    new_project_a_->setIconVisibleInMenu(false);
    new_project_a_->setShortcut(Qt::Key_N | Qt::CTRL);
    connect(new_project_a_, SIGNAL(triggered()), this, SLOT(NewProject()));

    open_project_a_ = new QAction(this);
    open_project_a_->setIcon(QIcon(":/img/menu/resources/Open-Project.ico"));
    open_project_a_->setToolTip(tr("Open project"));
    open_project_a_->setText(tr("Open"));
    open_project_a_->setIconVisibleInMenu(false);
    open_project_a_->setShortcut(Qt::Key_O | Qt::CTRL);
    connect(open_project_a_, SIGNAL(triggered()), this, SLOT(OpenProject()));

    save_project_a_ = new QAction(this);
    save_project_a_->setIcon(QIcon(":/img/menu/resources/Save-Project.ico"));
    save_project_a_->setToolTip(tr("Save project"));
    save_project_a_->setText(tr("Save"));
    save_project_a_->setEnabled(false);
    save_project_a_->setIconVisibleInMenu(false);
    save_project_a_->setShortcut(Qt::Key_S | Qt::CTRL);
    connect(save_project_a_, SIGNAL(triggered()), this, SLOT(SaveProject()));

    save_project_as_a_ = new QAction(this);
    save_project_as_a_->setIcon(QIcon(":/img/menu/resources/Save-Project-As.ico"));
    save_project_as_a_->setToolTip("Save project as");
    save_project_as_a_->setText("Save as");
    save_project_as_a_->setEnabled(false);
    save_project_as_a_->setIconVisibleInMenu(false);
    save_project_as_a_->setShortcut(Qt::Key_F12);
    connect(save_project_as_a_, SIGNAL(triggered()), this, SLOT(SaveProjectAs()));

    exit_a_ = new QAction(this);
    exit_a_->setText(tr("Exit"));
    connect(exit_a_, SIGNAL(triggered()), qApp, SLOT(quit()));

    // create tool bar
    tool_bar_ = new QToolBar(this);
    tool_bar_->addAction(new_project_a_);
    tool_bar_->addAction(open_project_a_);
    tool_bar_->addAction(save_project_a_);
    tool_bar_->addAction(save_project_as_a_);
    addToolBar(tool_bar_);


    // file menu
    menu_file_ = new QMenu(this);
    menu_file_->setTitle(tr("File"));
    menu_file_->addAction(new_project_a_);
    menu_file_->addSeparator();
    menu_file_->addAction(open_project_a_);
    menu_file_->addSeparator();
    menu_file_->addAction(save_project_a_);
    menu_file_->addAction(save_project_as_a_);
    menu_file_->addAction(exit_a_);

    menu_bar_ = new QMenuBar(this);
    menu_bar_->addMenu(menu_file_);

    menu_bar_->setNativeMenuBar(false);
    setMenuBar(menu_bar_);

    // open window on center of the screen
    QDesktopWidget desktop;
    int desktop_width = desktop.width();
    int desktop_height = desktop.height();
    setGeometry(desktop_width/2 - DEAFAULT_WIDTH/2, desktop_height/2 - DEFAULT_HEIGHT/2, DEAFAULT_WIDTH, DEFAULT_HEIGHT);

}

Window::~Window()
{
    delete ui;
}

void Window::OpenProject() {
    QString filter;
    filter.sprintf(tr("Project file (*%s)").toStdString().c_str(), PROJECT_EXT.toStdString().c_str());
    QString file_path = QFileDialog::getOpenFileName(this, tr("Open project"), QDir::currentPath(), filter);

    if(file_path.size() > 0) {
        if(!project_.Open(file_path)) {
            // can't open project file
            QString format = tr("Can't open project \"%s\"!!!");
            QMessageBox::critical(0, tr("Fail"), QString().sprintf(format.toStdString().c_str(), file_path.toStdString().c_str()));
        }  else {
            qDebug() << "opened!!!";
            qDebug() << file_path;
            ShowSaveButtons();
        }
    }
}

void Window::NewProject() {
    QMessageBox::information(this, tr("New Project"), tr("You need to choose reference images for recognition"));
    QList<QString> patterns = QFileDialog::getOpenFileNames(this, tr("Select reference images"), QDir::currentPath(), tr("Images (*.png *.bmp *.jpg)"));

    if(patterns.size() > 0) {
        // check images size
        std::vector<QImage> data;
        int width;
        int height;
        for(int i = 0; i < patterns.size(); i++) {
            QImage current_image(patterns.at(i));
            if(i == 0) {
                width = current_image.width();
                height = current_image.height();
            }

            if(width != current_image.width() || height != current_image.height()) {
                // images must have the same size
                QMessageBox::information(this, tr("New project"), tr("Images must have the same size!"));
                return;
            }

            data.push_back(current_image);
        }

        // create hamming network
        project_.Resize(width, height);
        if(!project_.Create(data)) {
            QMessageBox::critical(this, tr("Error"), tr("Can't create new project!"));
        } else {
            qDebug() << "created new project";
            ShowSaveButtons();
        }
    }

}

void Window::SaveProject() {
    if(!project_.IsSaved()) {
        SaveProjectAs();
    } else {
        project_.Save();
    }
}

void Window::SaveProjectAs() {
    QString filter;
    filter.sprintf(tr("Project file (*%s)").toStdString().c_str(), PROJECT_EXT.toStdString().c_str());
    QString file_path = QFileDialog::getSaveFileName(this, tr("Save as"), QDir::currentPath(), filter);
    if(file_path.size() > 0) {
        // check file extension
        if(file_path.indexOf(PROJECT_EXT) < 0 || file_path.indexOf(PROJECT_EXT) != file_path.size() - PROJECT_EXT.size()) {
            file_path += PROJECT_EXT;
        }

        qDebug() << "save path = " << file_path;
        project_.SaveAs(file_path);
    }
}

void Window::LoadImageFromFile() {

}
