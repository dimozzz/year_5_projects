package config;

/**
 * Created by HREN_VAM.
 */
public class Config{
    private static Config ourInstance = new Config();

    public static Config getInstance(){
        return ourInstance;
    }

    private Config(){
    }
}
