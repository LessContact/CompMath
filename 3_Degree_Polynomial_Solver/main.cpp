#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <cmath>
#include <random>
#include <iomanip>

std::random_device rd;
std::mt19937 gen(rd());

enum : int{
    left = -1,
    right = 1
};

struct root {
    root() {value = 0.0L; multiplicity = 0;}
    root(double value, int multiplicity) : value(value), multiplicity(multiplicity) {}
    double value;
    int multiplicity;
};

struct coeffs {
    double A;
    double B;
    double C;
    double D;
};

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
    std::wcout << L"Root value: " << root.value << std::endl << L"Multiplicity: " << root.multiplicity << std::endl;
}

class Solver {
public:
    double epsilon;
    double delta;
    coeffs coeffs;

    Solver() = default;
    Solver(double epsilon, double delta, struct coeffs coeffs) : epsilon(epsilon), delta(delta), coeffs(coeffs) {}

    root SolveClosedInterval(double pointLeft, double pointRight) {
        if(EqualsZero(std::abs(pointRight - pointLeft))) return {pointLeft, 1};

        double center = (pointLeft + pointRight) / 2.0L;
        double functionAtCenter = y(center, coeffs);
        if(EqualsZero(functionAtCenter)) return {center, 1};

        double functionAtLeft = y(pointLeft, coeffs);
        if(functionAtLeft < 0.0L) {
            if (functionAtCenter > 0.0L) return SolveClosedInterval(pointLeft, center);
            else if (functionAtCenter < 0.0L) return SolveClosedInterval(center, pointRight);
        }
        else {
            if (functionAtCenter > 0.0L) return SolveClosedInterval(center, pointRight);
            else if (functionAtCenter < 0.0L) return SolveClosedInterval(pointLeft, center);
        }

        return {};
    }

//direction is the infinity direction
    root SolveOpenInterval(double point, int direction) {
        if(direction == right) {
            return SolveRightInterval(point);
        }
        if(direction == left){
            return SolveLeftInterval(point);
        }
        return {};
    }

    [[nodiscard]] inline bool EqualsZero(double value) const{
        return std::abs(value) < epsilon;
    }

private:
    root SolveRightInterval(double point){
        double newPointVal = y(point + delta, coeffs);
        if (newPointVal > 0.0L){
            return SolveClosedInterval(point, point + delta);
        }
        else if(newPointVal < 0.0L){
            return SolveRightInterval(point + delta);
        }
        else return {point + delta, 1};
    }
    root SolveLeftInterval(double point){
        double newPointVal = y(point - delta, coeffs);
        if (newPointVal < 0.0L){
            return SolveClosedInterval(point - delta, point);
        }
        else if (newPointVal > 0.0L){
            return SolveLeftInterval(point - delta);
        }
        else return {point - delta, 1};
    }
};

/* variables
 * 1 -2 -1 2 (-1 1 2)
 * 2 -2 -1 2 (-0.8755)
 * 1 1 1 -3 (1)
 * 6 0 0 1 (-0.55)
 * 5 3 -1.8 0.2 (-1 0.2)
 * */

int main() {
    _setmode(STDOUT_FILENO, _O_U8TEXT);

    double epsilon, delta;
//    Solver Solver = Solver;

    coeffs coeffs;
    std::wstring temp;

    std::wcout << L"Hi! (p≧w≦q)\nEnter your polynomial coefficients for the form Ax³+Bx²+Cx+D.\nType \"r\" for random.\nType \"d\" for default(1,-2,-1,2)" << std::endl << "A = " << std::endl;
    std::wcin >> temp;
    if(temp == L"d"){
        coeffs.A = 1.0l;
        coeffs.B = -2.0L;
        coeffs.C = -1.0L;
        coeffs.D = 2.0L;
    }
    else if(temp == L"r"){
//        std::uniform_real_distribution<double> dis(std::numeric_limits<double>::min(), std::numeric_limits<double>::max());
        std::uniform_real_distribution<double> dis(100000000000000.0L, -10000000000000.0L);
        coeffs.A = dis(gen);
        coeffs.B = dis(gen);
        coeffs.C = dis(gen);
        coeffs.D = dis(gen);
        std::wcout << L"Generated equation: " << coeffs.A << L"•x³+" << coeffs.B << L"•x²+" << coeffs.C << L"•x+" << coeffs.D << L" = 0." << std::endl;
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
    std::wcout << std::setprecision(static_cast<int>(std::round(std::log10(epsilon))));

    coeffs.B = coeffs.B / coeffs.A;
    coeffs.C = coeffs.C / coeffs.A;
    coeffs.D = coeffs.D / coeffs.A;
    coeffs.A = coeffs.A / coeffs.A;

    Solver Solver(epsilon, delta, coeffs);

    // ------------------- proper stuff -------------------

    struct coeffs diffedCoeffs;
    {
        diffedCoeffs.A = coeffs.A * 3.0l;
        diffedCoeffs.B = coeffs.B * 2.0L;
        diffedCoeffs.C = coeffs.C;
        diffedCoeffs.D = 0.0L;
    }

    double discriminant = diffedCoeffs.B * diffedCoeffs.B - 4 * diffedCoeffs.A * diffedCoeffs.C;
    root difRoot1, difRoot2;
    difRoot1.value = (-diffedCoeffs.B + std::sqrt(discriminant)) / (diffedCoeffs.A * 2);
    difRoot2.value = (-diffedCoeffs.B - std::sqrt(discriminant)) / (diffedCoeffs.A * 2);
    if(difRoot1.value > difRoot2.value) std::swap(difRoot1, difRoot2); // sort roots)))

    // CASE IV
    if(Solver.EqualsZero(discriminant)){
        //third case
        if(Solver.EqualsZero(y(difRoot1.value, coeffs))){
            difRoot1.multiplicity = 3;
            OutputRoot(difRoot1);
            return 0;
        }
        //first case
        else if(y(difRoot1.value, coeffs) > Solver.epsilon) {
            root root = Solver.SolveOpenInterval(difRoot1.value, left);
            OutputRoot(root);
            return 0;
        }
        //second case
        else {
            root root = Solver.SolveOpenInterval(difRoot1.value, right);
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
            root root = Solver.SolveOpenInterval(0.0l, right);
            OutputRoot(root);
            return 0;
        }
        else {
            root root = Solver.SolveOpenInterval(0.0l, left);
            OutputRoot(root);
            return 0;
        }
    }

    //CASE II

    //II.3
    if(y(difRoot1.value, coeffs) > Solver.epsilon && y(difRoot2.value,coeffs) < -Solver.epsilon){
        root root1 = Solver.SolveOpenInterval(difRoot1.value, left);
        root root2 = Solver.SolveClosedInterval(difRoot1.value, difRoot2.value);
        root root3 = Solver.SolveOpenInterval(difRoot2.value, right);
        OutputRoot(root1);
        OutputRoot(root2);
        OutputRoot(root3);
        return 0;
    }
    //II.1
    else if(y(difRoot2.value, coeffs) > 0){
        root root = Solver.SolveOpenInterval(difRoot1.value, left);
        OutputRoot(root);
        return 0;
    }//II.2
    else if(y(difRoot1.value, coeffs) < 0){
        root root = Solver.SolveOpenInterval(difRoot2.value, right);
        OutputRoot(root);
        return 0;
    }

    //CASE III

    //III.1
    if(EqualsZero(y(difRoot2.value, coeffs), Solver.epsilon) && y(difRoot1.value, coeffs) > Solver.epsilon){
        difRoot2.multiplicity = 2;
        root root = Solver.SolveOpenInterval(difRoot2.value, right);
        OutputRoot(root);
        OutputRoot(difRoot2);
        return 0;
    }
    //III.2
    else if(EqualsZero(y(difRoot1.value, coeffs), Solver.epsilon) && y(difRoot2.value, coeffs) < -Solver.epsilon){
        difRoot1.multiplicity = 2;
        root root = Solver.SolveOpenInterval(difRoot2.value, right);
        OutputRoot(difRoot1);
        OutputRoot(root);
        return 0;
    }

    std::wcout << L"You've reached the end of the line.\nThis should never happen.\nBetter fix this quick\n┻━┻ ︵ ＼( °□° )／ ︵ ┻━┻" << std::endl;

    return 0;
}
