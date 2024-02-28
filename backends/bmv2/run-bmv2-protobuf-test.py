#!/usr/bin/env python3

import argparse
import logging
import os
import random
import sys
import tempfile
import time
import uuid
from google.protobuf import text_format
from datetime import datetime
from pathlib import Path

# tools_path = os.path.join(os.environ['PROJECT_PATH'], 'tools')
# sys.path.append(tools_path)
# ebpfenv_path = os.path.join(os.environ['PROJECT_PATH'], 'backends', 'ebpf', 'targets')
# sys.path.append(ebpfenv_path)

import grpc
from p4.tmp import p4config_pb2
from p4.v1 import p4runtime_pb2, p4runtime_pb2_grpc

PARSER = argparse.ArgumentParser()
PARSER.add_argument(
    "rootdir",
    help="The root directory of the compiler source tree."
    "This is used to import P4C's Python libraries",
)
PARSER.add_argument("p4_file", help="the p4 file to process")
PARSER.add_argument(
    "-tf",
    "--testfile",
    dest="testfile",
    help="The path for the .txtpb file for this test.",
)
PARSER.add_argument(
    "-td",
    "--testdir",
    dest="testdir",
    help="The location of the test directory.",
)
PARSER.add_argument(
    "-b",
    "--nocleanup",
    action="store_true",
    dest="nocleanup",
    help="Do not remove temporary results for failing tests.",
)
PARSER.add_argument(
    "-n",
    "--num_ifaces",
    default=8,
    dest="interface_count",
    help="How many virtual interfaces to create.",
)
PARSER.add_argument(
    "-ll",
    "--log_level",
    dest="log_level",
    default="WARNING",
    choices=["CRITICAL", "ERROR", "WARNING", "INFO", "DEBUG", "NOTSET"],
    help="The log level to choose.",
)

# Parse options and process argv
ARGS, ARGV = PARSER.parse_known_args()

# Append the root directory to the import path.
FILE_DIR = Path(__file__).resolve().parent
ROOT_DIR = Path(ARGS.rootdir).absolute()
sys.path.append(str(ROOT_DIR))

from backends.ebpf.targets.ebpfenv import Bridge  # pylint: disable=wrong-import-position
from tools import testutils  # pylint: disable=wrong-import-position

# 9559 is the default P4Runtime API server port
P4RUNTIME_PORT: int = 9559
THRIFT_PORT: int = 22000
GRPC_ADDRESS: str = "0.0.0.0"


class Options:
    """Options for this testing script. Usually correspond to command line inputs."""

    # File that is being compiled.
    p4_file: Path = Path(".")
    # Path to .txtpb test file that is used.
    testfile: Path = Path(".")
    # Actual location of the test framework.
    testdir: Path = Path(".")
    # The base directory where tests are executed.
    rootdir: Path = Path(".")
    # The number of interfaces to create for this particular test.
    interface_count: int = 8


class ProtobufTestEnv:
    options: Options = Options()
    switch_proc: testutils.Optional[testutils.subprocess.Popen] = None

    def __init__(self, options: Options) -> None:
        self.options = options

    def __del__(self) -> None:
        if self.switch_proc:
            # Terminate the switch process and emit its output in case of failure.
            testutils.kill_proc_group(self.switch_proc)

    def create_bridge(self, interface_count: int) -> Bridge:
        """Create a network namespace environment."""
        testutils.log.info(
            "---------------------- Creating a namespace ----------------------",
        )
        random.seed(datetime.now().timestamp())
        bridge = Bridge(str(uuid.uuid4()))
        result = bridge.create_virtual_env(interface_count)
        if result != testutils.SUCCESS:
            bridge.ns_del()
            testutils.log.error(
                "---------------------- Namespace creation failed ----------------------",
            )
            raise SystemExit("Unable to create the namespace environment.")
        testutils.log.info(
            "---------------------- Namespace successfully created ----------------------"
        )
        return bridge

    def compile_program(self, json_name: Path, info_name: Path) -> int:
        """Compile the input P4 program using p4c-bm2-ss."""
        testutils.log.info("---------------------- Compile with p4c-bm2-ss ----------------------")
        compilation_cmd = (
            f"{self.options.rootdir}/build/p4c-bm2-ss --target bmv2 --arch v1model "
            f"--p4runtime-files {info_name} {self.options.p4_file} -o {json_name}"
        )
        _, returncode = testutils.exec_process(compilation_cmd, timeout=30)
        if returncode != testutils.SUCCESS:
            testutils.log.error("Failed to compile the P4 program %s.", self.options.p4_file)
        return returncode

    def run_simple_switch_grpc(
        self, switchlog: Path, grpc_port: int
    ) -> testutils.Optional[testutils.subprocess.Popen]:
        raise NotImplementedError("method run_simple_switch_grpc not implemented for this class")

    def run_switch_test(self, grpc_port: int, json_name: Path, info_name: Path) -> int:
        raise NotImplementedError("method run_switch_test not implemented for this class")


class VethEnv(ProtobufTestEnv):
    bridge: testutils.Optional[Bridge] = None

    def __init__(self, options: Options) -> None:
        super().__init__(options)
        # Create the virtual environment for the test execution.
        self.bridge = self.create_bridge(options.interface_count)

    def __del__(self) -> None:
        if self.bridge:
            self.bridge.ns_del()
        super().__del__()

    def get_interface_map(self, interface_count: int, prefix: str = "") -> str:
        """Produce the interface arguments based on the number of interfaces the test uses."""
        iface_str = ""
        for iface_num in range(interface_count):
            iface_str += f"-i {iface_num}@{prefix}{iface_num} "
        return iface_str

    def run_simple_switch_grpc(
        self, switchlog: Path, grpc_port: int
    ) -> testutils.Optional[testutils.subprocess.Popen]:
        """Start simple_switch_grpc and return the process handle."""
        testutils.log.info(
            "---------------------- Start simple_switch_grpc ----------------------",
        )
        interface_map = self.get_interface_map(interface_count=self.options.interface_count)
        thrift_port = testutils.pick_tcp_port(GRPC_ADDRESS, THRIFT_PORT)
        simple_switch_grpc = (
            f"simple_switch_grpc --thrift-port {thrift_port} --device-id 0 --log-file {switchlog} "
            f"{interface_map} --log-flush --no-p4 "
            f"-- --grpc-server-addr {GRPC_ADDRESS}:{grpc_port}"
        )
        if self.bridge:
            bridge_cmd = self.bridge.get_ns_prefix() + " " + simple_switch_grpc
            self.switch_proc = testutils.open_process(bridge_cmd)
            return self.switch_proc
        return None

    def run_switch_test(self, grpc_port: int, json_name: Path, info_name: Path) -> int:
        channel = grpc.insecure_channel(f'localhost:{grpc_port}')  
        stub = p4runtime_pb2_grpc.P4RuntimeStub(channel)  # <class 'p4.v1.p4runtime_pb2_grpc.P4RuntimeStub'>
        testutils.log.info("Sending P4 config from file %s with P4info %s", json_name, info_name)
        request = p4runtime_pb2.SetForwardingPipelineConfigRequest()  # <class 'p4.v1.p4runtime_pb2.SetForwardingPipelineConfigRequest'>
        
        # The following fields are generated/assumed based upon https://github.com/p4lang/tutorials/blob/master/utils/p4runtime_lib/switch.py
        # and /home/zz3470/p4c-zzmic/build/_deps/p4runtime-src/proto/p4/v1/p4runtime.proto
        device_id = 1
        request.device_id = device_id
        config = request.config
        
        try:
            with open(info_name, "r", encoding="utf-8") as p4info_f:
                text_format.Merge(p4info_f.read(), config.p4info, allow_unknown_field=True)
        except Exception as e:
            testutils.log.error("Failed to parse P4Info file: %s", e)
            return testutils.FAILURE

        try:
            with open(json_name, "rb") as config_f:
                config.p4_device_config = config_f.read()
        except Exception as e:
            testutils.log.error("Failed to read P4 device config file: %s", e)
            return testutils.FAILURE

        request.action = p4runtime_pb2.SetForwardingPipelineConfigRequest.VERIFY_AND_COMMIT
        try:
            response = stub.SetForwardingPipelineConfig(request)
            logging.debug("Response %s", response)
        except Exception as e:
            logging.error("Error during SetForwardingPipelineConfig")
            logging.error(e)
            return testutils.FAILURE
        return testutils.SUCCESS        
        
def run_test(options: Options) -> int:
    """Define the test environment and compile the P4 target
    Optional: Run the generated model"""
    test_name = Path(options.p4_file.name)
    json_name = options.testdir.joinpath(test_name.with_suffix(".json"))
    info_name = options.testdir.joinpath(test_name.with_suffix(".p4info.txtpb"))

    testenv = VethEnv(options)

    # Compile the P4 program.
    returncode = testenv.compile_program(json_name, info_name)
    if returncode != testutils.SUCCESS:
        return returncode

    # Pick available ports for the gRPC switch.
    switchlog = options.testdir.joinpath("switchlog")
    grpc_port = testutils.pick_tcp_port(GRPC_ADDRESS, P4RUNTIME_PORT)
    switch_proc = testenv.run_simple_switch_grpc(switchlog, grpc_port)
    if switch_proc is None:
        return testutils.FAILURE
    # Breath a little.
    time.sleep(1)
    # Run the Protobuf test and retrieve the result.
    try:
        result = testenv.run_switch_test(grpc_port, json_name, info_name)
    except Exception as e:
        testutils.log.error("Failed to run switch test: %s", e)
        result = testutils.FAILURE
    # Delete the test environment and trigger a clean up.
    del testenv
    # Print switch log if the results were not successful.
    if result != testutils.SUCCESS:
        if switchlog.with_suffix(".txt").exists():
            switchout = switchlog.with_suffix(".txt").read_text()
            testutils.log.error("######## Switch log ########\n%s", switchout)
        if switch_proc.stdout:
            out = switch_proc.stdout.read()
            # Do not bother to print whitespace.
            if out.strip():
                testutils.log.error("######## Switch output ######## \n%s", out)
        if switch_proc.stderr:
            err = switch_proc.stderr.read()
            # Do not bother to print whitespace.
            if err.strip():
                testutils.log.error("######## Switch errors ######## \n%s", err)
    return result


def create_options(test_args: testutils.Any) -> testutils.Optional[Options]:
    """Parse the input arguments and create a processed options object."""
    options = Options()
    result = testutils.check_if_file(test_args.p4_file)
    if not result:
        return None
    options.p4_file = result
    testfile = test_args.testfile
    if not testfile:
        testutils.log.info("No test file provided. Checking for file in folder.")
        testfile = options.p4_file.with_suffix(".py")
    result = testutils.check_if_file(testfile)
    if not result:
        return None
    options.testfile = result
    testdir = test_args.testdir
    if not testdir:
        testutils.log.info("No test directory provided. Generating temporary folder.")
        testdir = tempfile.mkdtemp(dir=Path(".").absolute())
        # Generous permissions because the program is usually edited by sudo.
        os.chmod(testdir, 0o755)
    options.testdir = Path(testdir)
    options.rootdir = Path(test_args.rootdir)
    options.interface_count = test_args.interface_count
    # Configure logging.
    logging.basicConfig(
        filename=options.testdir.joinpath("test.log"),
        format="%(levelname)s: %(message)s",
        level=getattr(logging, test_args.log_level),
        filemode="w",
    )
    stderr_log = logging.StreamHandler()
    stderr_log.setFormatter(logging.Formatter("%(levelname)s: %(message)s"))
    logging.getLogger().addHandler(stderr_log)
    return options


if __name__ == "__main__":
    test_options = create_options(ARGS)
    if not test_options:
        sys.exit(testutils.FAILURE)

    if not testutils.check_root():
        testutils.log.error("This script requires root privileges; Exiting.")
        sys.exit(1)

    # Run the test with the extracted options
    test_result = run_test(test_options)
    if not (ARGS.nocleanup or test_result != testutils.SUCCESS):
        testutils.log.info("Removing temporary test directory.")
        # testutils.del_dir(str(test_options.testdir))
    sys.exit(test_result)

