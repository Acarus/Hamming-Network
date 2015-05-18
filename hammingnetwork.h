#ifndef HAMMINGNETWORK_H
#define HAMMINGNETWORK_H

#include <vector>
#include <QString>
#include <QImage>

class HammingNetwork {
private:
    // activation function parameters
    const int LO = -1; // black color
    const int HI = 1; // white color
    const int T = 0; // threshold for the activation function

    // max number iteration of Hopfield layer
    const int MAX_ITER = 700;

    // input pattern size
    int img_height_;
    int img_width_;

    // network size
    int size_layer_0_;
    int size_layer_1_;

    // braking coefficient
    double braking_;

    std::vector<double> layer_0_;
    std::vector<double> layer_1_;
    std::vector<double> layer_1_prev_;

    // weidght matrix hamming
    std::vector< std::vector<double> > weight_hamming_;

    // save images of patterns
    std::vector<QImage> patterns_images_;

    /**
     * @brief StateHamming
     * @return
     */
    double StateHamming(int);

    /**
     * @brief StepHamming
     */
    void StepHamming();

    /**
     * @brief StableHopfield
     * @return
     */
    bool StableHopfield();

    /**
     * @brief HopfieldActivateFunction
     * @return
     */
    double HopfieldActivateFunction(double);

    /**
     * @brief StateHopfield
     * @return
     */
    double StateHopfield(int);

    /**
     * @brief StepHopfield
     */
    void StepHopfield();

    /**
     * @brief LoadInputToLayer
     * @param img
     * @return
     */
    bool LoadInputToLayer(QImage img);

    /**
     * @brief LoadInputFromFile
     * @param filePath
     * @return
     */
    bool LoadInputFromFile(QString filePath);

public:
    HammingNetwork(int width = 17, int height = 31);

    /**
     * @brief AddPattern
     * @return
     */
    bool AddPattern(QString);

    /**
     * @brief AddPattern
     * @return
     */
    bool AddPattern(QImage);

    /**
     * @brief Train
     * @return
     */
    bool Train();

    /**
     * @brief TestPattern
     * @param img
     * @return
     */
    int TestPattern(QImage img);

    /**
     * @brief TestPattern
     * @param filePath
     * @return
     */
    int TestPattern(QString filePath);

    /**
     * @brief GetPattern
     * @param index
     * @return
     */
    QImage GetPattern(int index);

    /**
     * @brief GetZeroLayerSize
     * @return
     */
    int GetZeroLayerSize();

    /**
     * @brief GetFirstLayerSize
     * @return
     */
    int GetFirstLayerSize();

    /**
     * @brief GetCountOfPatterns
     * @return
     */
    int GetCountOfPatterns();
};

#endif // HAMMINGNETWORK_H
