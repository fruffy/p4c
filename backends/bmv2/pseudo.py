#!/usr/bin/env python3

import sys
import grpc
from p4.v1 import p4runtime_pb2, p4runtime_pb2_grpc

def set_forwarding_pipeline_config(config_path, p4info_path, grpc_addr, device_id):
    channel = grpc.insecure_channel(grpc_addr)
    stub = p4runtime_pb2_grpc.P4RuntimeStub(channel)

    request = p4runtime_pb2.SetForwardingPipelineConfigRequest()
    request.device_id = device_id
    request.election_id.high = 0
    request.election_id.low = 1
    
    config = request.config
    with open(p4info_path, 'rb') as p4info_f:
        config.p4info.ParseFromString(p4info_f.read())
    with open(config_path, 'rb') as config_f:
        config.p4_device_config = config_f.read()
    request.action = p4runtime_pb2.SetForwardingPipelineConfigRequest.VERIFY_AND_COMMIT

    try:
        response = stub.SetForwardingPipelineConfig(request)
        print("SetForwardingPipelineConfig was successful.")
    except grpc.RpcError as e:
        print(f"Failed to set forwarding pipeline config: {e}")

if __name__ == "__main__":
    p4info_path = ""
    config_path = ""
    grpc_addr = ""
    device_id = ""
    set_forwarding_pipeline_config(config_path=config_path, p4info_path=p4info_path, grpc_addr=grpc_addr, device_id=device_id)