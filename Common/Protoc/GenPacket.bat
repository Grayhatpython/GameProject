pushd %~dp0
protoc.exe -I=./ --cpp_out=./ ./Enum.proto
protoc.exe -I=./ --cpp_out=./ ./Struct.proto
protoc.exe -I=./ --cpp_out=./ ./Protocol.proto

::protoc_3_12.exe -I=./ --csharp_out=./ ./Enum.proto
::protoc_3_12.exe -I=./ --csharp_out=./ ./Struct.proto
::protoc_3_12.exe -I=./ --csharp_out=./ ./Protocol.proto

IF ERRORLEVEL 1 PAUSE

PacketGenerator.exe

XCOPY /Y Enum.pb.h "../../GameServer"
XCOPY /Y Enum.pb.cc "../../GameServer"
XCOPY /Y Struct.pb.h "../../GameServer"
XCOPY /Y Struct.pb.cc "../../GameServer"
XCOPY /Y Protocol.pb.h "../../GameServer"
XCOPY /Y Protocol.pb.cc "../../GameServer"
XCOPY /Y ClientPacketHandler.h "../../GameServer"

XCOPY /Y Enum.pb.h "../../DummyClient"
XCOPY /Y Enum.pb.cc "../../DummyClient"
XCOPY /Y Struct.pb.h "../../DummyClient"
XCOPY /Y Struct.pb.cc "../../DummyClient"
XCOPY /Y Protocol.pb.h "../../DummyClient"
XCOPY /Y Protocol.pb.cc "../../DummyClient"
XCOPY /Y ServerPacketHandler.h "../../DummyClient"

XCOPY /Y Enum.pb.h "../../GameClient/src"
XCOPY /Y Enum.pb.cc "../../GameClient/src"
XCOPY /Y Struct.pb.h "../../GameClient/src"
XCOPY /Y Struct.pb.cc "../../GameClient/src"
XCOPY /Y Protocol.pb.h "../../GameClient/src"
XCOPY /Y Protocol.pb.cc "../../GameClient/src"
XCOPY /Y ServerPacketHandler.h "../../GameClient/src"

DEL /Q /F *.pb.h
DEL /Q /F *.pb.cc
DEL /Q /F *.h
DEL /Q /F *.cs

PAUSE