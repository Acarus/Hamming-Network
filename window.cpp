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

void Window::ShowAll() {
    save_project_a_->setEnabled(true);
    save_project_as_a_->setEnabled(true);
    multi_test_a_->setEnabled(true);
    centralWidget()->setEnabled(true);
}

void Window::HideAll() {
    save_project_a_->setEnabled(false);
    save_project_as_a_->setEnabled(false);
    multi_test_a_->setEnabled(false);
    centralWidget()->setEnabled(false);
}

Window::Window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Window) {
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
    save_project_a_->setIconVisibleInMenu(false);
    save_project_a_->setShortcut(Qt::Key_S | Qt::CTRL);
    connect(save_project_a_, SIGNAL(triggered()), this, SLOT(SaveProject()));

    save_project_as_a_ = new QAction(this);
    save_project_as_a_->setIcon(QIcon(":/img/menu/resources/Save-Project-As.ico"));
    save_project_as_a_->setToolTip("Save project as");
    save_project_as_a_->setText("Save as");
    save_project_as_a_->setIconVisibleInMenu(false);
    save_project_as_a_->setShortcut(Qt::Key_F12);
    connect(save_project_as_a_, SIGNAL(triggered()), this, SLOT(SaveProjectAs()));

    exit_a_ = new QAction(this);
    exit_a_->setText(tr("Exit"));
    connect(exit_a_, SIGNAL(triggered()), qApp, SLOT(quit()));

    multi_test_a_ = new QAction(this);
    multi_test_a_->setIcon(QIcon(":/img/menu/resources/test.ico"));
    multi_test_a_->setText(tr("Test"));
    multi_test_a_->setToolTip(tr("Test"));
    connect(multi_test_a_, SIGNAL(triggered()), this, SLOT(Test()));

    // create tool bar
    tool_bar_ = new QToolBar(this);
    tool_bar_->addAction(new_project_a_);
    tool_bar_->addAction(open_project_a_);
    tool_bar_->addAction(save_project_a_);
    tool_bar_->addAction(save_project_as_a_);
    tool_bar_->addAction(multi_test_a_);
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

    centralWidget()->setEnabled(false);
    setWindowTitle(tr("Hamming Network"));
    HideAll();
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
            ShowAll();
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
            ShowAll();
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



void Window::on_load_btn_clicked() {
    QString file_path = QFileDialog::getOpenFileName(this, tr("Input"), QDir::currentPath(), tr("Images (*.png *.bmp *.jpg)"));
    if(file_path.size() > 0) {
        qDebug() << file_path;
        QImage img = project_.GetSimilarPattern(file_path);
        ui->input->setPixmap(QPixmap(file_path));
        ui->output->setPixmap(QPixmap::fromImage(img));
        prev_image_ = file_path;
    }
}

void Window::on_pushButton_clicked() {
    if(prev_image_.size() > 0) {
        QImage img = project_.GetSimilarPattern(prev_image_);
        ui->input->setPixmap(QPixmap(prev_image_));
        ui->output->setPixmap(QPixmap::fromImage(img));
    }
}

void Window::Test() {
    QList<QString> files = QFileDialog::getOpenFileNames(this, tr("Test images"), QDir::currentPath(), tr("Images (*.png *.bmp *.jpg)"));
    if(files.size() > 0) {
       QString to_save = QFileDialog::getExistingDirectory(this, tr("Select folder"), QDir::currentPath());
       QDir dir(to_save);
       if(to_save.size() > 0) {
           for(int i = 0; i < files.size(); i++) {
               QImage img_inp(files.at(i));
               QImage img_ans = project_.GetSimilarPattern(files.at(i));
               img_inp.save(dir.absoluteFilePath(QString().sprintf("input_%d.png", i + 1)));
               img_ans.save(dir.absoluteFilePath(QString().sprintf("output_%d.png", i + 1)));
            }

           // Create html page
           QFile file(dir.absoluteFilePath("index.html"));
           if(file.open(QIODevice::WriteOnly)) {;
               QTextStream ts(&file);
               ts << "<html><head><title>Test result</title></head><body><span style=\"color:gray; font-size: 24px;\">Test result: </span><p><table><tr><th>IN</th><th>OUT</th></tr>";

               for(int i = 0; i < files.size(); i++) {
                   ts << "<tr>";
                   ts << QString().sprintf("<th><img src = 'input_%d.png'/></th><th><img src = 'output_%d.png'/></th>", i + 1, i + 1);
                   ts << "</tr>";
               }
               ts << "</p></table></body></html>";
           }

           file.close();
           QMessageBox::information(this, tr("Message"), tr("Success!"));
       }
    }
 }
