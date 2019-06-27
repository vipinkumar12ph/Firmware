/*
* teste_actuator.cpp
*
* Created on: 25 de Set de 2015
*
Author: Pedro Silva
*/
#include "teste_actuator.hpp"
#include <systemlib/err.h>
#include <string.h>
#include <math.h>
#include <cstdio>

int convert(int value);
UavcanTeste::UavcanTeste(uavcan::INode &node) :_node(node),_uavcan_pub_teste_cmd(node),
    _uavcan_pub_servo_zero(node),_uavcan_pub_servo_one(node),_uavcan_pub_servo_two(node),
    _uavcan_pub_servo_three(node),_uavcan_sub_status(node)
{
}
UavcanTeste::~UavcanTeste()
{
}
int UavcanTeste::init()
{
    int res = _uavcan_sub_status.start(StatusCbBinder(this,&UavcanTeste::teste_receive_sub_cb));
    if (res < 0)
    {
        warnx("uavcan sub failed %i", res);
        return res;
        }
    return res;
    }
void UavcanTeste::update_outputs(int output)
{
    const auto timestamp = _node.getMonotonicTime();
    if ((timestamp - _prev_cmd_pub).toUSec() < (1000000 / MAX_RATE_HZ)) {
        return;
        }
_prev_cmd_pub = timestamp;
uavcan::equipment::test_can::TesteCommand msg;

//printf("%x",ouput);
//formation of can frame
int speed = convert(output);
//conver value in hex

msg.valor = speed;

printf("msg = %x\n",msg.valor);
(void)_uavcan_pub_teste_cmd.broadcast(msg);
}

void UavcanTeste::update_outputs_servo(int id,float servo_angle)
{
    const auto timestamp = _node.getMonotonicTime();
    if ((timestamp - _prev_cmd_pub).toUSec() < (1000000 / MAX_RATE_HZ)) {
        return;
        }
_prev_cmd_pub = timestamp;

uavcan::equipment::servo_param::ServoZero servo_angle_zero;
uavcan::equipment::servo_param::ServoOne servo_angle_One;
uavcan::equipment::servo_param::ServoTwo servo_angle_Two;
uavcan::equipment::servo_param::ServoThree servo_angle_Three;

switch (id) {
case 0:
    servo_angle_zero.angle = servo_angle;
    (void)_uavcan_pub_servo_zero.broadcast(servo_angle_zero);
    break;
case 1:
    servo_angle_One.angle = servo_angle;
    (void)_uavcan_pub_servo_one.broadcast(servo_angle_One);
    break;
case 2:
    servo_angle_Two.angle = servo_angle;
    (void)_uavcan_pub_servo_two.broadcast(servo_angle_Two);
    break;
case 3:
    servo_angle_Three.angle = servo_angle;
    (void)_uavcan_pub_servo_three.broadcast(servo_angle_Three);
    break;
default:
    break;
}
}


int convert(int value){
    char a[3]="30";
    char c[100];
    sprintf(c,"%s%0x",a,value);

    int i, len, dec = 0;
    int r = 1;
    len = strlen(c);
    for (i = 0; c[i] != '\0'; i++)
    {
     len--;
     if(c[i] >= '0' && c[i] <= '9')
      r = c[i] - 48;
     else if(c[i] >= 'a' && c[i] <= 'f')
      r = c[i] - 87;
     else if(c[i] >= 'A' && c[i] <= 'F')
      r = c[i] - 55;
     dec += r * pow(16,len);
    }
    //printf("%d",dec);

    return(dec);
}

void UavcanTeste::teste_receive_sub_cb(const uavcan::ReceivedDataStructure<uavcan::equipment::test_can::TesteReceive> &msg)
{
    _teste_receive.inc=msg.recebe;
    uint8_t node= msg.getSrcNodeID().get();
    printf("node=%d\n",node);
    if (_teste_receive_pub != nullptr)
    {
        (void)orb_publish(ORB_ID(teste_receive), _teste_receive_pub,&_teste_receive);
        }
    else
    {
        _teste_receive_pub = orb_advertise(ORB_ID(teste_receive),&_teste_receive);
        }
    }

