import Chat.Server;
import Chat.ServerHelper;
import Chat.User;
import Chat.UserHelper;
import config.Config;
import gui.MainFrame;
import impl.Reciever;
import impl.Sender;
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
 * Created by HREN_VAM.
 */
public class Main{

    public static void main(String[] args)throws InvalidName, AdapterInactive,
            org.omg.CosNaming.NamingContextPackage.InvalidName, WrongPolicy,
            ServantAlreadyActive, ServantNotActive, NotFound, CannotProceed{

        ORB orb = ORB.init(args, null);

        POA rootPOA = POAHelper.narrow(orb.resolve_initial_references("RootPOA"));
        rootPOA.the_POAManager().activate();

        final BlockingQueue<Query> recieveQueue = new LinkedBlockingQueue<Query>();
        final BlockingQueue<String> sendQueue = new LinkedBlockingQueue<String>();

        UserImpl userImpl = new UserImpl(recieveQueue);
        rootPOA.activate_object(userImpl);

        final User user = UserHelper.narrow(rootPOA.servant_to_reference(userImpl));

        org.omg.CORBA.Object objRef = orb.resolve_initial_references("NameService");
        NamingContextExt ncRef = NamingContextExtHelper.narrow(objRef);

        NameComponent[] path = ncRef.to_name(Config.getInstance().getPath());
        Server serv = ServerHelper.narrow(ncRef.resolve(path));
        serv.register(user, Config.getInstance().getName());

        new Thread(new Sender(sendQueue, serv, user)).start();
        new Thread(new Reciever(orb)).start();

        FrameCreator f = new FrameCreator("ЧЯТ", sendQueue);
        try{
            SwingUtilities.invokeAndWait(f);
            while(true){
                Query q = recieveQueue.take();
                f.getFrame().addText(q.getAuthor() + ": " + q.getMessage());
            }
        }catch(InterruptedException e){
            throw new RuntimeException(e);
        } catch(InvocationTargetException e){
            throw new RuntimeException(e);
        }
    }

    private static class FrameCreator implements Runnable{

        private MainFrame f;
        private String title;
        private BlockingQueue<String> queue;

        public FrameCreator(String title, BlockingQueue<String> queue){
            this.title = title;
            this.queue = queue;
        }

        public void run(){
            f = new MainFrame(title, queue);
        }

        public MainFrame getFrame(){
            return f;
        }
    }
}
