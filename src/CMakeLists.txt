include_directories(../../RakNet-bin/include ../../boost-bin/include .)
link_directories(/root/mmomm/RakNet-bin/lib)
link_libraries(RakNet pthread)
add_executable(mmomm initializePacketManager.cpp networkServer.cpp serverChat.cpp serverCollisionPackets.cpp serverMain.cpp serverNativePackets.cpp serverWorldInstance.cpp worldCommon.cpp worldInstance.cpp worldMap.cpp networkPacket.cpp networkCommon.cpp frameTimer.cpp collision.cpp playerActor.cpp actor.cpp user.cpp)
