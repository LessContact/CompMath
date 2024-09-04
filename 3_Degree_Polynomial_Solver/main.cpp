#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <cmath>

enum{
    left,
    right
};

typedef struct root {
    root() = default;
    root(double value, int multiplicity) : value(value), multiplicity(multiplicity) {}
    double value;
    int multiplicity;
} root;

typedef struct coeffs {
    double A;
    double B;
    double C;
    double D;
} coeffs;

inline bool EqualsZero(double value, double epsilon){
    return std::abs(value) < epsilon;
}

inline double y(double x, coeffs coeffs){
    return coeffs.A*x*x*x + coeffs.B*x*x + coeffs.C*x + coeffs.D;
}
inline double yDif(double x, coeffs coeffs){
    return coeffs.A*x*x + coeffs.B*x + coeffs.C;
}

inline void OutputRoot(root root){
    std::cout << "Root value: " << root.value << std::endl << "Multiplicity: " << root.multiplicity << std::endl;
}

root SolveClosedInterval(double pointLeft, double pointRight){
    //todo: implement
}

//direction is the infinity direction
root SolveOpenInterval(double point, int direction){
    //todo: implement
}

int main() {
    _setmode(STDOUT_FILENO, _O_U8TEXT);

    double epsilon, delta;
    coeffs coeffs;
    std::wstring temp;

    std::wcout << L"Enter your polynomial coefficients for the form Ax³+Bx²+Cx+D. Type \"d\" for default(1,-2,-1,2)" << std::endl << "A = " << std::endl;
    std::wcin >> temp;
    if(temp == L"d"){
        coeffs.A = 1.0l;
        coeffs.B = -2.0L;
        coeffs.C = -1.0L;
        coeffs.D = 2.0L;
    }
    else {
        coeffs.A = std::stod(temp);
        std::wcout << "B = " << std::endl;
        std::wcin >> coeffs.B;
        std::wcout << "C = " << std::endl;
        std::wcin >> coeffs.C;
        std::wcout << "D = " << std::endl;
        std::wcin >> coeffs.D;
    }
    coeffs.A = coeffs.A / coeffs.A;
    coeffs.B = coeffs.B / coeffs.A;
    coeffs.C = coeffs.C / coeffs.A;
    coeffs.D = coeffs.D / coeffs.A;

    std::wcout << L"Set desirable precision. Type \"d\" for default (ε=1e-8, Δ=1)" << std::endl << L"ε:" << std::endl;
    std::wcin >> temp;
    if(temp == L"d"){
        epsilon = 0.00000001L;
        delta = 1.0L;
    }
    else {
        epsilon = std::stod(temp);
        std::wcout << "Approach step size." << std::endl << L"Δ:" << std::endl;
        std::wcin >> delta;
    }

    // ------------------- proper stuff -------------------

    struct coeffs diffedCoeffs;
    {
        diffedCoeffs.A = coeffs.A * 3.0l;
        diffedCoeffs.B = coeffs.B * 2.0L;
        diffedCoeffs.C = coeffs.C;
    }

    double discriminant = diffedCoeffs.B * diffedCoeffs.B - 4 * diffedCoeffs.A * diffedCoeffs.C;
    root difRoot1, difRoot2;
    difRoot1.value = (-diffedCoeffs.B + std::sqrt(discriminant)) / (diffedCoeffs.A * 2);
    difRoot2.value = (-diffedCoeffs.B - std::sqrt(discriminant)) / (diffedCoeffs.A * 2);
    if(difRoot1.value > difRoot2.value) std::swap(difRoot1, difRoot2); // sort roots)))

    // CASE IV
    if(EqualsZero(discriminant, epsilon)){ //todo: god knows if this will work
        //third case
        if(EqualsZero(y(difRoot1.value, coeffs), epsilon)){
            difRoot1.multiplicity = 3;
            OutputRoot(difRoot1);
            return 0;
        }
        //first case
        else if(y(difRoot1.value, coeffs) > epsilon) {
            root root = SolveOpenInterval(difRoot1.value, left);
            OutputRoot(root);
            return 0;
        }
        //second case
        else {
            root root = SolveOpenInterval(difRoot1.value, right);
            OutputRoot(root);
            return 0;
        }
    }

    //CASE I
    if(discriminant < 0.0l){
        double temp = y(0.0l, coeffs);
        if(temp == 0.0l){
            root root (0.0l, 1);
            OutputRoot(root);
            return 0;
        }
        else if(temp < 0.0l){
            root root = SolveOpenInterval(0.0l, right);
            OutputRoot(root);
            return 0;
        }
        else {
            root root = SolveOpenInterval(0.0l, left);
            OutputRoot(root);
            return 0;
        }
    }

    //CASE II

    //II.3
    if(y(difRoot1.value, coeffs) > epsilon && y(difRoot2.value,coeffs) < -epsilon){
        root root1 = SolveOpenInterval(difRoot1.value, left);
        root root2 = SolveClosedInterval(difRoot1.value, difRoot2.value);
        root root3 = SolveOpenInterval(difRoot2.value, right);
        OutputRoot(root1);
        OutputRoot(root2);
        OutputRoot(root3);
        return 0;
    }
    //II.1
    else if(y(difRoot2.value, coeffs) > 0){
        root root = SolveOpenInterval(difRoot1.value, left);
        OutputRoot(root);
        return 0;
    }//II.2
    else if(y(difRoot1.value, coeffs) < 0){
        root root = SolveOpenInterval(difRoot2.value, right);
        OutputRoot(root);
        return 0;
    }

    //CASE III

    //III.1
    if(EqualsZero(y(difRoot2.value, coeffs), epsilon) && y(difRoot1.value, coeffs) > epsilon){
        difRoot2.multiplicity = 2;
        root root = SolveOpenInterval(difRoot2.value, right);
        OutputRoot(root);
        OutputRoot(difRoot2);
        return 0;
    }
    //III.2
    else if(EqualsZero(y(difRoot1.value, coeffs), epsilon) && y(difRoot2.value, coeffs) < -epsilon){
        difRoot1.multiplicity = 2;
        root root = SolveOpenInterval(difRoot2.value, right);
        OutputRoot(difRoot1);
        OutputRoot(root);
        return 0;
    }

    std::wcout << L"You've reached the end of the line.\nThis should never happen.\nBetter fix this quick\n┻━┻ ︵ ＼( °□° )／ ︵ ┻━┻" << std::endl;

    return 0;
}
