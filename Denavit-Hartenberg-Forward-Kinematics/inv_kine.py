import numpy as np
import sympy as sym
theta, alpha, d, a = sym.symbols('theta alpha d a')
from params import *


# mtrx = sym.Matrix(
#     [
#     [sym.cos(theta),-sym.sin(theta)*sym.cos(alpha),sym.sin(theta)*sym.sin(alpha),a*sym.cos(theta)],
#     [sym.sin(theta),sym.cos(theta)*sym.cos(alpha),-sym.cos(theta)*sym.sin(alpha),a*sym.sin(theta)],
#     [0,sym.sin(alpha),sym.cos(alpha),d],
#     [0,0,0,1]
# ])

# print(mtrx.inv().subs(
#     [(theta, 0),
#     (alpha, 0),
#     (d, 0),
#     (a, 1)
#     ]))


# d - os z
# a - os x




# q1 = atan2(y, x)

# algorithm

# move the q1 to match the angle

# introduce new coordinates - (sqrt(x^2 + y^2), z) as a plane as x_2, z_2
# define distance from the target point as xd 
# q2 = +-arccos((xd - l2^2 - l3^2) / 2l2l3) 
from math import sqrt

# x_2 /= sqrt(x&*)
import numpy as np



def solve_ik(x, y, z):
    q_1 = np.arctan2(y, x)
    
    # x in coord frame of joint 2
    x_2 = np.sqrt(x**2 + y**2)
    z_2 = z - l1

    # radius of final point in frame of joint 2
    d_2 = np.sqrt(x_2**2 + z_2**2)

    # angle of final point in frame of joint 2
    theta = np.arctan2(z_2, x_2)

    # q_2 can be infered from the length of radius which final destination points belong to
    # eq 7    
    # elbow up and elbow down sols
    c_3 = (d_2**2 - l2**2 - l3**2) / (2 * l2 * l3)
    if np.allclose(c_3, 1):
        c_3 = 1
    elif np.allclose(c_3, -1):
        c_3 = -1
    
    q_3_a = np.arccos(c_3)
    q_3_b = -q_3_a


    heading_when_q_2_is_zero = lambda q_3: np.arctan2(l2*np.sin(q_3), l1+l2*np.cos(q_3)) 

    q_2_a = theta - heading_when_q_2_is_zero(q_3_a)
    q_2_b = theta - heading_when_q_2_is_zero(q_3_b)

    return q_1, q_2_a, q_2_b, q_3_a, q_3_b



def pick_angles(q_2_a, q_2_b, q_3_a, q_3_b):
    if q_2_a > q_2_b:
        return q_2_a, q_3_a
    else:
        return q_2_b, q_3_b
    