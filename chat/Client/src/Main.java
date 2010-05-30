import Chat.*;
import util.Config;
import gui.MainFrame;
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
import util.Message;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * @author Sokolov.
 */
public class Main {

    private static Server getServer(ORB orb, String serverName) throws InvalidName,
            org.omg.CosNaming.NamingContextPackage.InvalidName, NotFound, CannotProceed {
        org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
        System.out.println("Resolved initial reference to NameService");
        NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);
        System.out.println("Naming context got");

        NameComponent[] path = {
                new NameComponent(serverName,"Object")
        };/*ncRef.to_name(serverName);*/
        return ServerHelper.narrow(ncRef.resolve(path));
    }

    private static User getUser(ORB orb, final BlockingQueue<Message> incomingMessages) throws InvalidName,
            AdapterInactive, WrongPolicy, ServantAlreadyActive, ServantNotActive {
        POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
        rootPOA.the_POAManager().activate();
        System.out.println("rootPOA activated");

        UserPOA userImpl = new UserPOA() {
            @Override
            public void receive(String author, String text) {
                incomingMessages.offer(new Message(author, text));
            }
        };
        rootPOA.activate_object(userImpl);

        return UserHelper.narrow(rootPOA.servant_to_reference(userImpl));
    }

    public static void main(String[] args) throws InvalidName, AdapterInactive,
            org.omg.CosNaming.NamingContextPackage.InvalidName, WrongPolicy,
            ServantAlreadyActive, ServantNotActive, NotFound, CannotProceed,
            InterruptedException, InvocationTargetException {
        Config config = Config.getInstance();
        String ourName = config.getOurName();
        if (args[args.length - 1].startsWith("name="))
        {
            ourName = args[args.length - 1].substring(5);
            String tmp[] = new String[args.length - 1];
            System.arraycopy(args, 0, tmp, 0, tmp.length);
            args = tmp;
        }
        final ORB orb = ORB.init(args, null);
        System.out.println("ORB initialized");

        final BlockingQueue<Message> incomingMessages = new LinkedBlockingQueue<Message>();
        final BlockingQueue<String> outcomingMessages = new LinkedBlockingQueue<String>();

        final Server server = getServer(orb, config.getServerName());
        System.out.println("Server created");
        final User user = getUser(orb, incomingMessages);
        System.out.println("User created");
        server.register(user, ourName);

        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    for (String message = outcomingMessages.take(); !message.equals(":quit");
                         message = outcomingMessages.take()) {
                        server.send(user, message);
                        System.out.println("send message: " + message);
                    }
                    System.out.println("Main: exit");
                    server.quit(user);
                    System.exit(0);
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

        FrameCreator f = new FrameCreator("Intelligent chat, you are " + ourName, outcomingMessages);
        SwingUtilities.invokeAndWait(f);
        while (true) {
            Message m = incomingMessages.take();
            System.out.println("got message: author = " + m.getAuthor() + ", text = " + m.getText());
            f.getFrame().publishMessage(m);
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
