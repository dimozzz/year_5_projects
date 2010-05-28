package impl;

import Chat.Server;
import Chat.User;
import impl.query.Query;
import org.omg.CORBA.ORB;
import org.omg.CosNaming.NameComponent;
import org.omg.CosNaming.NamingContext;
import org.omg.CosNaming.NamingContextHelper;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.concurrent.BlockingQueue;

/**
 * Created by HREN_VAM.
 */
public class Sender implements Runnable{

    private BlockingQueue<Query> queue;
    private Server serv;
    private User u;

    public Sender(BlockingQueue<Query> queue, Server serv){
        this.queue = queue;
        this.serv = serv;
        this.u = u;
    }

    public void run(){
        try{
            while(true){
                Query q = queue.take();
                serv.send(q.getUser(), q.getMessage());
            }
        }catch(InterruptedException e){
            throw new RuntimeException(e);
        }
    }
}
