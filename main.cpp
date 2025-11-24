#include "image.h"
#include <iostream>
#include <iomanip>
#include <vector>

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
        std::vector<GrayscaleImage<float>> images = {
            GrayscaleImage<float>(1,1)(0,0) = 0.0f,
            GrayscaleImage<float>(1,1)(0,0) = 1.0f,
            GrayscaleImage<float>(1,1)(0,0) = 0.5f
        };

        auto avg = average_images(images);
        std::cout << "average of three 1x1 images:\n" << avg << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}