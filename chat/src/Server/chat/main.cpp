#include <omniORB4/CORBA.h>
#include <omniORB4/poa.h>

#include "common/orb_initializer.h"
#include <iostream>

#include "chat.idl.h"
#include "server_i.h"

CORBA::Boolean bindObjectToName(CORBA::ORB_ptr orb, CORBA::Object_ptr objref)
{
    using namespace std;

    CosNaming::NamingContext_var rootContext;
    try
    {
        // Obtain a reference to the root context of the Name service:
        CORBA::Object_var obj;
        obj = orb->resolve_initial_references("NameService");
        // Narrow the reference returned.
        std::cout << "NameService has been resolved" << std::endl;
        rootContext = CosNaming::NamingContext::_narrow(obj);
        std::cout << "Root Context has been created" << std::endl;
        if( CORBA::is_nil(rootContext) )
        {
            cerr << "Failed to narrow the root naming context." << endl;
            return 0;
        }
    }
    catch (CORBA::NO_RESOURCES&)
    {
        cerr << "Caught NO_RESOURCES exception. You must configure omniORB "
             << "with the location" << endl
             << "of the naming service." << endl;
        return 0;
    }
    catch (CORBA::ORB::InvalidName&)
    {
        // This should not happen!
        cerr << "Service required is invalid [does not exist]." << endl;
        return 0;
    }
    catch (CORBA::TRANSIENT const & )
    {
        std::cout << "xui!" << std::endl;
        exit(239);
    }

    try
    {
        // Bind objref with name Echo to the testContext:
        CosNaming::Name objectName;
        objectName.length(1);
        objectName[0].id   = (const char*) "ChatServer";   // string copied
        objectName[0].kind = (const char*) "Object"; // string copied
        try
        {
            rootContext->bind(objectName, objref);
        }
        catch(CosNaming::NamingContext::AlreadyBound& )
        {
            rootContext->rebind(objectName, objref);
        }
        // Note: Using rebind() will overwrite any Object previously bound
        //       to /test/Echo with obj.
        //       Alternatively, bind() can be used, which will raise a
        //       CosNaming::NamingContext::AlreadyBound exception if the name
        //       supplied is already bound to an object.
        //       Amendment: When using OrbixNames, it is necessary to first try bind
        //       and then rebind, as rebind on own its will throw a NotFoundexception if
        //       the Name has not already been bound. [This is incorrect behaviour -
        //       it should just bind].
    }
    catch(CORBA::TRANSIENT& )
    {
        cerr << "Caught system exception TRANSIENT -- unable to contact the "
             << "naming service." << endl
             << "Make sure the naming server is running and that omniORB is "
             << "configured correctly." << endl;
        return 0;
    }
    catch(CORBA::SystemException& ex)
    {
        cerr << "Caught a CORBA::" << ex._name()
             << " while using the naming service." << endl;
        return 0;
    }
    return 1;
}


int main(int argc, char ** argv)
{
//    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);

	orb_initializer orb(argc, argv);

    CORBA::Object_var obj = orb.get()->resolve_initial_references("RootPOA");
    std::cout << "RootPOA has been created" << std::endl;
    PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);

    Server_i * myecho = new Server_i(orb.get());
    PortableServer::ObjectId_var myechoid = poa->activate_object(myecho);

    CORBA::Object_var obj2 = myecho->_this();
    CORBA::String_var sior(orb.get()->object_to_string(obj2));
    std::cout << sior << std::endl;

    if (!bindObjectToName(orb.get(), obj2))
        return 1;

    myecho->_remove_ref();

    PortableServer::POAManager_var pman = poa->the_POAManager();
    pman->activate();

    std::cout << "server has activated" << std::endl;
    orb.get()->run();

    return 0;
}
