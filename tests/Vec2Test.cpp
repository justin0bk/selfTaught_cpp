#include "../src/Vec2.h"
#include <iostream>

int main()
{
    // Create test vectors
    Vec2 v1(3, 4);
    Vec2 v2(1, 2);
    Vec2 v3(3, 4);

    std::cout << "v1: "; v1.print();
    std::cout << "v2: "; v2.print();
    std::cout << "v3: "; v3.print();


    std::cout << "Vector Operations Test\n-------------------\n";
    
    // Test equality
    std::cout << "Equality (v1 == v3): " << (v1 == v3) << "\n";
    std::cout << "Inequality (v1 != v2): " << (v1 != v2) << "\n";

    // Test arithmetic
    std::cout << "Addition (v1 + v2): "; (v1 + v2).print();
    std::cout << "Subtraction (v1 - v2): "; (v1 - v2).print();
    
    // Test scalar operations
    std::cout << "Multiply by 2 (v1 * 2): "; (v1 * 2).print();
    std::cout << "Divide by 2 (v1 / 2): "; (v1 / 2).print();
    
    // Test compound operators
    Vec2 v4 = v1;
    v4 += v2;
    std::cout << "Compound add (v4 += v2): "; v4.print();
    
    // Test geometric functions
    std::cout << "Length of v1: " << v1.length() << "\n";
    std::cout << "Distance v1 to v2: " << v1.dist(v2) << "\n";
    
    // Test normalization
    Vec2 v5 = v1;
    v5.normalize();
    std::cout << "Normalized v1: "; v5.print();
    
    return 0;
}