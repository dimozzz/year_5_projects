package impl;

import org.omg.CORBA.ORB;

/**
 * Created by HREN_VAM.
 */
public class Reciever implements Runnable {

    private ORB orb;

    public Reciever(ORB orb) {
        this.orb = orb;
    }

    public void run() {
        orb.run();
    }
}
