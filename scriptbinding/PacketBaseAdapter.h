#ifndef PACKETBASEADAPTER_HEADER
#define PACKETBASEADAPTER_HEADER

#include "PacketBase.h"
#include "luaport.h"

class NETInputPacketAdapter : public NETInputPacket
{
public:
    int GetCmdType(lua_State* L) {
        lua_pushnumber(L, NETInputPacket::GetCmdType());
        return 1;
    }
    int ReadInt(lua_State* L) {
        lua_pushnumber(L, NETInputPacket::ReadInt());
        return 1;
    }
    int ReadIntDel(lua_State* L) {
        lua_pushnumber(L, NETInputPacket::ReadIntDel());
        return 1;
    }
    int ReadShort(lua_State* L) {
        lua_pushnumber(L, NETInputPacket::ReadShort());
        return 1;
    }
    int ReadByte(lua_State* L) {
        lua_pushnumber(L, NETInputPacket::ReadByte());
        return 1;
    }
    int ReadString(lua_State* L) {
        lua_pushstring(L, NETInputPacket::ReadString().c_str());
        return 1;
    }
    int Copy(lua_State* L) {
        void* buf = *(void**)lua_topointer(L, 2);
        int len = lua_tonumber(L, 3);
        bool ret = NETInputPacket::Copy(buf, len);
        lua_pushnumber(L, ret);
        return 1;
    }
    static mfunc_item<NETInputPacketAdapter> mfuncs[8];
};


class NETOutputPacketAdapter : public NETOutputPacket
{
public:
    int packet_buf(lua_State* L) {
        char* ret = NETOutputPacket::packet_buf();
        *(char**)lua_newuserdata(L,sizeof(char*)) = ret;
        return 1;
    }
    int packet_size(lua_State* L) {
        lua_pushnumber(L, NETOutputPacket::packet_size());
        return 1;
    }
    int WriteInt(lua_State* L) {
        int p1 = (int)lua_tonumber(L, 2);
        lua_pushnumber(L, NETOutputPacket::WriteInt(p1));
        return 1;
    }
    int WriteByte(lua_State* L) {
        BYTE p1 = (BYTE)lua_tonumber(L, 2);
        lua_pushnumber(L, NETOutputPacket::WriteByte(p1));
        return 1;
    }
    int WriteShort(lua_State* L) {
        short p1 = (short)lua_tonumber(L, 2);
        lua_pushnumber(L, NETOutputPacket::WriteShort(p1));
        return 1;
    }
    int InsertInt(lua_State* L) {
        int p1 = (int)lua_tonumber(L, 2);
        lua_pushnumber(L, NETOutputPacket::InsertInt(p1));
        return 1;
    }
    int InsertByte(lua_State* L) {
        BYTE p1 = (BYTE)lua_tonumber(L, 2);
        lua_pushnumber(L, NETOutputPacket::InsertByte(p1));
        return 1;
    }
    int WriteString(lua_State* L) {
        const char* p1 = lua_tostring(L, 2);
        lua_pushnumber(L, NETOutputPacket::WriteString(p1));
        return 1;
    }
    int Begin(lua_State* L) {
        short cmd = lua_tonumber(L, 2);
        NETOutputPacket::Begin(cmd);
        return 0;
    }
    int End(lua_State* L) {
        NETOutputPacket::End();
        return 0;
    }
    static mfunc_item<NETOutputPacketAdapter> mfuncs[11];
};

typedef NETInputPacketAdapter lua_inpack;
typedef NETOutputPacketAdapter lua_outpack;

#endif

