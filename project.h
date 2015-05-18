#ifndef PROJECT_H
#define PROJECT_H

#include "hammingnetwork.h"

class Project {
private:
    HammingNetwork net_;
    int number_of_attempts_;
    int number_of_errors_;
    int votes_ = 0;
    QString project_file_path_;
    bool ready_ = false;

public:
    Project();

    /**
     * @brief Project
     * @param source_file
     */
    Project(QString source_project_file);


    /**
     * @brief Create
     * @param patterns
     * @return
     */
    bool Create(std::vector<QString> patterns);

    /**
     * @brief Create
     * @param patterns
     * @return
     */
    bool Create(std::vector<QImage> patterns);

    /**
     * @brief GetSimilarPatternNumber
     * @param file_path
     * @return
     */
    int GetSimilarPatternNumber(QString file_path);

    /**
     * @brief GetSimilarPatternNumber
     * @param img
     * @return
     */
    int GetSimilarPatternNumber(QImage img);

    /**
     * @brief GetSimilarPattern
     * @param file_path
     * @return
     */
    QImage GetSimilarPattern(QString file_path);

    /**
     * @brief GetSimilarPattern
     * @param img
     * @return
     */
    QImage GetSimilarPattern(QImage img);

    /**
     * @brief GetAttemptsNumber
     * @return
     */
    int GetAttemptsNumber();

    /**
     * @brief GetErrorsNumber
     * @return
     */
    int GetErrorsNumber();

    /**
     * @brief Open
     * @param source_file
     * @return
     */
    bool Open(QString source_project_file);

    /**
     * @brief SaveAs
     * @param project_path
     * @return
     */
    bool SaveAs(QString project_path);

    /**
     * @brief Save
     * @param project_path
     * @return
     */
    bool Save();

    /**
     * @brief GetNetworkInformation
     * @return
     */
    NetworkInformation GetNetworkInformation();

    /**
     * @brief ReadyToWork
     * @return
     */
    bool IsReady();

    /**
     * @brief IsSaved
     * @return
     */
    bool IsSaved();

    /**
     * @brief IncreaseError
     */
    void IncreaseError();

    /**
     * @brief Resize
     * @param width
     * @param height
     */
    void Resize(int width, int height);

};

#endif // PROJECT_H
