package impl;

import Chat.UserPOA;

/**
 * Created by HREN_VAM.
 */
public class UserImpl extends UserPOA {

    public void receive(String author, String message) {
        System.out.println(author + ": " + message);
    }
}
