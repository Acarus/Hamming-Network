#include "project.h"
#include <QDataStream>
#include <QFile>
#include <QList>

Project::Project() {
}

Project::Project(QString source_project_file) {
    Open(source_project_file);
}

bool Project::Create(std::vector<QImage> patterns) {
    // add patterns
    for(int i = 0; i < patterns.size(); i++)
        if(!net_.AddPattern(patterns.at(i)))
            return false;

    // training hamming network
    if(!net_.Train())
        return false;

    number_of_attempts_ = 0;
    number_of_errors_ = 0;
    ready_ = true;

    return true;
}

bool Project::Create(std::vector<QString> patterns) {
    std::vector<QImage> imgs;
    for(int i = 0; i < patterns.size(); i++)
        imgs.push_back(QImage(patterns.at(i)));

    return Create(imgs);
}

int Project::GetSimilarPatternNumber(QImage img) {
    if(!ready_)
        return -1;

    // icrement number of attempts and votes_
    number_of_attempts_++;
    votes_++;
    return net_.TestPattern(img);
}

int Project::GetSimilarPatternNumber(QString file_path) {
    if(!ready_)
        return -1;

    // icrement number of attempts and vots
    number_of_attempts_++;
    votes_++;
    return net_.TestPattern(file_path);
}

QImage Project::GetSimilarPattern(QImage img) {
    if(!ready_)
        return QImage();

    int number = net_.TestPattern(img);
    if(number < 0)
        return QImage();

    // icrement number of attempts and votes_
    number_of_attempts_++;
    votes_++;
    return net_.GetPattern(number);
}

QImage Project::GetSimilarPattern(QString file_path) {
    QImage img(file_path);
    return GetSimilarPattern(img);
}

int Project::GetAttemptsNumber() {
    return number_of_attempts_;
}

int Project::GetErrorsNumber() {
    return number_of_errors_;
}

bool Project::Open(QString source_project_file) {
    QFile file(source_project_file);
    if(!file.open(QIODevice::ReadOnly))
        return false;

    QDataStream ds(&file);
    QList<QImage> patterns;
    ds >> patterns;

    // add patterns
    for(int i = 0; i < patterns.size(); i++)
        if(!net_.AddPattern(patterns.at(i)))
            return false;

    // training hamming network
    if(!net_.Train())
        return false;

    ds >> number_of_attempts_;
    ds >> number_of_errors_;

    ready_ = true;
    project_file_path_ = source_project_file;

    file.close();

    return true;
}

NetworkInformation Project::GetNetworkInformation() {
    return net_.GetInformation();
}

bool Project::SaveAs(QString project_path) {
    QFile file(project_path);
    if(!file.open(QIODevice::WriteOnly))
        return false;

    // write serialized data to file
    QDataStream ds(&file);

    QList<QImage> patterns;
    for(int i = 0; i < net_.GetCountOfPatterns(); i++)
        patterns.push_back(net_.GetPattern(i));

    ds << patterns;
    ds << number_of_attempts_;
    ds << number_of_errors_;

    //file.flush();
    file.close();

    project_file_path_ = project_path;

    return true;
}

bool Project::Save() {
    return SaveAs(project_file_path_);
}

bool Project::IsReady() {
    return ready_;
}

bool Project::IsSaved() {
    return project_file_path_.size() > 0;
}

void Project::IncreaseError() {
    if(votes_) {
        number_of_errors_++;
        votes_--;
    }
}
