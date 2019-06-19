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
UavcanTeste::UavcanTeste(uavcan::INode &node) :_node(node),_uavcan_pub_teste_cmd(node),_uavcan_sub_status(node)
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

