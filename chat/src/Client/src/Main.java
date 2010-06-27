import Chat.*;
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
import util.Config;
import util.Message;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

import java.util.Properties;

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
                new NameComponent(serverName, "Object")
        };/*ncRef.to_name(serverName);*/
        return ServerHelper.narrow(ncRef.resolve(path));
    }

    private static User getUser(ORB orb, final BlockingQueue<Message> incomingMessages, final MainFrame frame) throws InvalidName,
            AdapterInactive, WrongPolicy, ServantAlreadyActive, ServantNotActive {
        POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
        rootPOA.the_POAManager().activate();
        System.out.println("rootPOA activated");

        UserPOA userImpl = new UserPOA() {
            @Override
            public synchronized void receive(String author, String text) {
                incomingMessages.offer(new Message(author, text));
            }

            @Override
            public void addUser(String name) {
                frame.addUser(name);
            }

            @Override
            public void removeUser(String name) {
                frame.removeUser(name);
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

		Properties p = new Properties();    
		p.setProperty("com.sun.CORBA.codeset.charsets", "0x05010001"); // UTF-8
		p.setProperty("com.sun.CORBA.codeset.wcharsets", "0x00010109, 0x05010001"); // UTF-16, UTF-8

        final ORB orb = ORB.init(args, p);
        System.out.println("ORB initialized");

        final BlockingQueue<Message> incomingMessages = new LinkedBlockingQueue<Message>();
        final BlockingQueue<String> outcomingMessages = new LinkedBlockingQueue<String>();

        final Server server = getServer(orb, config.getServerName());
        System.out.println("Server created");

        FrameCreator f = new FrameCreator(outcomingMessages);
        SwingUtilities.invokeAndWait(f);

        final User user = getUser(orb, incomingMessages, f.getFrame() );
        System.out.println("User created");

        String ourName;
        ourName = javax.swing.JOptionPane.showInputDialog("Type your name");
        while ( !server.register(user, ourName)) {
            javax.swing.JOptionPane.showMessageDialog(null, "There has been user with name " + ourName, "", javax.swing.JOptionPane.WARNING_MESSAGE);
            ourName = javax.swing.JOptionPane.showInputDialog("Type your name");
                
        }
        System.out.println("User registered");
        f.getFrame().setTitle("You are " + ourName);

        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    for (String message = outcomingMessages.take(); !message.equals(":quit");
                         message = outcomingMessages.take()) {
                        server.send(user, message);
                        System.out.println("send message: " + message);
                    }
                    server.quit(user);
                    System.exit(0);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
            }
        }).start();

        while (true) {
            Message m = incomingMessages.take();
            if ( m.getAuthor().equals("alive") && m.getText().equals(""))
                continue;
            System.out.println("got message: author = " + m.getAuthor() + ", text = " + m.getText());
            f.getFrame().publishMessage(m);
        }
    }

    private static class FrameCreator implements Runnable {

        private MainFrame f;
        private final BlockingQueue<String> queue;

        public FrameCreator(BlockingQueue<String> queue) {
            this.queue = queue;
        }

        public void run() {
            f = new MainFrame(queue);
        }

        public MainFrame getFrame() {
            return f;
        }
    }
}
