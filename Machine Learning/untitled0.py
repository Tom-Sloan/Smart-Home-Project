# -*- coding: utf-8 -*-
"""
Created on Fri Jun  7 15:04:15 2019

@author: tsloa
"""

i = 0
my_data_file = open('data.txt', 'x')
my_data_file.close()

my_data_file = open('data.txt', 'a')
my_data_file.write(str(i))
my_data_file.write('\n')
my_data_file.close()
i += 1
my_data_file.close()