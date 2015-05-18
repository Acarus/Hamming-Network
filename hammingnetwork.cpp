#include "hammingnetwork.h"
#include <QImage>
#include <QColor>

HammingNetwork::HammingNetwork(int width, int height) :img_height_(height), img_width_(width) {
}

void HammingNetwork::LoadInputFromFile(QString filePath) {
    QImage img(filePath);
    int k = 0;
    for(int i = 0; i < img_height_; i++)
        for(int j = 0; j < img_width_; j++) {
            QColor col = img.pixel(j, i);
            layer_0_[k++] = col.red() ? HI : LO;
        }
}

void HammingNetwork::Training(std::vector<QString> files) {
    for(int fi = 0; fi < files.size(); fi++) {
        // load training input and ideal output
        LoadInputFromFile(files.at(fi));
        // hamming layer traning rule
        for(int i = 0; i < size_layer_0_; i++)
            weight_hamming_[fi][i] = layer_0_[i] * 0.5;

    }
}

double HammingNetwork::StateHamming(int num) {
    double s = 0.0;
    for(int i = 0; i < size_layer_0_; i++)
        s += layer_0_[i] * weight_hamming_[num][i];
    return s;
}

void HammingNetwork::StepHamming() {
    for(int i = 0; i < size_layer_1_; i++)
        layer_1_[i] = StateHamming(i);
}

bool HammingNetwork::StableHopfield() {
    return layer_1_ == layer_1_prev_;
}

double HammingNetwork::HopfieldActivateFunction(double x) {
    return x > T ? x : T;
}

double HammingNetwork::StateHopfield(int num) {
    double s = 0.0;
    for(int i = 0; i < size_layer_1_; i++)
        if(i != num)
            s += layer_1_prev_[i];
    return layer_1_prev_[num] - braking_ * s;
}

void HammingNetwork::StepHopfield() {
    layer_1_prev_ = layer_1_;
    for(int i = 0; i < size_layer_1_; i++)
        layer_1_[i] = HopfieldActivateFunction(StateHopfield(i));
}

void HammingNetwork::Step() {
    StepHamming();
    for(int i = 0; i < MAX_ITER; i++) {
        StepHopfield();
        if(StableHopfield())
            break;
    }
}
