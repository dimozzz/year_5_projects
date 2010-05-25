package impl;

import Chat.UserPOA;
import impl.query.Query;
import impl.query.QueryRunner;

import java.util.concurrent.BlockingQueue;

/**
 * Created by HREN_VAM.
 */
public class UserImpl extends UserPOA{

    public void receive(String author, String message){
        System.out.println(author + ": " + message);
    }
}
