# Script to go through the source files and update the copyright info

import os

filetypes = [".cpp", ".h"]
header = [  "// [CopyrightNotice]\n",
            "// Copyright (c) 2019-2021, Martin Linklater\n",
            "// All rights reserved.\n",
            "//\n",
            "// See file 'LICENSE' for license details\n",
            "\n"]

current_directory = os.getcwd()

# recurse through current directory and find all the source files

for root, dir, files in os.walk(current_directory):
    for filename in files:
        full_filename = root + "/" + filename

        match = False
        for t in filetypes:
            if full_filename.endswith(t):
                match = True
        if match:
            file = open(full_filename, "r")
            lines = file.readlines()
            file.close()

            needsSave = False

            if "[CopyrightNotice]" in lines[0]:
                # present - replace
                for i in range(0, len(header)):
                    if lines[i] != header[i]:
                        lines[i] = header[i]
                        needsSave = True
            else:
                # prepend
                needsSave = True
                for i in range(0, len(header)):
                    lines.insert(i, header[i])

            if needsSave:
                file = open(full_filename, "w")
                lines = file.writelines(lines)
                file.close()

        print("done")
