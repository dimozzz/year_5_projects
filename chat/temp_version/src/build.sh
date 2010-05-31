if ! [ -d ../bin ]
then
   mkdir ../bin
fi

cd ../bin
omniidl -bcxx -Wbh=.idl.h -Wbs=.idl.cpp ../src/idl/chat.idl
g++ -ggdb -I. -I../src `pkg-config --cflags --libs omniORB4` -l boost_thread -o chat ../src/chat/main.cpp chat.idl.cpp
g++ -ggdb -I. -I../src `pkg-config --cflags --libs omniORB4` -o chat_client ../src/chat_client/main.cpp chat.idl.cpp
