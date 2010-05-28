package impl;

import Chat.Server;
import Chat.User;
import impl.query.Query;

import java.util.concurrent.BlockingQueue;

/**
 * Created by HREN_VAM.
 */
public class Sender implements Runnable{

    private BlockingQueue<String> queue;
    private Server serv;
    private User u;

    public Sender(BlockingQueue<String> queue, Server serv, User u){
        this.queue = queue;
        this.serv = serv;
        this.u = u;
    }

    public void run(){
        try{
            while(true){
                String mes = queue.take();
                serv.send(u, mes);
            }
        }catch(InterruptedException e){
            throw new RuntimeException(e);
        }
    }
}
