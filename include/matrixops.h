#pragma once
#ifndef _MATRIXOPS_H
#define _MATRIXOPS_H
#include <iostream>

namespace matrixops {
    
    template <typename T = double, std::size_t rowSize = 6, std::size_t colSize = 6> 
        int matrixVectorMultiply(T outputVector[], const T matrix[rowSize][colSize], T inputVector[])
    {
        for (unsigned int i = 0; i < rowSize; i++) {
            outputVector[i] = 0;
            for (int k = 0; k < colSize; k++) {
                outputVector[i] = outputVector[i] + (matrix[i][k] * inputVector[k]);
            }
        }

        return 0;
    }

    template <typename T = double, std::size_t length = 6>
        int vectorAdd(T outputVector[], T inputvector1[], T inputVector2[])
    {
        for (unsigned int i = 0; i < length; i++) {
            outputVector[i] = inputvector1[i] + inputVector2[i];
        }
        return 0;
    }
    template <typename T = double, std::size_t length = 6>
        int vectorSub(T outputVector[], T inputvector1[], T inputVector2[])
    {
        for (unsigned int i = 0; i < length; i++) {
                outputVector[i] = inputvector1[i] - inputVector2[i];
        }
        return 0;
    }
}
 
#endif // !_MATRIXOPS_H

