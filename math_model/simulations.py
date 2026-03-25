import random
import math
import sys

import pandas as pd

#Defining Arm values
length_ab, length_bc = 19, 18
min_alpha, min_beta, min_gamma = 0, 35, 32
max_alpha, max_beta, max_gamma = 270, 100, 127

#Clamping Function
def clamp(val, min_val, max_val):
    return max(min_val, min(max_val, val))

#Proccress D2 Function generating angles from position
def generate_angle(x, y, z):
    try:
        r = math.sqrt(x ** 2 + y ** 2)
        pho = math.sqrt(x** 2 + y ** 2 + z ** 2)

        alpha_radians = math.atan2(y, x)

        cos_phi = (length_ab ** 2 + pho ** 2 - length_bc ** 2) / (2 * length_ab * pho)
        cos_phi = clamp(cos_phi, -1, 1)
        phi = math.acos(cos_phi)
        omega = math.atan2(z, r)

        beta_radians = phi + omega

        cos_gamma = (length_ab ** 2 + length_bc ** 2 - pho ** 2) / (2 * length_ab * length_bc)
        cos_gamma = clamp(cos_gamma, -1, 1)
        gamma_radians = math.acos(cos_gamma)

        alpha = math.degrees(alpha_radians)
        beta =  math.degrees(beta_radians)
        gamma = math.degrees(gamma_radians)

        if not min_alpha < alpha < max_alpha:
            raise ValueError(f'Alpha angle is out of range: {alpha}')
        
        if not min_beta < beta < max_beta:
            raise ValueError(f'Beta angle is out of range: {beta}')

        if not min_gamma < gamma < max_gamma:
            raise ValueError(f'Gamma angle is out of range: {gamma}')

        return alpha, beta, gamma
    except Exception as e:
        return None

"""
Testing Code
"""
#Inverse of D2 Function position from angles
def angles_to_postion(alpha, beta, gamma):
    alpha_radian = math.radians(alpha)
    beta_radian = math.radians(beta)
    gamma_radian = math.radians(gamma)

    x = (length_ab * math.cos(beta_radian) - length_bc * math.cos(gamma_radian + beta_radian)) * math.cos(alpha_radian)
    y = (length_ab * math.cos(beta_radian) - length_bc * math.cos(gamma_radian + beta_radian)) * math.sin(alpha_radian)
    z = length_ab * math.sin(beta_radian) - length_bc * math.sin(gamma_radian + beta_radian)

    return x, y, z

#Checks the percent difference between input position and output position
def check_postion(input_coordinate, output_coordinate):
    cooridinate_coupled = list(zip(list(input_coordinate), list(output_coordinate)))
    
    difference = math.sqrt(sum([(coorid_input - coorid_output) ** 2 for coorid_input, coorid_output in cooridinate_coupled]))
    total_distance = math.sqrt(sum([cooridinate ** 2 for cooridinate in input_coordinate]))

    error = difference / total_distance

    return error

#Defining Test iterations
iterations = 10
if len(sys.argv) > 1:
    raw_iterations = sys.argv[1]
    try:
        iterations = int(raw_iterations)
    except:
        pass

#Generating test values points and angles in inches
test_input_value = []
resulting_angles = []
while len(test_input_value) < iterations:
    random_point = list(random.random() * 80 - 40 for _ in range(2))
    random_point.append(random.random() * 40)

    result_angle = generate_angle(*random_point)
    if result_angle is None:
        continue
    
    test_input_value.append(tuple(random_point))
    resulting_angles.append(result_angle)

#Runs the test
resulting_positions = []
for alpha, beta, gamma in resulting_angles:
    result_position = tuple(angles_to_postion(alpha, beta, gamma))
    resulting_positions.append(result_position)

differences = []
for input_cooridnate, output_cooridnate in list(zip(test_input_value, resulting_positions)):
    difference = check_postion(input_cooridnate, output_cooridnate)
    differences.append(difference)

data = {
    'Test Inputs': test_input_value,
    'Result Angles': resulting_angles,
    'Result Positions': resulting_positions,
    'Error': differences
}

x_values, y_values, z_values = zip(*test_input_value)
data_summary = {
    'range_x': tuple([min([abs(pos_value) for pos_value in x_values]), max([abs(pos_value) for pos_value in x_values])]),
    'range_y': tuple([min([abs(pos_value) for pos_value in y_values]), max([abs(pos_value) for pos_value in y_values])]),
    'range_z': tuple([min([abs(pos_value) for pos_value in z_values]), max([abs(pos_value) for pos_value in z_values])]),
    'Min Error': min(differences),
    'Max Error': max(differences),
    'Average Error': sum(differences) / len(differences)
}

#Maxizing view port
pd.set_option('display.max_rows', None)
pd.set_option('display.max_columns', None)
pd.set_option('display.max_colwidth', None)
pd.set_option('display.width', 1000)     

df = pd.DataFrame(data)
df_summary = pd.DataFrame(data_summary)

#Prints Reasult
print(df)
print(df_summary)