#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <cstddef>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include <ostream>

template <typename T>
class GrayscaleImage {
private:
    T* data_;
    std::size_t rows_;
    std::size_t cols_;

public:
    // Конструктор, деструктор, Rule of Five
    GrayscaleImage(std::size_t rows = 0, std::size_t cols = 0);
    
        : data_(rows && cols ? new T[rows * cols] : nullptr)
        , rows_(rows), cols_(cols) {}

    ~GrayscaleImage() { delete[] data_; }

    // Копирующий конструктор
    GrayscaleImage(const GrayscaleImage& other)
        : data_(nullptr), rows_(other.rows_), cols_(other.cols_) {
        if (rows_ * cols_) {
            data_ = new T[rows_ * cols_];
            std::copy(other.data_, other.data_ + rows_ * cols_, data_);
        }
    }

    // Копирующее присваивание
    GrayscaleImage& operator=(const GrayscaleImage& other) {
        if (this != &other) {
            GrayscaleImage tmp(other);
            std::swap(data_, tmp.data_);
            std::swap(rows_, tmp.rows_);
            std::swap(cols_, tmp.cols_);
        }
        return *this;
    }

    // Перемещающий конструктор
    GrayscaleImage(GrayscaleImage&& other) noexcept
        : data_(other.data_), rows_(other.rows_), cols_(other.cols_) {
        other.data_ = nullptr;
        other.rows_ = other.cols_ = 0;
    }

    // Перемещающее присваивание
    GrayscaleImage& operator=(GrayscaleImage&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            rows_ = other.rows_;
            cols_ = other.cols_;
            other.data_ = nullptr;
            other.rows_ = other.cols_ = 0;
        }
        return *this;
    }

    

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }
};

#endif