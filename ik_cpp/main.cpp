#include <math.h>
#include <iostream>
#include <stdlib.h>

const float l1 = 17.5;
const float l2 = 22.3;
const float l3 = 31.5;

struct ArmState {
    double q1, q2, q3;
};

struct IkSolutions {
    double q1, q2a, q2b, q3a, q3b;
};

inline double rad2deg(double rad){
    return rad / 3.14 * 180; 
}

bool almostEqual(double x, double y, double epsilon = 0.01f){
   return fabs(x - y) < epsilon;
}

double eeHeadingWhenQ2IsZero(double q3){
    return atan2(l2 * sin(q3), l1 + l2 * cos(q3));
}

IkSolutions solveIk(float x, float y, float z) {
    IkSolutions solutions;
    solutions.q1 = atan2(y, x);
    
    // x w ukladzie przegubu 2
    double x2 = sqrt(x * x + y * y); 
    double z2 = z - l1;

    // promien koncowki w ukladzie pzregubu 2
    double d2 = sqrt(x2 * x2 + z2 * z2);
    
    // kat pod jakim jest koncowka w ukladzie przegubu 2   
    double theta = atan2(z2, x2);

    // rownanie nr 7  - https://motion.cs.illinois.edu/RoboticSystems/InverseKinematics.html
    double c3 = (d2 * d2 - l2 * l2 - l3 * l3) / (2 * l2 * l3);
    if (almostEqual(c3, 1)){
        c3 = 1;
    }
    else if (almostEqual(c3, -1)){
        c3 = -1;
    }

    if (c3 > 1 || c3 < -1) {
        throw std::range_error("cosine should be between -1 and 1");
    }
    std::cout << std::endl << "cosine 3: " << c3 << std::endl;
    
    solutions.q3a = acos(c3);
    solutions.q3b = -solutions.q3a;

    solutions.q2a = theta - eeHeadingWhenQ2IsZero(solutions.q3a);
    solutions.q2b = theta - eeHeadingWhenQ2IsZero(solutions.q3b);

    std::cout << "ik solution found maaaaan" << std::endl;
    std::cout << "q1: " << rad2deg(solutions.q1) << std::endl; 
    std::cout << "q2: " << rad2deg(solutions.q2a) << " " << rad2deg(solutions.q2b) << std::endl;
    std::cout << "q3: " << rad2deg(solutions.q3a) << " " << rad2deg(solutions.q3b) << std::endl;
    std::cout << "-------------------------" << std::endl;


    return solutions;
}

ArmState pickSolution(IkSolutions& solutions) {
    ArmState targetState;
    if (solutions.q2a > solutions.q2b){
        targetState.q1 = solutions.q1;
        targetState.q2 = solutions.q2a;
        targetState.q3 = solutions.q3a;
    }
    else {
        targetState.q1 = solutions.q1;
        targetState.q2 = solutions.q2b;
        targetState.q3 = solutions.q3b;
    }
    return targetState;
}

int main(int argc, char* argv[]){

    double x = atof(argv[1]);
    double y = atof(argv[2]);
    double z = atof(argv[3]);
    IkSolutions solutions;
    ArmState armState;
    try {
        solutions = solveIk(x, y, z);
        armState = pickSolution(solutions);
        std::cout << "q1: " << rad2deg(armState.q1) << std::endl;
        std::cout << "q2: " << rad2deg(armState.q2) << std::endl;
        std::cout << "q3: " << rad2deg(armState.q3) << std::endl << std::endl;
    }
    catch (std::range_error) {
        std::cout << "chlopie poza zasiegiem" << std::endl;
    }
}