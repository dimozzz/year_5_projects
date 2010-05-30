package util;

import java.io.FileReader;
import java.io.IOException;
import java.util.Properties;

/**
 * @author Sokolov.
 */
public class Config {

    private final Properties p = new Properties();

    private final String ourName;
    private final String serverName;

    public String getServerName() {
        return serverName;
    }

    public String getOurName() {
        return ourName;
    }

    private static Config ourInstance = new Config();

    public static Config getInstance() {
        return ourInstance;
    }

    private Config() {
        String separator = System.getProperty("file.separator");
        try {
            p.load(new FileReader(".." + separator + "Client" + separator + "conf" + separator +
                    "main.properties"));
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        serverName = p.getProperty("server.name");
        ourName = p.getProperty("our.name");
    }
}
