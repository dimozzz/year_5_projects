package impl;

import Chat.Server;
import Chat.User;
import org.omg.CORBA.ORB;
import org.omg.CosNaming.NameComponent;
import org.omg.CosNaming.NamingContext;
import org.omg.CosNaming.NamingContextHelper;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/**
 * Created by HREN_VAM.
 */
public class Sender implements Runnable {

    private User user;
    private Server serv;

    public Sender(User user, Server serv) {
        this.user = user;
        this.serv = serv;
    }

    public void run() {
        try {
            BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
            while (true) {
                serv.send(user, in.readLine());
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
