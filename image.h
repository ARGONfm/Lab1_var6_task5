#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <cstddef>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include <ostream>
#include <limits>
#include <type_traits>

template <typename T>
class GrayscaleImage {
private:
    T* data_;
    std::size_t rows_;
    std::size_t cols_;
    
    // === НАСЫЩЕНИЕ ===
    T saturate_add(T a, T b) const {
        if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
            if (b > 0 && a > std::numeric_limits<T>::max() - b) return std::numeric_limits<T>::max();
            if (b < 0 && a < std::numeric_limits<T>::lowest() + b) return std::numeric_limits<T>::lowest();
        }
        if constexpr (std::is_integral_v<T> && !std::is_signed_v<T>) {
            if (a > std::numeric_limits<T>::max() - b) return std::numeric_limits<T>::max();
        }
        if constexpr (std::is_floating_point_v<T>) {
            // для float просто считаем, переполнения нет
        }
        return a + b;
    }

    T saturate_mul(T a, T b) const {
        if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
            if (b != 0 && a > std::numeric_limits<T>::max() / b) return std::numeric_limits<T>::max();
            if (b != 0 && a < std::numeric_limits<T>::lowest() / b) return std::numeric_limits<T>::lowest();
        }
        return a * b;
    }

public:
    // Конструктор, деструктор, Rule of Five
    GrayscaleImage(std::size_t rows = 0, std::size_t cols = 0)
        : data_(nullptr), rows_(rows), cols_(cols_) {
        if (rows_ && cols_) {
            data_ = new T[rows_ * cols_]{};
        }
    }

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
    // === ОПЕРАТОР ДОСТУПА (r, c) ===
    T& operator()(std::size_t r, std::size_t c) {
        if (r >= rows_ || c >= cols_)
            throw std::out_of_range("GrayscaleImage: index out of range");
        return data_[r * cols_ + c];
    }

    const T& operator()(std::size_t r, std::size_t c) const {
        if (r >= rows_ || c >= cols_)
            throw std::out_of_range("GrayscaleImage: index out of range");
        return data_[r * cols_ + c];
    }

    // === ВЫВОД В ПОТОК ===
    friend std::ostream& operator<<(std::ostream& os, const GrayscaleImage& img) {
        for (std::size_t i = 0; i < img.rows_; ++i) {
            for (std::size_t j = 0; j < img.cols_; ++j) {
                if (j > 0) os << ' ';
                os << static_cast<double>(img(i, j));
            }
            os << '\n';
        }
        return os;
    }
    // === ОПЕРАТОРЫ + И * ===
    GrayscaleImage operator+(const GrayscaleImage& other) const {
        std::size_t nr = std::max(rows_, other.rows_);
        std::size_t nc = std::max(cols_, other.cols_);
        GrayscaleImage result(nr, nc);

        for (std::size_t i = 0; i < nr; ++i) {
            for (std::size_t j = 0; j < nc; ++j) {
                T a = (i < rows_ && j < cols_) ? (*this)(i, j) : T{0};
                T b = (i < other.rows_ && j < other.cols_) ? other(i, j) : T{0};

                if constexpr (std::is_same_v<T, bool>)
                    result(i, j) = a || b;
                else
                    result(i, j) = saturate_add(a, b);
            }
        }
        return result;
    }

    GrayscaleImage operator*(const GrayscaleImage& other) const {
        std::size_t nr = std::max(rows_, other.rows_);
        std::size_t nc = std::max(cols_, other.cols_);
        GrayscaleImage result(nr, nc);

        for (std::size_t i = 0; i < nr; ++i) {
            for (std::size_t j = 0; j < nc; ++j) {
                T a = (i < rows_ && j < cols_) ? (*this)(i, j) : T{0};
                T b = (i < other.rows_ && j < other.cols_) ? other(i, j) : T{0};

                if constexpr (std::is_same_v<T, bool>)
                    result(i, j) = a && b;
                else
                    result(i, j) = saturate_mul(a, b);
            }
        }
        return result;
    }

    // === ИНВЕРСИЯ ===
    GrayscaleImage operator!() const {
        GrayscaleImage result(*this);
        for (std::size_t i = 0; i < rows_ * cols_; ++i) {
            if constexpr (std::is_same_v<T, bool>)
                result.data_[i] = !result.data_[i];
            else if constexpr (std::is_same_v<T, float>)
                result.data_[i] = 1.0f - result.data_[i];
            else
                result.data_[i] = -result.data_[i];
        }
        return result;
    }

    // === ПОВОРОТ НА 180° ===
    GrayscaleImage rotated180() const {
        GrayscaleImage result(rows_, cols_);
        for (std::size_t i = 0; i < rows_; ++i) {
            for (std::size_t j = 0; j < cols_; ++j) {
                result(i, j) = (*this)(rows_ - 1 - i, cols_ - 1 - j);
            }
        }
        return result;
    }
    
    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }
};

#endif