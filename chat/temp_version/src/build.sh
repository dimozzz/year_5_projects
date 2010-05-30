mkdir ../bin
g++ -ggdb `pkg-config --cflags --libs omniORB4` -l boost_thread -o ../bin/chat chat/main.cpp idl/chatSK.cc
g++ -ggdb `pkg-config --cflags --libs omniORB4` -o ../bin/chat_client chat_client/main.cpp idl/chatSK.cc
