if ! [ -d bin ]
then
   mkdir bin
fi

cd src/Server

if ! [ -d idl ]
then
    mkdir idl
fi

omniidl -C idl -bcxx -Wbh=.idl.h -Wbs=.idl.cpp ../idl/chat.idl
g++ -ggdb -I. -I idl `pkg-config --cflags --libs omniORB4` -l boost_thread -o ../../bin/chat chat/main.cpp idl/chat.idl.cpp
g++ -ggdb -I. -I idl `pkg-config --cflags --libs omniORB4` -o ../../bin/chat_client chat_client/main.cpp idl/chat.idl.cpp
