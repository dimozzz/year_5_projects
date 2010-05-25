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

    public Sender(User user, Server serv){
        this.user = user;
        this.serv = serv;
        this.u = u;
    }

    public void run(){
        try{
            BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
            while(true){
                serv.send(user, in.readLine());
            }
        }catch(IOException e){
            throw new RuntimeException(e);
        }
    }
}
