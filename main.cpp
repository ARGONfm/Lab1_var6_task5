#include "image.h"
#include <iostream>
#include <iomanip>
#include <vector>

// === ЗАДАЧА: усреднение массива изображений (вне класса) ===
template <typename T>
GrayscaleImage<T> average_images(const std::vector<GrayscaleImage<T>>& imgs) {
    if (imgs.empty()) return GrayscaleImage<T>();

    std::size_t max_r = 0, max_c = 0;
    for (const auto& img : imgs) {
        max_r = std::max(max_r, img.rows());
        max_c = std::max(max_c, img.cols());
    }

    GrayscaleImage<T> result(max_r, max_c);
    GrayscaleImage<double> sum(max_r, max_c);
    std::vector<double> count(max_r * max_c, 0.0);

    for (const auto& img : imgs) {
        for (std::size_t i = 0; i < img.rows(); ++i) {
            for (std::size_t j = 0; j < img.cols(); ++j) {
                std::size_t idx = i * max_c + j;
                sum(i, j) += static_cast<double>(img(i, j));
                count[i * max_c + j]++;
            }
        }
    }

    for (std::size_t i = 0; i < max_r; ++i) {
        for (std::size_t j = 0; j < max_c; ++j) {
            if (count[i * max_c + j] > 0) {
                result(i, j) = static_cast<T>(sum(i, j) / count[i * max_c + j]);
            }
        }
    }
    return result;
}

int main() {
    try {
        // Демонстрация разных типов
        GrayscaleImage<float> img1(2, 3);
        img1(0,0) = 0.1f; img1(0,1) = 0.2f; img1(0,2) = 0.3f;
        img1(1,0) = 0.4f; img1(1,1) = 0.5f; img1(1,2) = 0.6f;

        GrayscaleImage<float> img2(3, 2);
        img2(0,0) = 0.5f; img2(0,1) = 0.5f;
        img2(1,0) = 0.5f; img2(1,1) = 0.5f;
        img2(2,0) = 0.5f; img2(2,1) = 0.5f;

        std::cout << std::fixed << std::setprecision(1);
        std::cout << "img1:\n" << img1 << "\n";
        std::cout << "img2:\n" << img2 << "\n";

        auto sum = img1 + img2;
        auto prod = img1 * img2;
        auto inv = !img1;
        auto rot = img1.rotated180();

        std::cout << "sum (3x3):\n" << sum << "\n";
        std::cout << "product (3x3):\n" << prod << "\n";
        std::cout << "inverted img1:\n" << inv << "\n";
        std::cout << "rotated 180°:\n" << rot << "\n";

        std::cout << "fill_factor img1: " << img1.fill_factor() << "\n";
        std::cout << "fill_factor sum: " << sum.fill_factor() << "\n";

        // Задача: average_images вне класса
        GrayscaleImage<float> i1(1, 1); i1(0, 0) = 0.0f;
        GrayscaleImage<float> i2(1, 1); i2(0, 0) = 1.0f;
        GrayscaleImage<float> i3(1, 1); i3(0, 0) = 0.5f;

        auto avg = average_images(std::vector<GrayscaleImage<float>>{i1, i2, i3});
        std::cout << "average of three 1x1 images:\n" << avg << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}