package util;

public class Message implements Comparable<Message>{
    
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

    public int getTime(){
        return time;
    }

    @Override
    public int compareTo(Message o){
        return Double.compare(time, o.time);
    }

    public int hashCode(){
        return (text + time).hashCode();
    }

    public boolean equals(Object o){
        Message temp = (Message)o;
        return author.equals(temp.author) && (text.equals(temp.text)) && (time == temp.time);
    }

    public String toString(){
        return getText();
    }
}
