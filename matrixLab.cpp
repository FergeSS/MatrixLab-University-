#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

//Free memory
void delete_array_2d(double** array, int size) {
    for (int i = 0; i < size; ++i) delete[] array[i];
    delete[] array;
}

//Calculate determinant
double det(double** matrix, int height, int width) {
    double det = 1;

    for (int i = 0; i < height; ++i) {
        for (int j = height - 1; j > i; --j) {
            if (matrix[i][i] != 0 && matrix[j][i] != 0) {
                double cof = matrix[j][i] / matrix[i][i];
                for (int k = 0; k < width; ++k) {
                    matrix[j][k] = matrix[j][k] - matrix[i][k] * cof;
                }
            }
            else if (matrix[i][i] == 0) {
                std::swap(matrix[j], matrix[i]); 
                det *= -1;
            }
        }
        det *= matrix[i][i];
    }

    if (det == -0.0) det = 0;
    return det;
}

void reverseMatrix(double** matrix, int size) {
    for (int i = 0; i < size / 2; ++i) {
        for (int j = 0; j < size; ++j) {
            std::swap(matrix[i][j], matrix[size - i - 1][size - j - 1]);
            std::swap(matrix[i][j + size], matrix[size - i - 1][size * 2 - j - 1]);
        }
    }
    if (size % 2 == 1){
        for (int j = 0; j < size / 2; ++j) {
            std::swap(matrix[size / 2][j], matrix[size / 2][size - j - 1]);
            std::swap(matrix[size / 2][j + size], matrix[size / 2][size * 2 - j - 1]);
        }
    }
}


void inverseMatrix(double** matrix, int size) {
    det(matrix, size, size * 2);
    reverseMatrix(matrix, size);
    det(matrix, size, size * 2);
    reverseMatrix(matrix, size);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j + size] = matrix[i][j + size] / matrix[i][i];
        }
    }  
}

int main(int argc, char* argv[]) {
    int width;
    int height;

    if (argc < 4) {
        std::cout << "Error, not enough arguments" << std::endl;
        return 1;
    }

    std::ofstream otp(argv[3]);
    std::ifstream inp(argv[2]);

    if (!inp) {
        std::cout << "Error! Input file not found!";
        return 1;
    }

    inp >> width;
    inp >> height;

    if (width != height) {
        otp << "error";
        otp.close();
        inp.close();
        return 0;
    }

    double** matrix = new double* [width];    
    double** matrix_copy = new double* [width];

    for (int i = 0; i < width; ++i) {
        matrix[i] = new double[width];
        matrix_copy[i] = new double[width * 2];
        for (int j = 0; j < width; j++) {
            inp >> matrix[i][j];
            matrix_copy[i][j] = matrix[i][j];
            if (i == j) matrix_copy[i][width + j] = 1;
            else matrix_copy[i][width + j] = 0;  
        }
    }
    inp.close();

    double deter = det(matrix, height, width);

    if (strcmp(argv[1], "-det") == 0) {
        otp << deter;
    }
    else if (strcmp(argv[1], "-inv") == 0) {
        if (deter == 0) {
            otp << "singular";       
        }
        else {
            inverseMatrix(matrix_copy, width);

            otp << width << " " << width << std::endl;

            for (int i = 0; i < width; ++i) {
                for (int j = width; j < 2 * width; ++j) {
                    otp << std::setw(20) << matrix_copy[i][j];
                }
                otp << std::endl;
            }
        }
    }
    else {
        otp << "error";
    }   
    otp.close();
    delete_array_2d(matrix, width);
    delete_array_2d(matrix_copy, width);
}
