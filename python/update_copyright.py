# Script to go through the source files and update the copyright info

import os

filetypes = [".cpp", ".h"]

current_directory = os.getcwd()

# recurse through current directory and find all the source files

for root, dir, files in os.walk(current_directory):
    for filename in files:
        full_filename = root + "/" + filename

        match = False
        for t in filetypes:
            if full_filename.endswith(t):
                match = True
#        if match:
#            file = 
