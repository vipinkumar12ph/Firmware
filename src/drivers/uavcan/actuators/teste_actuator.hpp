#pragma once

#include <uavcan/uavcan.hpp>
#include <uavcan/equipment/test_can/TesteCommand.hpp>
#include <uavcan/equipment/test_can/TesteReceive.hpp>
#include <uavcan/equipment/servo_param/ServoZero.hpp>
#include <uavcan/equipment/servo_param/ServoOne.hpp>
#include <uavcan/equipment/servo_param/ServoTwo.hpp>
#include <uavcan/equipment/servo_param/ServoThree.hpp>
#include <lib/perf/perf_counter.h>
#include <uORB/topics/teste_topic.h>
#include <uORB/topics/motor_param_in.h>
#include <uORB/topics/servo_param_in.h>
#include <uORB/topics/teste_receive.h>

class UavcanTeste
{
public:
    UavcanTeste(uavcan::INode &node);
    ~UavcanTeste();

    int init();

    void update_outputs(int output);
    void update_outputs_servo(int id,float servo_angle);
private:
/*
* TesteReceive message reception will be reported via this callback.
*/
void teste_receive_sub_cb(const uavcan::ReceivedDataStructure<uavcan::equipment::test_can::TesteReceive> &msg);

typedef uavcan::MethodBinder<UavcanTeste *,

void (UavcanTeste::*)(const uavcan::ReceivedDataStructure<uavcan::equipment::test_can::TesteReceive>&)> StatusCbBinder;
/*
* libuavcan related things
*/
uavcan::MonotonicTime _prev_cmd_pub;
///< rate limiting
uavcan::INode &_node;
uavcan::Publisher<uavcan::equipment::test_can::TesteCommand> _uavcan_pub_teste_cmd;
uavcan::Publisher<uavcan::equipment::servo_param::ServoZero> _uavcan_pub_servo_zero;
uavcan::Publisher<uavcan::equipment::servo_param::ServoOne> _uavcan_pub_servo_one;
uavcan::Publisher<uavcan::equipment::servo_param::ServoTwo> _uavcan_pub_servo_two;
uavcan::Publisher<uavcan::equipment::servo_param::ServoThree> _uavcan_pub_servo_three;
uavcan::Subscriber<uavcan::equipment::test_can::TesteReceive, StatusCbBinder> _uavcan_sub_status;







static constexpr unsigned MAX_RATE_HZ = 200;
struct teste_receive_s _teste_receive;
orb_advert_t _teste_receive_pub = nullptr;
};
