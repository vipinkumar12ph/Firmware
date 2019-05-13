/*
* teste_actuator.cpp
*
* Created on: 25 de Set de 2015
*
Author: Pedro Silva
*/
#include "teste_actuator.hpp"
#include <systemlib/err.h>
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
msg.valor = output;
printf("msg = %x\n",msg.valor);
(void)_uavcan_pub_teste_cmd.broadcast(msg);
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

