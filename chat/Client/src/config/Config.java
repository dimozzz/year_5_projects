package config;

import java.io.FileReader;
import java.io.IOException;
import java.util.Properties;

/**
 * Created by HREN_VAM.
 */
public class Config{

    private Properties p;

    private int time;
    private String path;
    private String name;

    public int getTime(){
        return time;
    }

    public String getPath(){
        return path;
    }

    public String getName(){
        return name;
    }

    private static Config ourInstance = new Config();

    public static Config getInstance(){
        return ourInstance;
    }

    private Config(){
        p = new Properties();
        try{
            p.load(new FileReader("conf" + System.getProperty("file.separator") +
                    "main.properties"));
        }catch(IOException e){
            throw new RuntimeException(e);
        }
        time = Integer.parseInt(p.getProperty("time"));
        path = p.getProperty("path");
        name = p.getProperty("name");
    }
}
