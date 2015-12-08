__author__ = 'Albee'
import sys
import os
import os.path

def read_one_file(filename):
    l = []
    input_file = open(filename, 'r')
    lines = input_file.readlines()
    for line in lines:
        s = ""
        for ele in line.split():
            for i in range(len(ele)-1):
                if ele[i] == ".":
                    ele = ele[:i]+ele[i+1:]
            if ele.isdigit():
                ele += '0'
                while ele[0] == "0" and len(ele) > 1:
                    ele = ele[1:]
            if len(s) == 0:
                s += ele.lower()
            else:
                s += " "+ele.lower()
        l.append(s)
        print(s)
    return l


def write_one_file(l,title):
    """argv[2] is the out put file"""
    print(l)
    output_file = open(sys.argv[2]+'/'+title, 'w')
    for i in l:
        output_file.write(i+"\n")


def write_all(root_dir):
    s = ""
    for file_name in sorted(os.listdir(root_dir)):
            if "TXT" in file_name:
                temp_list = read_one_file(root_dir+'/'+file_name)
                title = file_name[:-4]+".lab"
                write_one_file(temp_list, title)



"""argv[3] is the root file"""
write_all(sys.argv[1])

