import Chat.Server;
import Chat.ServerHelper;
import Chat.User;
import Chat.UserHelper;
import config.Config;
import gui.MainFrame;
import impl.UserImpl;
import impl.query.Query;
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

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * @author Sokolov.
 */
public class Main {

    private static Server getServer(ORB orb, String serverName) throws InvalidName,
            org.omg.CosNaming.NamingContextPackage.InvalidName, NotFound, CannotProceed {
        org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
        System.out.println("Resolved initial reference to NameService");
        NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

        NameComponent[] path = ncRef.to_name(serverName);
        return ServerHelper.narrow(ncRef.resolve(path));
    }

    private static User getUser(ORB orb, BlockingQueue<Query> incomingMessages) throws InvalidName,
            AdapterInactive, WrongPolicy, ServantAlreadyActive, ServantNotActive {
        POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
        rootPOA.the_POAManager().activate();
        System.out.println("rootPOA activated");

        UserImpl userImpl = new UserImpl(incomingMessages);
        rootPOA.activate_object(userImpl);

        return UserHelper.narrow(rootPOA.servant_to_reference(userImpl));
    }

    public static void main(String[] args) throws InvalidName, AdapterInactive,
            org.omg.CosNaming.NamingContextPackage.InvalidName, WrongPolicy,
            ServantAlreadyActive, ServantNotActive, NotFound, CannotProceed,
            InterruptedException, InvocationTargetException {
        Config config = Config.getInstance();
        final ORB orb = ORB.init(args, null);
        System.out.println("ORB initialized");


        final BlockingQueue<Query> incomingMessages = new LinkedBlockingQueue<Query>();
        final BlockingQueue<String> outcomingMessages = new LinkedBlockingQueue<String>();

        final Server server = getServer(orb, config.getServerName());
        System.out.println("Server created");
        final User user = getUser(orb, incomingMessages);
        System.out.println("User created");
        server.register(user, config.getOurName());

        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    while (true) {
                        server.send(user, outcomingMessages.take());
                    }
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
            }
        }).start();
        new Thread(new Runnable() {
            @Override
            public void run() {
                orb.run();
            }
        }).start();

        FrameCreator f = new FrameCreator("ЧЯТ", outcomingMessages);
        SwingUtilities.invokeAndWait(f);
        while (true) {
            Query q = incomingMessages.take();
            f.getFrame().addText(q.getAuthor() + ": " + q.getMessage());
        }
    }

    private static class FrameCreator implements Runnable {

        private MainFrame f;
        private String title;
        private BlockingQueue<String> queue;

        public FrameCreator(String title, BlockingQueue<String> queue) {
            this.title = title;
            this.queue = queue;
        }

        public void run() {
            f = new MainFrame(title, queue);
        }

        public MainFrame getFrame() {
            return f;
        }
    }
}
