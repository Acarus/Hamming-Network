#include "hammingnetwork.h"
#include <QColor>
#include <QDebug>
#include <algorithm>

HammingNetwork::HammingNetwork(int width, int height) :img_height_(height), img_width_(width), size_layer_0_(img_width_ * img_height_), size_layer_1_(0) {
}

bool HammingNetwork::LoadInputFromFile(QString filePath) {
    QImage img(filePath);
    return LoadInputToLayer(img);
}

bool HammingNetwork::LoadInputToLayer(QImage img) {
    if(img.width() != img_width_ || img.height() != img_height_)
        return false;

    int k = 0;
    for(int i = 0; i < img_height_; i++)
        for(int j = 0; j < img_width_; j++) {
            if(k > size_layer_0_ - 1)
                return false;

            QColor col = img.pixel(j, i);
            // determine whether it is black or white
            layer_0_[k++] = col.red() ? HI : LO;
        }

    return true;
}

bool HammingNetwork::Train() {
    // allocate memory
    layer_0_.assign(size_layer_0_, 0);
    layer_1_.assign(size_layer_1_, 0);
    layer_1_prev_.assign(size_layer_1_, 0);
    weight_hamming_.assign(size_layer_1_, std::vector<double>(size_layer_0_, 0));

    // define braking coefficient
    braking_ = 1.0 / (size_layer_1_ * 2.0);

    for(int fi = 0; fi < patterns_images_.size(); fi++) {
        if(!LoadInputToLayer(patterns_images_.at(fi)))
            return false;

        // hamming layer traning rule
        for(int i = 0; i < size_layer_0_; i++)
            weight_hamming_[fi][i] = layer_0_[i] * 0.5;
    }

    return true;
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

int HammingNetwork::TestPattern(QImage img) {
    if(!LoadInputToLayer(img))
        return -1;

    layer_1_.assign(size_layer_1_, 0);

    StepHamming();
    for(int i = 0; i < MAX_ITER; i++) {
        StepHopfield();
        if(StableHopfield())
            break;
    }

    return std::distance(layer_1_.begin(), std::max_element(layer_1_.begin(), layer_1_.end()));
}

bool HammingNetwork::AddPattern(QString filePath) {
    QImage img(filePath);
    if(img.width() != img_width_ || img.height() != img_height_)
        return false;

    return AddPattern(img);
}

bool HammingNetwork::AddPattern(QImage img) {
    patterns_images_.push_back(img);
    size_layer_1_++;
    return true;
}

int HammingNetwork::GetZeroLayerSize() {
    return size_layer_0_;
}

int HammingNetwork::GetFirstLayerSize() {
    return size_layer_1_;
}

int HammingNetwork::GetCountOfPatterns() {
    return size_layer_1_;
}

QImage HammingNetwork::GetPattern(int index) {
    if(index > patterns_images_.size() || index < 0)
        return QImage();

    return patterns_images_.at(index);
}
