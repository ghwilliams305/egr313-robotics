import random
import math
import sys

import pandas as pd

#Defining Arm values
length_ab, length_bc = 19, 18
min_alpha, min_beta, min_gamma = 0, 35, 32
max_alpha, max_beta, max_gamma = 270, 100, 127

#Proccress D2 Function generating angles from position
def generate_angle(x, y, z):
    try:
        r = math.sqrt(x** 2 + y ** 2)

        alpha_radians = math.atan2(y, x)
        if alpha_radians < 0:
            alpha_radians += 2 * math.pi

        phi = math.acos((length_ab ** 2 + x ** 2 + y ** 2 - length_bc ** 2) / (2 * length_ab * r))
        omega = math.atan2(z, r)
        if omega < 0:
            omega += 2 * math.pi

        beta_radians = phi + omega

        gamma_radians = math.acos((length_ab ** 2 + length_bc ** 2 - r ** 2) / (2 * length_ab * length_bc))

        conversion_factor_to_degrees = 180 / math.pi
        alpha = alpha_radians * conversion_factor_to_degrees
        beta =  beta_radians * conversion_factor_to_degrees
        gamma = gamma_radians * conversion_factor_to_degrees

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
    conversion = math.pi / 180
    alpha_radian = alpha * conversion
    beta_radian = beta * conversion
    gamma_radian = gamma * conversion

    x = (length_ab * math.cos(beta_radian) - length_bc * math.cos(gamma_radian + beta_radian)) * math.cos(alpha_radian)
    y = (length_ab * math.cos(beta_radian) - length_bc * math.cos(gamma_radian + beta_radian)) * math.sin(alpha_radian)
    z = length_ab * math.sin(beta_radian) - length_bc * math.sin(gamma_radian + beta_radian)

    return x, y, z

#Checks the percent difference between input position and output position
def check_postion(input_coordinate, output_coordinate):
    input_distance = math.sqrt(sum([coordinate ** 2 for coordinate in input_coordinate]))
    output_distance = math.sqrt(sum([coordinate ** 2 for coordinate in output_coordinate]))

    max_cooridinates = list(angles_to_postion(max_alpha, max_beta, max_gamma))
    max_distance = math.sqrt(sum(cooridinate ** 2 for cooridinate in max_cooridinates))
    difference = abs(input_distance - output_distance) / input_distance

    return difference

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