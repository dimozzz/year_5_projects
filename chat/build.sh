cd idl
idlj -fall chat.idl
cd ..
cp -r idl/Chat Client/src
cd Client
ant
