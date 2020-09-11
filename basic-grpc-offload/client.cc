/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>
#include <stdint.h>

#include <grpcpp/grpcpp.h>
#include "fib_rpc.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using fib_rpc::HelloRequest;
using fib_rpc::HelloReply;
using fib_rpc::FibIn;
using fib_rpc::FibOut;
using fib_rpc::Greeter;

uint32_t fib(uint32_t n)
{
    if(n < 2) {
        return n;
    }
    return fib(n-1) + fib(n-2);
}

static inline
struct timespec timespec_diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
        temp.tv_sec = end.tv_sec-start.tv_sec-1;
        temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec-start.tv_sec;
        temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}


class GreeterClient {
public:
    GreeterClient(std::shared_ptr<Channel> channel)
        : stub_(Greeter::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string SayHello(const std::string& user) {
        // Data we are sending to the server.
        HelloRequest request;
        request.set_name(user);

        // Container for the data we expect from the server.
        HelloReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        Status status = stub_->SayHello(&context, request, &reply);

        // Act upon its status.
        if (status.ok()) {
        return reply.message();
        } else {
        std::cout << status.error_code() << ": " << status.error_message()
                    << std::endl;
        return 0;
        }
    }
    uint32_t DoFib(const uint32_t client_send) {
        // Data we are sending to the server.
        // HelloRequest request;
        // request.set_name(user);
        FibIn para_in;
        para_in.set_fib_count(client_send);

        // Container for the data we expect from the server.
        // HelloReply reply;
        FibOut para_out;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        // Status status = stub_->SayHello(&context, request, &reply);
        Status status = stub_->DoFib(&context, para_in, &para_out);

        // Act upon its status.
        if (status.ok()) {
        return para_out.fib_back();
        } else {
        std::cout << status.error_code() << ": " << status.error_message()
                    << std::endl;
        return 0;
        }
    }
private:
    std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** argv) {
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint (in this case,
    // localhost at port 50051). We indicate that the channel isn't authenticated
    // (use of InsecureChannelCredentials()).
    //
    struct timespec start, finish;
    uint32_t res;

    clock_gettime(CLOCK_REALTIME, &start);
    res =  fib(40);
    clock_gettime(CLOCK_REALTIME, &finish);

    struct timespec delta = timespec_diff(start, finish);

    //unsigned ms = (delta.tv_sec*1000) + (delta.tv_nsec/1000000);
    //printf("%d [%u ms]\n", res, ms);

    double fms = (delta.tv_sec*1000.0) + (delta.tv_nsec/1000000.0);
    printf("Native res: %d, time [%.3f ms]\n", res, fms);

    GreeterClient greeter(grpc::CreateChannel(
        "127.0.0.1:50051", grpc::InsecureChannelCredentials()));
    // std::string user("world");
    // std::string reply = greeter.SayHello(user);
    // std::cout << "Greeter received: " << reply << std::endl;
    clock_gettime(CLOCK_REALTIME, &start);
    uint32_t reply = greeter.DoFib(40);
    clock_gettime(CLOCK_REALTIME, &finish);
    delta = timespec_diff(start, finish);
    fms = (delta.tv_sec*1000.0) + (delta.tv_nsec/1000000.0);
    printf("Offload res: %d, time [%.3f ms]\n", res, fms);
    std::cout << "Greeter received: " << reply << std::endl;
    return 0;
}
