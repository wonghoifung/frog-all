cmd_stoploop = 0
cmd_login = 1
cmd_logout = 2
cmd_client_say = 3

local stop = false

local cmd_callbacks = {}

local onlinemembers = {}

local function get_memberinfo(mid)
    for k,v in pairs(onlinemembers) do
        if k==mid then
            return v
        end
    end
    return nil
end

function tablelength(T)
  local count = 0
  for _ in pairs(T) do count = count + 1 end
  return count
end

local function handle_stoploop(msg)
    stop = true
end

local function handle_login(msg)
    mid = msg:ReadInt()
    pwd = msg:ReadString()
    print("handle_login mid:"..mid..", pwd:"..pwd)
    onlinemembers[mid] = pwd -- should hold a connection instead of pwd
end

local function handle_logout(msg)
    mid = msg:ReadInt()
    info = get_memberinfo(mid)
    if info==nil then
        print("handle_logout mid:"..mid.." was not online")
        return
    end
    print("handle_logout mid:"..mid..", info:"..info..", online left:"..tablelength(onlinemembers))
    for k,v in pairs(onlinemembers) do
        if k==mid then
            onlinemembers[k]=nil
        end
    end
end

local function handle_client_say(msg)
    mid = msg:ReadInt()
    info = get_memberinfo(mid)
    if info==nil then
        print("handle_client_say mid:"..mid.." was not online")
        return
    end
    content = msg:ReadString()
    print("handle_client_say mid:"..mid.." info:"..info.." say:"..content..", online left:"..tablelength(onlinemembers))
end

local function init_callbacks()
    cmd_callbacks[cmd_stoploop] = handle_stoploop
    cmd_callbacks[cmd_login] = handle_login
    cmd_callbacks[cmd_logout] = handle_logout
    cmd_callbacks[cmd_client_say] = handle_client_say
end

local function dispatch_msg(msg)
    cmd = msg:GetCmdType()
    if cmd_callbacks[cmd] then
        cmd_callbacks[cmd](msg)
    end
end

local function dump_onlinemembers()
    --print("dump_onlinemembers----------------onlinemembers count:"..#onlinemembers)
    if tablelength(onlinemembers)>0 then
        print("") --empty line
        print("dump onlinemembers==========")
        for k,v in pairs(onlinemembers) do
            print("k:"..k..", v:"..v)
        end
        print("dump done===================")
        print("") --empty line
    end
end

msgrecvcount = cppgetmsgcnt()

local function runloop()
    while stop~=true do
        msg = getmsg()
        if msg~=nil then
            dispatch_msg(msg)
            msgrecvcount = msgrecvcount + 1
        else
            print("nothing received")
        end
        print(">>>>>>>>> msg recv count: "..msgrecvcount)
        --dump_onlinemembers()
        --sleep(1)
    end
    cppsetmsgcnt(msgrecvcount)
end

init_callbacks()
print("loop start...")
runloop()
print("loop stop...")


----------------------------------------
version=2.1
name="abbbbbz"

people={name="albert",age=30,com="fl"}

function main()
	print("hello lua")
end

--main()

function getstr(name1,name2)
	return "hi "..name1.." "..name2
end

--print(getstr("a","b"))
----------------------------------------
-- Meta class
Shape = {area = 0}
-- Base class method new
function Shape:new(o,side)
  o = o or {}
  setmetatable(o, self)
  self.__index = self
  side = side or 0
  self.area = side*side;
  return o
end
-- Base class method printArea
function Shape:printArea()
  print("The area is ",self.area)
end

-- Creating an object
myshape = Shape:new(nil,10)
--myshape:printArea()

----------------------------------------

Rectangle = Shape:new()
-- Derived class method new
function Rectangle:new(o,length,breadth)
  o = o or Shape:new(o)
  setmetatable(o, self)
  self.__index = self
  self.area = length * breadth
  return o
end

-- Derived class method printArea
function Rectangle:printArea()
  print("The area of Rectangle is ",self.area)
end

-- Creating an object
myrectangle = Rectangle:new(nil,10,20)
--myrectangle:printArea()

----------------------------------------
