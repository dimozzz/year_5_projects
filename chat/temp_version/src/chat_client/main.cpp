#include <omniORB4/CORBA.h>
#include <omniORB4/poa.h>

#include "../idl/chat.hh"

#include <iostream>
using namespace std;

CORBA::Object_ptr getObjectReference(CORBA::ORB_ptr orb);

struct User_i : POA_Chat::User
{
    void receive(const char* author, const char* message)
    {
        std::cout << author << ": " << message << std::endl;
    }
};

int main( int argc, char** argv )
{
    try {
        CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

        CORBA::Object_var poa_obj = orb->resolve_initial_references("RootPOA");
        PortableServer::POA_var poa = PortableServer::POA::_narrow(poa_obj);

        User_i * u = new User_i();
        PortableServer::ObjectId_var myechoid = poa->activate_object(u);

        Chat::User_ptr u_ptr = u->_this();

        CORBA::String_var src = "Hello!";

        PortableServer::POAManager_var pman = poa->the_POAManager();
        pman->activate();

        CORBA::Object_var obj = getObjectReference(orb);
        Chat::Server_var echoref = Chat::Server::_narrow(obj);

        if ( CORBA::is_nil(echoref) ) {
            cerr << "hello: The object reference is nil!\n" << endl;
            return 1;
        }

        echoref->_cxx_register(u_ptr, argv[1]);
        echoref->send(u_ptr, src);

        u->_remove_ref();

        orb->run();
    }
    catch (CORBA::TRANSIENT&) {
        cerr << "Caught system exception TRANSIENT -- unable to contact the "
             << "server." << endl;
    }
    catch (CORBA::SystemException & ex) {
        cerr << "Caught a CORBA::" << ex._name() << endl;
    }
    catch ( CORBA::Exception & ex ) {
        cerr << "Caught CORBA::Exception: " << ex._name() << endl;
    }
    catch ( omniORB::fatalException & fe ) {
        cerr << "Caught omniORB::fatalException:" << endl;
        cerr << "  file: " << fe.file() << endl;
        cerr << "  line: " << fe.line() << endl;
        cerr << "  mesg: " << fe.errmsg() << endl;
    }
    return 0;
}

static CORBA::Object_ptr getObjectReference(CORBA::ORB_ptr orb)
{
    CosNaming::NamingContext_var rootContext;

    try {
        CORBA::Object_var obj = orb->resolve_initial_references( "NameService");
        rootContext = CosNaming::NamingContext::_narrow(obj);
        if ( CORBA::is_nil(rootContext) ) {
            cerr << "Failed to narrow the root naming context." << endl;
            return CORBA::Object::_nil();
        }
    }
    catch (CORBA::NO_RESOURCES & ) {
        cerr << "Caught NO_RESOURCES exception. You must configure omniORB "
             << "with the location" << endl
             << "of the naming service." << endl;
        return 0;
    }
    catch ( CORBA::ORB::InvalidName & ) {
        cerr << "Service required is invalid [does not exist]." << endl;
        return CORBA::Object::_nil();
    }

    CosNaming::Name name;
    name.length( 2 );

    name[0].id = "test";
    name[0].kind = "my_context";
    name[1].id = "Echo";
    name[1].kind = "Object";

    try {
        return rootContext->resolve(name);
    }
    catch (CosNaming::NamingContext::NotFound & ) {
        cerr << "Context not found." << endl;
    }
    catch (CORBA::TRANSIENT &  ) {
        cerr << "Caught system exception TRANSIENT -- unable to contact the "
            << "naming service." << endl
            << "Make sure the naming service is running and that omniORB is "
            << "configured correctly." << endl;
    }
    catch (CORBA::SystemException & ex ) {
        cerr << "Caught a CORBA::" << ex._name()
            << " while using the naming service." << endl;
        return 0;
    }
    return CORBA::Object::_nil();
}