package impl.query;

import Chat.User;

/**
 * Created by HREN_VAM.
 */
public class Query{

    private User u;
    private String author;
    private String message;

    public Query(User u, String message){
        this.u = u;
        this.message = message;
    }

    public Query(String author, String message){
        this.author = author;
        this.message = message;
    }

    public User getUser(){
        return u;
    }

    public String getAuthor(){
        return author;
    }

    public String getMessage(){
        return message;
    }
}
