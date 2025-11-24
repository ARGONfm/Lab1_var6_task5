#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <cstddef>
#include <algorithm>
#include <utility>

template <typename T>
class GrayscaleImage {
private:
    T* data_;
    std::size_t rows_;
    std::size_t cols_;

public:
    GrayscaleImage(std::size_t rows = 0, std::size_t cols = 0);
    ~GrayscaleImage();

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }
};

#endif