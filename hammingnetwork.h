#ifndef HAMMINGNETWORK_H
#define HAMMINGNETWORK_H

#include <vector>
#include <QString>

class HammingNetwork {
private:
    /* input pattern size */
    int img_height_;
    int img_width_;

    /* network size */
    int size_layer_0_;
    int size_layer_1_;

    // braking coefficient
    double braking_; //(1.0/(SIZE_LAYER_1*2.0))

    std::vector<double> layer_0_;
    std::vector<double> layer_1_;
    std::vector<double> layer_1_prev_;

    // weidght matrix hamming
    std::vector< std::vector<double> > weight_hamming_;

public:
    HammingNetwork(int width = 17, int height = 31);

    /* activation function parameters */
    const int LO =  -1;
    const int HI = 1;
    const int T = 0;

    // max number iteration of Hopfield layer
    const int MAX_ITER = 700;

    /**
     * @brief loadInputFromFile
     * @param filePath
     */
    void LoadInputFromFile(QString filePath);

    /**
     * @brief training Training hamming layer
     * @param files list which contains path to patterns
     **/
    void Training(std::vector<QString> files);

    /**
     * @brief state_hamming
     * @return state of neuron num
     */
    double StateHamming(int);

    /**
     * @brief step_hamming
     */
    void StepHamming();

    /**
     * @brief stable_hopfield
     * @return
     */
    bool StableHopfield();

    /**
     * @brief activate_function_hopfield
     * @return
     */
    double HopfieldActivateFunction(double);

    /**
     * @brief state_hopfield
     * @return
     */
    double StateHopfield(int);

    /**
     * @brief step_hopfield
     */
    void StepHopfield();

    /**
     * @brief step
     */
    void Step();

};

#endif // HAMMINGNETWORK_H
