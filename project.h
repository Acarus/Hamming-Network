#ifndef PROJECT_H
#define PROJECT_H

#include "hammingnetwork.h"

class Project {
private:
    HammingNetwork net_;
    int number_of_attempts_;
    int number_of_errors_;
    QString project_file_path_;

public:
    Project();

    /**
     * @brief Project
     * @param source_file
     */
    Project(QString source_file);


    /**
     * @brief Create
     * @param source_file
     * @param patterns
     * @return
     */
    bool Create(QString source_file, std::vector<QString> patterns);

    /**
     * @brief Create
     * @param source_file
     * @param patterns
     * @return
     */
    bool Create(QString source_file, std::vector<QImage> patterns);

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
    bool Open(QString source_file);

    /**
     * @brief Save
     * @return
     */
    bool Save();
};

#endif // PROJECT_H
