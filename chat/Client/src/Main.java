import Chat.Server;
import Chat.ServerHelper;
import Chat.User;
import Chat.UserHelper;
import config.Config;
import impl.Reciever;
import impl.Sender;
import impl.UserImpl;
import org.omg.CORBA.ORB;
import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.CosNaming.NameComponent;
import org.omg.CosNaming.NamingContextExt;
import org.omg.CosNaming.NamingContextExtHelper;
import org.omg.CosNaming.NamingContextPackage.CannotProceed;
import org.omg.CosNaming.NamingContextPackage.NotFound;
import org.omg.PortableServer.POA;
import org.omg.PortableServer.POAHelper;
import org.omg.PortableServer.POAManagerPackage.AdapterInactive;
import org.omg.PortableServer.POAPackage.ServantAlreadyActive;
import org.omg.PortableServer.POAPackage.ServantNotActive;
import org.omg.PortableServer.POAPackage.WrongPolicy;

/**
 * Created by HREN_VAM.
 */
public class Main {

    public static void main(String[] args) throws InvalidName, AdapterInactive,
            org.omg.CosNaming.NamingContextPackage.InvalidName, WrongPolicy,
            ServantAlreadyActive, ServantNotActive, NotFound, CannotProceed {

        ORB orb = ORB.init(args, null);

        POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
        rootPOA.the_POAManager().activate();

        UserImpl userImpl = new UserImpl();
        rootPOA.activate_object(userImpl);

        User user = UserHelper.narrow(rootPOA.servant_to_reference(userImpl));

        org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
        NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

        NameComponent[] path = ncRef.to_name(Config.getInstance().getPath());
        Server serv = ServerHelper.narrow(ncRef.resolve(path));
        serv.register(user, Config.getInstance().getName());

        new Thread(new Sender(user, serv)).start();
        new Thread(new Reciever(orb)).start();
    }
}
