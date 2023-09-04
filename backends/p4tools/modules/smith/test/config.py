#!/usr/bin/python3

import os
import sys

TARGET = "tofino"
ARCH = "tna"
STD = "p4-16"
MAX_P4S = 5

SMITH_TIMEOUT = 60
P4C_TIMEOUT = 600
TESTGEN_TIMEOUT = 600
PTF_TIMEOUT = 600

ROOT_PATH = os.path.abspath(os.path.dirname(__file__)).replace('/testgen/smith/test', '')
P4C = os.path.join(ROOT_PATH, "compilers/bf-p4c-compilers/build/p4c/bf-p4c")
P4CHECK = os.path.join(ROOT_PATH, "compilers/bf-p4c-compilers/build/p4check")
PTFRUNNER = os.path.join(ROOT_PATH, "compilers/bf-p4c-compilers/p4-tests/ptf_runner.py")

# python3 /mnt/compilers/bf-p4c-compilers/p4-tests/ptf_runner.py --testdir /mnt/compilers/bf-p4c-compilers/build/testgen/testgen-bfp4c-tofino-ptf/p4c-1179.out --name p4c-1179 --ptfdir /mnt/compilers/bf-p4c-compilers/build/testgen/testgen-bfp4c-tofino-ptf/p4c-1179.out --top-builddir /mnt/compilers/bf-p4c-compilers/build/CMakeCache.txt --device tofino --bfrt-test /mnt/compilers/bf-p4c-compilers/build/testgen/testgen-bfp4c-tofino-ptf/p4c-1179.out/p4c-1179.conf
OUTPUT_PATH = os.path.join(ROOT_PATH, "testgen/smith/test/output")
print("OUTPUT_PATH: ", OUTPUT_PATH)
STD_INC_1 = os.path.join(ROOT_PATH, "compilers/bf-p4c-compilers/bf-p4c/p4include")
STD_INC_2 = os.path.join(ROOT_PATH, "compilers/bf-p4c-compilers/p4c/p4include")

SMITH_OPTS = [P4CHECK, "smith"]
P4C_OPTS = [P4C, "-g", "--verbose 1"]
TESTGEN_OPTS = [
    P4CHECK,
    "testgen",
    "--test-backend PTF",
    "--std p4-16",
    "--max-tests 30",
    "--packet-size 4096",
    "-I",
    STD_INC_1,
    "-I",
    STD_INC_2,
]
PTF_OPTS = [PTFRUNNER, "--top-builddir /mnt/compilers/bf-p4c-compilers/build/CMakeCache.txt"]


class Test(object):
    def __init__(self, test_num, p4name, target, arch):
        self.test_num = test_num
        self.p4name = p4name
        self.p4 = self.p4name + '.p4'
        self.test_path = ''
        self.p4_path = ''
        self.include_path = ''
        self.p4_opts = ''
        self.target = target
        self.std = STD
        self.arch = arch
        self.out_path = ''
        self.smith_cmd = ''
        self.smith_out_path = ''
        self.p4c_cmd = ''
        self.p4c_result = None
        self.testgen_cmd = ''
        self.testgen_out = ''
        self.ptf_path = self.p4name + '.py'
        self.ptf_cmd = ''
        self.ptf_out = ''
        self.ptf_result = None

    def display(self):
        print('P4: ', self.p4)
        print('Target: ', self.target)
        print('Arch: ', self.arch)
