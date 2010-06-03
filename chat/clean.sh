if [ -d bin ]
then
    rm -Rf bin
fi

cd src/Client
ant clean

cd ../Server

if [ -d idl ]
then
    rm -Rf idl
fi
