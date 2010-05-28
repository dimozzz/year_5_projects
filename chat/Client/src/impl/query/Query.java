package impl.query;

import Chat.User;

/**
 * Created by HREN_VAM.
 */
public class Query{

    private String author;
    private String message;

    public Query(String author, String message){
        this.author = author;
        this.message = message;
    }

    public String getAuthor(){
        return author;
    }

    public String getMessage(){
        return message;
    }
}
