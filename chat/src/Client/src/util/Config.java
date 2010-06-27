package util;

import java.io.FileReader;
import java.io.IOException;
import java.util.Properties;

/**
 * @author Sokolov.
 */
public class Config{

    private final Properties p = new Properties();

    private final String ourName;
    private final String serverName;

    public String getServerName(){
        return serverName;
    }

    public String getOurName(){
        return ourName;
    }

    private static Config ourInstance = new Config();

    public static Config getInstance(){
        return ourInstance;
    }

    private static String makePath(String... filenames){
        if(filenames.length == 0)
            return "";
        String res = filenames[0];
        for(int i = 1;i != filenames.length;++i){
            res += System.getProperty("file.separator") + filenames[i];
        }
        return res;
    }

    private Config(){
        try{
            p.load(new FileReader(makePath("..", "src", "Client", "conf", "main.properties")));
        } catch(IOException e){
            throw new RuntimeException(e);
        }
        serverName = p.getProperty("server.name");
        ourName = p.getProperty("our.name");
    }
}
