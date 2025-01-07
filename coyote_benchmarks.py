import os
from coyote import *
import time
import csv
import subprocess
from evaluate import * 
import numpy as np
 
def cond(b, true, false):
    return b * true + (Var('1') + b) * false

def shift_down(image):
    return [[Var('0')] * 16] + image[:-1]

def shift_up(image):
    return image[1:] + [[Var('0')] * 16]

def shift_left(image):
    return [row[1:] + [Var('0')] for row in image]

def shift_right(image):
    return [[Var('0')] + row[:-1] for row in image]

def matrix_addition(matrix1, matrix2):    
    return [[matrix1[i][j] + (matrix2[i][j]) for j in range(16)] for i in range(16)]

coyote = coyote_compiler()
# Fullly replicated benchmarks 
"""    
@coyote.define_circuit(xs=vector(3, replicate=True), ys=vector(3, replicate=True))
def distances_3x3_fully(xs, ys):
    return [(x - y) * (x - y) for x in xs for y in ys]

@coyote.define_circuit(xs=vector(4, replicate=True), ys=vector(4, replicate=True))
def distances_4x4_fully(xs, ys):
    return [(x - y) * (x - y) for x in xs for y in ys]

@coyote.define_circuit(xs=vector(5, replicate=True), ys=vector(5, replicate=True))
def distances_5x5_fully(xs, ys):
    return [(x - y) * (x - y) for x in xs for y in ys]

@coyote.define_circuit(a=matrix(2, 2, replicate=True), b=matrix(2, 2, replicate=True))
def matmul_2x2_fully(a, b):
    return [recursive_sum([a[i][k] * b[k][j] for k in range(len(a))]) for i in range(len(a)) for j in range(len(a))]

@coyote.define_circuit(a=matrix(8, 8, replicate=True), b=matrix(8, 8, replicate=True))
def matmul_8x8_fully(a, b):
    return [recursive_sum([a[i][k] * b[k][j] for k in range(len(a))]) for i in range(len(a)) for j in range(len(a))]

@coyote.define_circuit(sig=vector(4, replicate=True), ker=vector(2, replicate=True))
def conv_4x2_fully(sig, ker):
    output = []
    for offset in range(len(sig) - len(ker) + 1):
        output.append(recursive_sum([sig[offset + i] * ker[i] for i in range(len(ker))]))
    return output

@coyote.define_circuit(sig=vector(5, replicate=True), ker=vector(3, replicate=True))
def conv_5x3_fully(sig, ker):
    output = []
    for offset in range(len(sig) - len(ker) + 1):
        output.append(recursive_sum([sig[offset + i] * ker[i] for i in range(len(ker))]))
    return output
@coyote.define_circuit(v1=vector(3, replicate=True), v2=vector(3, replicate=True))
def dot_product_3x3_fully(v1, v2):
    return recursive_sum([a * b for a, b in zip(v1, v2)])

@coyote.define_circuit(v1=vector(6, replicate=True), v2=vector(6, replicate=True))
def dot_product_6x6_fully(v1, v2):
    return recursive_sum([a * b for a, b in zip(v1, v2)])

@coyote.define_circuit(v1=vector(10, replicate=True), v2=vector(10, replicate=True))
def dot_product_10x10_fully(v1, v2):
    return recursive_sum([a * b for a, b in zip(v1, v2)])
@coyote.define_circuit(cs=vector(3, replicate=True), os=vector(6, replicate=True))
def sort_3_packed_fully(cs, os):
    return cond(cs[0], 
                (cond(cs[1], 
                    os[0],
                    cond(cs[2], os[1], os[4]))), 
                (cond(cs[2],
                    os[2],
                    cond(cs[1], os[3], os[5]))))

@coyote.define_circuit(v1=vector(32, replicate=True), v2=vector(32, replicate=True))
def dot_product_32x32_fully(v1, v2):
    return recursive_sum([a * b for a, b in zip(v1, v2)])
    
@coyote.define_circuit(xs=vector(32, replicate=True), ys=vector(32, replicate=True))
def distances_32x32_fully(xs, ys):
    return [(x - y) * (x - y) for x in xs for y in ys]

@coyote.define_circuit(c0=vector(16,replicate=True), c1=vector(16,replicate=True), c2=vector(16,replicate=True), c3=vector(16,replicate=True), c4=vector(16,replicate=True))
def poly_reg_16(c0, c1, c2, c3, c4):
    return [c1[i] - (c0[i] * c0[i] * c4[i] + c0[i] * c3[i] + c2[i]) for i in range(16)]

@coyote.define_circuit(a=vector(16,replicate=True), b=vector(16,replicate=True))
def l2_distance_16(a, b):
    return recursive_sum([(x - y) * (x - y) for x, y in zip(a, b)])

@coyote.define_circuit(c0=vector(16,replicate=True), c1=vector(16,replicate=True), c2=vector(16,replicate=True), c3=vector(16,replicate=True))
def linear_reg_16(c0, c1, c2, c3):
    return [c1[i] - (c2[i] * c0[i]) - c3[i] for i in range(16)]

@coyote.define_circuit(image = matrix(16,16))
def box_blur_16x16(image):
    top_row = shift_up(image)
    bottom_row = shift_down(image)
    top_sum = matrix_addition(matrix_addition(shift_left(top_row), top_row), shift_right(top_row))
    curr_sum = matrix_addition(matrix_addition(shift_left(image), image), shift_right(image))
    bottom_sum = matrix_addition(matrix_addition(shift_left(bottom_row), bottom_row), shift_right(bottom_row))
    result = matrix_addition(matrix_addition(top_sum, curr_sum), bottom_sum)
    return [result[i][j] for i in range(16) for j in range(16)]

@coyote.define_circuit(image = matrix(16,16,replicate=True))
def roberts_cross_16x16(image):
    height, width = 16,16
    ## add padding to the image
    result = np.zeros_like(image)
    padded_image = np.pad(image, pad_width=1, mode='constant', constant_values=Var('0'))
    gx_kernel = [[Var('1'),Var('0')],[Var('0'),Var('-1')]]
    gy_kernel = [[Var('0'),Var('1')],[Var('-1'),Var('0')]]
    for i in range(1,height+1):
        for j in range(1,width+1):
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            Gx = curr_row[1] + bottom_row[2] 
            # Apply Kernel 2 (Gy)
            Gy = curr_row[2] + bottom_row[1]    
            # Compute the gradient magnitude and store in the output array (clipped to 255)
            result[i-1][j-1] = Gx*Gx + Gy*Gy
    return [result[i][j] for i in range(height) for j in range(width)]

@coyote.define_circuit(image = matrix(16,16,replicate=True))
def sobel_16x16(image):
    height, width = 16,16
    result = np.zeros_like(image)
    padded_image = np.pad(image, pad_width=1, mode='constant', constant_values=Var('0'))
    gx_output = result 
    gy_output = result
    gx_kernel = [[Var('-1'),Var('0'),Var('1')],[Var('-2'),Var('0'),Var('2')],[Var('-1'),Var('0'),Var('1')]]
    for i in range(1,height+1):
        for j in range(1,width+1):
            #Traverse the kernel window centered around (i, j)
            top_row = padded_image[i-1, j-1:j+2]
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            top_sum = top_row[0] + top_row[2]
            curr_sum = Var('2')*curr_row[0] + Var('2') * curr_row[2]
            bottom_sum = bottom_row[0] + bottom_row[2]
            gx_output[i-1, j-1] = top_sum + curr_sum + bottom_sum
    ###########################
    gy_kernel = [[Var('-1'),Var('-2'),Var('-1')],[Var('0'),Var('0'),Var('0')],[Var('1'),Var('2'),Var('1')]]
    for i in range(1,height+1):
        for j in range(1,width+1):
            top_row = padded_image[i-1, j-1:j+2]
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            top_sum = top_row[0] + Var('2')*top_row[1]  + top_row[2]
            bottom_sum = bottom_row[0] + Var('2')*bottom_row[1] + bottom_row[2]
            gy_output[i-1, j-1] = top_sum + bottom_sum
    ########################            
    for i in range(height):
        for j in range(width):
            result[i][j]= gx_output[i][j] * gx_output[i][j] + gy_output[i][j] * gy_output[i][j]
    return [result[i][j] for i in range(height) for j in range(width)]

@coyote.define_circuit(cs=vector(10, replicate=True), os=vector(5, replicate=True))
def max_5_packed_fully(cs, os):
    return cond(cs[0], 
                (cond(cs[1],
                    cond(cs[3],
                        cond(cs[6], os[0], os[4]),
                        cond(cs[7], os[3], os[4])),
                    cond(cs[4],
                        cond(cs[8], os[2], os[4]),
                        cond(cs[7], os[3], os[4])))),
                    
                (cond(cs[2],
                    cond(cs[5],
                        cond(cs[9], os[1], os[4]),
                        cond(cs[7], os[3], os[4])),
                    cond(cs[4],
                        cond(cs[8], os[2], os[4]),
                        cond(cs[7], os[3], os[4])))))
"""
########################################################################################
#########################Additional CHIHAB Benchmarks###################################
########################################################################################

"""
@coyote.define_circuit(a=vector(4,replicate=True), b=vector(4,replicate=True))
def l2_distance_4(a, b):
    return recursive_sum([(x - y) * (x - y) for x, y in zip(a, b)])
##############################
@coyote.define_circuit(a=vector(8,replicate=True), b=vector(8,replicate=True))
def l2_distance_8(a, b):
    return recursive_sum([(x - y) * (x - y) for x, y in zip(a, b)])
###############################

#########################################################################################
#########################################################################################
@coyote.define_circuit(c0=vector(4,replicate=True), c1=vector(4,replicate=True), c2=vector(4,replicate=True), c3=vector(4,replicate=True), c4=vector(4,replicate=True))
def poly_reg_4(c0, c1, c2, c3, c4):
    return [c1[i] - (c0[i] * c0[i] * c4[i] + c0[i] * c3[i] + c2[i]) for i in range(4)]
################################
@coyote.define_circuit(c0=vector(8,replicate=True), c1=vector(8,replicate=True), c2=vector(8,replicate=True), c3=vector(8,replicate=True), c4=vector(8,replicate=True))
def poly_reg_8(c0, c1, c2, c3, c4):
    return [c1[i] - (c0[i] * c0[i] * c4[i] + c0[i] * c3[i] + c2[i]) for i in range(8)]

#########################################################################################
#########################################################################################
@coyote.define_circuit(c0=vector(4,replicate=True), c1=vector(4,replicate=True), c2=vector(4,replicate=True), c3=vector(4,replicate=True))
def linear_reg_4(c0, c1, c2, c3):
    return [c1[i] - (c2[i] * c0[i]) - c3[i] for i in range(4)]
#################################
@coyote.define_circuit(c0=vector(8,replicate=True), c1=vector(8,replicate=True), c2=vector(8,replicate=True), c3=vector(8,replicate=True))
def linear_reg_8(c0, c1, c2, c3):
    return [c1[i] - (c2[i] * c0[i]) - c3[i] for i in range(8)]

#########################################################################################
#########################################################################################
@coyote.define_circuit(a=vector(4,replicate=True), b=vector(4,replicate=True))
def hamming_distance_4(a, b):
    return recursive_sum([x + y - Var('2')*(x * y) for x, y in zip(a, b)])
###############################

@coyote.define_circuit(a=vector(16,replicate=True), b=vector(16,replicate=True))
def hamming_distance_16(a, b):
    return recursive_sum([x + y - Var('2')*(x * y) for x, y in zip(a, b)])

#########################################################################################
#########################################################################################
@coyote.define_circuit(image = matrix(4,4,replicate=True))
def box_blur_4x4(image):
    height, width = 4,4
    result = np.empty((height, width), dtype=object)
    result[:] = Var('0')
    ####################
    for i in range(height):
        for j in range(width):
            for ki in range(-1,2): 
                for kj in range(-1,2):
                    ni = i + ki
                    nj = j + kj
                    #Ensure the indices are within the image bounds
                    if (ni >= 0 and ni < height and nj >= 0 and nj < width):
                        result[i, j] = result[i, j] + image[ni][nj]
    return [result[i][j] for i in range(height) for j in range(width)]

###########################################
@coyote.define_circuit(image = matrix(8,8,replicate=True))
def box_blur_8x8(image):
    height, width = 8,8
    result = np.empty((height, width), dtype=object)
    result[:] = Var('0')
    ####################
    for i in range(height):
        for j in range(width):
            for ki in range(-1,2): 
                for kj in range(-1,2):
                    ni = i + ki
                    nj = j + kj
                    #Ensure the indices are within the image bounds
                    if (ni >= 0 and ni < height and nj >= 0 and nj < width):
                        result[i, j] = result[i, j] + image[ni][nj]
    return [result[i][j] for i in range(height) for j in range(width)]
#########################################################################################
#########################################################################################
@coyote.define_circuit(image = matrix(4,4,replicate=True))
def gx_kernel_4x4(image):
    height, width = 4,4  
    result = np.zeros_like(image)
    padded_image = np.pad(image, pad_width=1, mode='constant', constant_values=Var('0'))

    for i in range(1, height + 1):
        for j in range(1, width + 1):
            top_row = padded_image[i-1, j-1:j+2]
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            
            top_sum = top_row[0] +   top_row[2]
            curr_sum = Var('2')*curr_row[0] + Var('2') * curr_row[2]
            bottom_sum = bottom_row[0] + bottom_row[2]

            result[i-1, j-1] = top_sum + curr_sum + bottom_sum

    return [result[i][j] for i in range(height) for j in range(width)]
#############################################
@coyote.define_circuit(image = matrix(8,8,replicate=True))
def gx_kernel_8x8(image):
    height, width = 8,8 
    result = np.zeros_like(image)
    padded_image = np.pad(image, pad_width=1, mode='constant', constant_values=Var('0'))

    for i in range(1, height + 1):
        for j in range(1, width + 1):
            top_row = padded_image[i-1, j-1:j+2]
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            
            top_sum = top_row[0] +   top_row[2]
            curr_sum = Var('2')*curr_row[0] + Var('2') * curr_row[2]
            bottom_sum = bottom_row[0] + bottom_row[2]

            result[i-1, j-1] = top_sum + curr_sum + bottom_sum

    return [result[i][j] for i in range(height) for j in range(width)]

#########################################################################################
#########################################################################################
@coyote.define_circuit(image = matrix(4,4,replicate=True))
def gy_kernel_4x4(image):
    height, width = 4,4
    result = np.zeros_like(image)
    padded_image = np.pad(image, pad_width=1, mode='constant', constant_values=Var('0'))
    for i in range(1, height + 1):
        for j in range(1, width + 1):
            top_row = padded_image[i-1, j-1:j+2]
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            
            top_sum = top_row[0] + Var('2')*top_row[1]  + top_row[2]
            bottom_sum = bottom_row[0] + Var('2')*bottom_row[1] + bottom_row[2]

            result[i-1, j-1] = top_sum + bottom_sum

    return [result[i][j] for i in range(height) for j in range(width)]
#############################################
@coyote.define_circuit(image = matrix(8,8,replicate=True))
def gy_kernel_8x8(image):
    height, width = 8,8
    result = np.zeros_like(image)
    padded_image = np.pad(image, pad_width=1, mode='constant', constant_values=Var('0'))
    for i in range(1, height + 1):
        for j in range(1, width + 1):
            top_row = padded_image[i-1, j-1:j+2]
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            top_sum = top_row[0] + Var('2')*top_row[1]  + top_row[2]
            bottom_sum = bottom_row[0] + Var('2')*bottom_row[1] + bottom_row[2]
            result[i-1, j-1] = top_sum + bottom_sum

    return [result[i][j] for i in range(height) for j in range(width)]
###################################################################################
###################################################################################
@coyote.define_circuit(image = matrix(4,4,replicate=True))
def roberts_cross_4x4(image):
    height, width = 4,4
    ## add padding to the image
    result = np.zeros_like(image)
    padded_image = np.pad(image, pad_width=1, mode='constant', constant_values=Var('0'))
    gx_kernel = [[Var('1'),Var('0')],[Var('0'),Var('-1')]]
    gy_kernel = [[Var('0'),Var('1')],[Var('-1'),Var('0')]]
    for i in range(1,height+1):
        for j in range(1,width+1):
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            Gx = curr_row[1] + bottom_row[2] 
            # Apply Kernel 2 (Gy)
            Gy = curr_row[2] + bottom_row[1]    
            # Compute the gradient magnitude and store in the output array (clipped to 255)
            result[i-1][j-1] = Gx*Gx + Gy*Gy
    return [result[i][j] for i in range(height) for j in range(width)]

###############################
@coyote.define_circuit(image = matrix(8,8,replicate=True))
def roberts_cross_8x8(image):
    height, width = 8,8
    ## add padding to the image
    result = np.zeros_like(image)
    padded_image = np.pad(image, pad_width=1, mode='constant', constant_values=Var('0'))
    gx_kernel = [[Var('1'),Var('0')],[Var('0'),Var('-1')]]
    gy_kernel = [[Var('0'),Var('1')],[Var('-1'),Var('0')]]
    for i in range(1,height+1):
        for j in range(1,width+1):
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            Gx = curr_row[1] + bottom_row[2] 
            # Apply Kernel 2 (Gy)
            Gy = curr_row[2] + bottom_row[1]    
            # Compute the gradient magnitude and store in the output array (clipped to 255)
            result[i-1][j-1] = Gx*Gx + Gy*Gy
    return [result[i][j] for i in range(height) for j in range(width)]

###################################################################################
###################################################################################
@coyote.define_circuit(image = matrix(4,4,replicate=True))
def sobel_4x4(image):
    height, width = 4,4
    result = np.zeros_like(image)
    padded_image = np.pad(image, pad_width=1, mode='constant', constant_values=Var('0'))
    gx_output = result 
    gy_output = result
    gx_kernel = [[Var('-1'),Var('0'),Var('1')],[Var('-2'),Var('0'),Var('2')],[Var('-1'),Var('0'),Var('1')]]
    for i in range(1,height+1):
        for j in range(1,width+1):
            #Traverse the kernel window centered around (i, j)
            top_row = padded_image[i-1, j-1:j+2]
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            top_sum = top_row[0] + top_row[2]
            curr_sum = Var('2')*curr_row[0] + Var('2') * curr_row[2]
            bottom_sum = bottom_row[0] + bottom_row[2]
            gx_output[i-1, j-1] = top_sum + curr_sum + bottom_sum
    ###########################
    gy_kernel = [[Var('-1'),Var('-2'),Var('-1')],[Var('0'),Var('0'),Var('0')],[Var('1'),Var('2'),Var('1')]]
    for i in range(1,height+1):
        for j in range(1,width+1):
            top_row = padded_image[i-1, j-1:j+2]
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            top_sum = top_row[0] + Var('2')*top_row[1]  + top_row[2]
            bottom_sum = bottom_row[0] + Var('2')*bottom_row[1] + bottom_row[2]
            gy_output[i-1, j-1] = top_sum + bottom_sum
    ########################            
    for i in range(height):
        for j in range(width):
            result[i][j]= gx_output[i][j] * gx_output[i][j] + gy_output[i][j] * gy_output[i][j]
    return [result[i][j] for i in range(height) for j in range(width)]

###########################
@coyote.define_circuit(image = matrix(8,8,replicate=True))
def sobel_8x8(image):
    height, width = 8,8
    result = np.zeros_like(image)
    padded_image = np.pad(image, pad_width=1, mode='constant', constant_values=Var('0'))
    gx_output = result 
    gy_output = result
    gx_kernel = [[Var('-1'),Var('0'),Var('1')],[Var('-2'),Var('0'),Var('2')],[Var('-1'),Var('0'),Var('1')]]
    for i in range(1,height+1):
        for j in range(1,width+1):
            #Traverse the kernel window centered around (i, j)
            top_row = padded_image[i-1, j-1:j+2]
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            top_sum = top_row[0] + top_row[2]
            curr_sum = Var('2')*curr_row[0] + Var('2') * curr_row[2]
            bottom_sum = bottom_row[0] + bottom_row[2]
            gx_output[i-1, j-1] = top_sum + curr_sum + bottom_sum
    ###########################
    gy_kernel = [[Var('-1'),Var('-2'),Var('-1')],[Var('0'),Var('0'),Var('0')],[Var('1'),Var('2'),Var('1')]]
    for i in range(1,height+1):
        for j in range(1,width+1):
            top_row = padded_image[i-1, j-1:j+2]
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            top_sum = top_row[0] + Var('2')*top_row[1]  + top_row[2]
            bottom_sum = bottom_row[0] + Var('2')*bottom_row[1] + bottom_row[2]
            gy_output[i-1, j-1] = top_sum + bottom_sum
    ########################            
    for i in range(height):
        for j in range(width):
            result[i][j]= gx_output[i][j] * gx_output[i][j] + gy_output[i][j] * gy_output[i][j]
    return [result[i][j] for i in range(height) for j in range(width)]
###########################################################################################
###########################################################################################
###########################################################################################
@coyote.define_circuit(v1=vector(16, replicate=True), v2=vector(16, replicate=True))
def dot_product_16x16_fully(v1, v2):
    return recursive_sum([a * b for a, b in zip(v1, v2)])

@coyote.define_circuit(c1=vector(1,replicate=True), c2=vector(1,replicate=True), c3=vector(1,replicate=True), c4=vector(1,replicate=True) , 
                        c5=vector(1,replicate=True),c6=vector(1,replicate=True),c7=vector(1,replicate=True),c8=vector(1,replicate=True),
                        c9=vector(1,replicate=True), c10=vector(1,replicate=True), c11=vector(1,replicate=True), c12=vector(1,replicate=True) , 
                        c13=vector(1,replicate=True),c14=vector(1,replicate=True),c15=vector(1,replicate=True),c16=vector(1,replicate=True)
                        )
def test5(c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16):
    return [
            
                    (
                        ((c1[i]*c2[i])+(c3[i]+c4[i]))
                +
                        ((c5[i]*c6[i])*(c7[i]*c8[i]))
                    )
            +
                    (
                        ((c9[i]+c10[i])*(c11[i]+c12[i]))
                +
                        ((c13[i]+c14[i])*(c15[i]*c16[i]))
                    )
            
            for i in range(1)]
"""
###########################################################################################
@coyote.define_circuit(c1=vector(1,replicate=True), c2=vector(1,replicate=True), 
                        c3=vector(1,replicate=True), c4=vector(1,replicate=True))
def test35(c1,c2,c3,c4):
    return [
            ((c1[i]*c2[i])+(c3[i]+c4[i]))
            for i in range(1)
        ]

@coyote.define_circuit(a=vector(4,replicate=True), b=vector(4,replicate=True))
def hamming_distance_4(a, b):
    return [x + y - Var('2')*(x * y) for x, y in zip(a, b)]
#########################################################################################
########################################################################################
with open('benchmarks_evaluation.csv', mode='a', newline='') as csvfile:
    csvwriter = csv.writer(csvfile)
    # Write the header
    csvwriter.writerow(['Benchmark name', 'Compile time (s)',
                        'multiplications',
                        'additions',
                        'substitutions',
                        'rotations',
                        'scalar/plain multiplications',
                        'Depth',
                        'Multiplicative depth'
                        ])

directory_path = 'bfv_backend/coyote_out'
for func in coyote.func_signatures:
    print(f"Compile function : {func.__name__}")
    benchmark_name = func.__name__
    benchmark_dir = os.path.join(directory_path, benchmark_name)

    if not os.path.exists(benchmark_dir):
        start_time = time.time()
        scalar_code = coyote.instantiate(benchmark_name)
        vector_code = list(map(str, coyote.vectorize()))

        end_time = time.time()
        compilation_time = end_time - start_time
        print(f'Compilation time: {compilation_time:.2f} seconds')
        try:
            os.mkdir('outputs')
        except FileExistsError:
            pass

        try:
            os.mkdir(f'outputs/{benchmark_name}')
        except FileExistsError:
            pass 
        
        open(f'outputs/{benchmark_name}/scal', 'w').write('\n'.join(scalar_code))
        open(f'outputs/{benchmark_name}/vec', 'w').write('\n'.join(vector_code))
        print(f'Successfully compiled benchmark {benchmark_name}; outputs placed in outputs/{benchmark_name}!')
        # Start the subprocess and capture stdout and stderr
        result = subprocess.Popen(['python3', 'compile_to_bfv.py', benchmark_name],
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE,
                                text=True)

        # Wait for the subprocess to complete and capture output
        stdout, stderr = result.communicate()
        # Display captured output
        print("welcome back in coyote Benchmarks ==>")
        print(stdout)  # Standard output
        print("endeeeeeeeeeeeeeeeeeeeeee")
        num_multiplications_vec, num_additions_vec, num_substitutions_vec, num_rotations_vec, num_plain_multiplications_vec, max_depth_vec, max_multiplicative_depth_vec = evaluate_vector(benchmark_name)
        num_multiplications_scal, num_additions_scal, num_substitutions_scal, num_rotations_scal, num_plain_multiplications_scal, max_depth_scal, max_multiplicative_depth_scal = evaluate_scalar(benchmark_name)

        with open('benchmarks_evaluation.csv', mode='a', newline='') as csvfile:
            csvwriter = csv.writer(csvfile)
            csvwriter.writerow(
                            [benchmark_name, compilation_time,
                            str(num_multiplications_scal)+" "+str(num_multiplications_vec), 
                            str(num_additions_scal)+" "+str(num_additions_vec), 
                            str(num_substitutions_scal)+" "+str(num_substitutions_vec), 
                            str(num_rotations_scal)+" "+str(num_rotations_vec), 
                            str(num_plain_multiplications_scal)+" "+str(num_plain_multiplications_vec), 
                            str(max_depth_scal)+" "+str(max_depth_vec), 
                            str(max_multiplicative_depth_scal)+" "+str(max_multiplicative_depth_vec)]
                            )

###########################################################################
process = subprocess.Popen(['python3', 'build_and_run_all.py', '--iters=5', '--runs=5'])
process.wait()
csv_directory = 'bfv_backend/csvs'

run_times = {}
for filename in os.listdir(csv_directory):
    if filename.endswith('.csv'):
        benchmark_name = filename[:-4]  # Remove the '.csv' extension to get the benchmark name
        file_path = os.path.join(csv_directory, filename)

        # Read the second row and fourth column from the CSV file
        with open(file_path, mode='r', newline='') as csvfile:
            reader = csv.reader(csvfile)
            rows = list(reader)
            if len(rows) > 1 and len(rows[1]) > 3:  # Ensure there is a second row and fourth column
                run_time = str(rows[1][7])+" "+str(rows[1][3])  # Get the value from the second row and fourth column
                run_times[benchmark_name] = run_time

with open('benchmarks_evaluation.csv', mode='r', newline='') as infile:
    reader = csv.reader(infile)
    headers = next(reader)
    rows = list(reader)

if 'Run time' not in headers:
    headers.append('Run time')

# Update the benchmark evaluation CSV with the run times
with open('benchmarks_evaluation.csv', mode='w', newline='') as outfile:
    writer = csv.writer(outfile)
    writer.writerow(headers)
    for row in rows:
        benchmark_name = row[0]
        if benchmark_name in run_times:
            row.append(run_times[benchmark_name])
        else:
            row.append('')
        writer.writerow(row)


