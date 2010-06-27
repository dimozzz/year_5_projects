package util;

public class Message{
    private final String author;
    private final String text;
    private final int time;

    public Message(String author, String text, int time){
        this.text = text;
        this.author = author;
        this.time = time;
    }

    public String getAuthor(){
        return author;
    }

    public String getText(){
        return text;
    }


}
