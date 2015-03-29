#include "PacketBaseAdapter.h"

mfunc_item<NETInputPacketAdapter> NETInputPacketAdapter::mfuncs[8] = {
    {"GetCmdType", &NETInputPacketAdapter::GetCmdType},
    {"ReadInt",    &NETInputPacketAdapter::ReadInt},
    {"ReadIntDel", &NETInputPacketAdapter::ReadIntDel},
    {"ReadShort",  &NETInputPacketAdapter::ReadShort},
    {"ReadByte",   &NETInputPacketAdapter::ReadByte},
    {"ReadString", &NETInputPacketAdapter::ReadString},
    {"Copy",       &NETInputPacketAdapter::Copy},
    {NULL, NULL}
};

mfunc_item<NETOutputPacketAdapter> NETOutputPacketAdapter::mfuncs[11] = {
    {"packet_buf",  &NETOutputPacketAdapter::packet_buf},
    {"packet_size", &NETOutputPacketAdapter::packet_size},
    {"WriteInt",    &NETOutputPacketAdapter::WriteInt},
    {"WriteByte",   &NETOutputPacketAdapter::WriteByte},
    {"WriteShort",  &NETOutputPacketAdapter::WriteShort},
    {"InsertInt",   &NETOutputPacketAdapter::InsertInt},
    {"InsertByte",  &NETOutputPacketAdapter::InsertByte},
    {"WriteString", &NETOutputPacketAdapter::WriteString},
    {"Begin",       &NETOutputPacketAdapter::Begin},
    {"End",         &NETOutputPacketAdapter::End},
    {NULL, NULL}
};
