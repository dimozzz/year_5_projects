package impl;

import Chat.UserPOA;
import impl.query.Query;
import impl.query.QueryRunner;

import java.util.concurrent.BlockingQueue;

/**
 * Created by HREN_VAM.
 */
public class UserImpl extends UserPOA {

    private BlockingQueue<Query> queue;

    public UserImpl(BlockingQueue<Query> queue) {
        this.queue = queue;
    }

    public void receive(String author, String message) {
        queue.offer(new Query(author, message));
    }
}
