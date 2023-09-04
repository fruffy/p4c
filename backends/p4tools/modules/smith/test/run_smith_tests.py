#!/usr/bin/env python3

import os
import sys
import argparse
import config
import subprocess
import shutil
from runner import RunCmd
from datetime import datetime


# Utility functions
def writeblock_to_file(filename, fileblock):
    with open(filename, 'w') as fwrite:
        if fileblock:
            for line in iter(fileblock.readline, b''):
                fwrite.write(line.decode('utf-8'))


def appendblock_to_file(filename, fileblock):
    with open(filename, 'a') as fwrite:
        if fileblock:
            for line in iter(fileblock.readline, b''):
                fwrite.write(line.decode('utf-8'))


def print_log(filename):
    with open(filename, 'r') as fread:
        for line in fread.readlines():
            print(line.strip())


def process_p4c_logs(mTest):
    mTest.p4c_result = False
    with open(mTest.p4c_out, 'r') as fread:
        for line in fread.readlines():
            if 'Number of ERRORs: 0' in line:
                mTest.p4c_result = True
                break


def process_ptf_logs(mTest):
    mTest.ptf_result = False
    with open(mTest.ptf_out, 'r') as fread:
        for line in fread.readlines():
            if 'INFO:PTF runner:SUCCESS' in line:
                mTest.ptf_result = True
                break


def gen_smith(mTest):
    mTest.p4_path = os.path.join(mTest.test_path, mTest.p4)
    smith_opts = [
        "--target",
        config.TARGET,
        "--smithArch",
        config.ARCH,
        "--seed",
        str(mTest.test_num + 1000),
        "--output",
        mTest.p4_path,
    ]
    mTest.smith_cmd = ' '.join(config.SMITH_OPTS + smith_opts)
    mTest.smith_out = os.path.join(mTest.test_path, mTest.p4name + '_smith.log')
    print('Smith command: ', mTest.smith_cmd)
    rc = RunCmd(mTest.smith_cmd, config.SMITH_TIMEOUT)
    writeblock_to_file(mTest.smith_out, rc.err)
    appendblock_to_file(mTest.smith_out, rc.out)


def run_smith(mTest):
    gen_smith(mTest)
    mTest.out_path = os.path.join(mTest.test_path, mTest.p4name + '.out')
    p4c_opts = [
        "--target",
        config.TARGET,
        "--arch",
        config.ARCH,
        mTest.p4_path,
        "-o",
        mTest.out_path,
    ]
    mTest.p4c_cmd = ' '.join(config.P4C_OPTS + p4c_opts)
    mTest.p4c_out = os.path.join(mTest.test_path, mTest.p4name + '_p4c.log')
    print('P4C command: ', mTest.p4c_cmd)
    rc = RunCmd(mTest.p4c_cmd, config.P4C_TIMEOUT)
    writeblock_to_file(mTest.p4c_out, rc.err)
    appendblock_to_file(mTest.p4c_out, rc.out)
    process_p4c_logs(mTest)
    return mTest.p4c_result


def gen_ptf(mTest):
    testgen_opts = [
        "--target",
        config.TARGET,
        "--arch",
        config.ARCH,
        "--seed",
        str(mTest.test_num + 1000),
        "--out-dir",
        mTest.test_path,
        mTest.p4_path,
    ]
    mTest.testgen_cmd = ' '.join(config.TESTGEN_OPTS + testgen_opts)
    mTest.testgen_out = os.path.join(mTest.test_path, mTest.p4name + '_testgen.log')
    print('Testgen command: ', mTest.testgen_cmd)
    rc = RunCmd(mTest.testgen_cmd, config.TESTGEN_TIMEOUT)
    writeblock_to_file(mTest.testgen_out, rc.err)
    appendblock_to_file(mTest.testgen_out, rc.out)


def run_testgen(mTest):
    gen_ptf(mTest)
    ptf_opts = [
        "--testdir",
        mTest.out_path,
        "--name",
        mTest.p4name,
        "--ptfdir",
        mTest.test_path,
        "--device",
        config.TARGET,
        "--bfrt-test",
        os.path.join(mTest.out_path, mTest.p4name + '.conf'),
    ]
    mTest.ptf_cmd = ' '.join(config.PTF_OPTS + ptf_opts)
    mTest.ptf_out = os.path.join(mTest.test_path, mTest.p4name + '_ptf.log')
    print('PTF command: ', mTest.ptf_cmd)
    rc = RunCmd(mTest.ptf_cmd, config.PTF_TIMEOUT)
    writeblock_to_file(mTest.ptf_out, rc.err)
    appendblock_to_file(mTest.ptf_out, rc.out)
    process_ptf_logs(mTest)
    return mTest.ptf_result


# Run smith, generate p4s and compile with bf-p4c
# - On success: generate PTF using testgen, and run the tests.
# - On failure: record failure.
def main():
    # Parse command line arguments
    parser = argparse.ArgumentParser(
        description='Generate p4smith programs (and PTF tests on success).'
    )
    parser.add_argument('--target', type=str, default=config.TARGET, help='Target (tofino/tofino2)')
    parser.add_argument('--arch', type=str, default=config.ARCH, help='Arch (tna/t2na)')
    parser.add_argument(
        '--max_p4s',
        type=int,
        default=config.MAX_P4S,
        help='Max number of p4smith programs to be generated.',
    )

    args = parser.parse_args()

    print('=' * 120)
    ts = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
    config.OUTPUT_PATH = os.path.join(config.OUTPUT_PATH, ts)
    os.mkdir(os.path.join(config.OUTPUT_PATH))
    print(ts, 'Begin Test Suite: ')
    print(ts, 'Outputs at: ', config.OUTPUT_PATH)
    for i in range(args.max_p4s):
        ts = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
        print(ts, 'Iteration started: ', str(i + 1))
        print(ts, 'Generating and compiling p4 ...')
        mTest = config.Test((i + 1), "prog" + str(i + 1), args.target, args.arch)
        mTest.test_path = os.path.join(config.OUTPUT_PATH, mTest.p4name)
        os.mkdir(mTest.test_path)
        run_smith(mTest)
        ts = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
        print(ts, 'Compilation log at:', mTest.p4c_out)
        if mTest.p4c_result:
            print(ts, 'Compilation success!')
            print(ts, 'Generating and running PTF ...')
            run_testgen(mTest)
            ts = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
            print(ts, 'PTF log at: ', mTest.ptf_out)
            if mTest.ptf_result:
                print(ts, 'PTF success!')
            else:
                print(ts, 'PTF failed!')
        else:
            print(ts, 'Compilation failed!')
        print(ts, 'Iteration completed:', str(i + 1))
        print('-' * 120)
    print('=' * 120)
    print(ts, 'End Test Suite: ')


if __name__ == "__main__":
    main()
